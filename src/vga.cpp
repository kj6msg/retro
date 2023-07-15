////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/color.hpp"
#include "retro/sprite.hpp"
#include "retro/types.hpp"
#include "retro/vga.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <map>
#include <span>
#include <stdexcept>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
namespace
{

////////////////////////////////////////////////////////////////////////////////
constexpr std::array<retro::color, 16> ega_palette
{
    retro::color::black,
    retro::color::blue,
    retro::color::green,
    retro::color::cyan,
    retro::color::red,
    retro::color::magenta,
    retro::color::brown,
    retro::color::white,
    retro::color::dark_gray,
    retro::color::bright_blue,
    retro::color::bright_green,
    retro::color::bright_cyan,
    retro::color::bright_red,
    retro::color::bright_magenta,
    retro::color::bright_yellow,
    retro::color::bright_white,
};


////////////////////////////////////////////////////////////////////////////////
extern "C" const std::array<std::byte, 8 * 256> glyphs_8x8;
extern "C" const std::array<std::byte, 14 * 256> glyphs_8x14;
extern "C" const std::array<std::byte, 16 * 256> glyphs_8x16;

const retro::font vga_8x8{glyphs_8x8, 8, 8};
const retro::font ega_8x14{glyphs_8x14, 8, 14};
const retro::font vga_8x16{glyphs_8x16, 8, 16};
const retro::font vga_9x16{glyphs_8x16, 9, 16};


////////////////////////////////////////////////////////////////////////////////
struct vga_mode
{
    int width{};
    int height{};
    int num_colors{};
    const retro::font& font;
};

constexpr vga_mode vga_03h{720, 400, 16, vga_9x16};
constexpr vga_mode ega_0dh{320, 200, 16, vga_8x8};
constexpr vga_mode ega_0eh{640, 200, 16, vga_8x8};
constexpr vga_mode ega_10h{640, 350, 16, ega_8x14};
constexpr vga_mode vga_12h{640, 480, 16, vga_8x16};
constexpr vga_mode vga_13h{320, 200, 256, vga_8x8};


////////////////////////////////////////////////////////////////////////////////
const std::map<retro::vga::mode, const vga_mode&> vga_modes
{
    {retro::vga::mode::vga_03h, vga_03h},
    {retro::vga::mode::ega_0dh, ega_0dh},
    {retro::vga::mode::ega_0eh, ega_0eh},
    {retro::vga::mode::ega_10h, ega_10h},
    {retro::vga::mode::vga_12h, vga_12h},
    {retro::vga::mode::vga_13h, vga_13h}
};

}   // unnamed


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
vga::vga(const mode video_mode)
{
    SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &m_window, &m_renderer);

    if(m_window == nullptr || m_renderer == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }

    set_mode(video_mode);

    reset_palette();
}


////////////////////////////////////////////////////////////////////////////////
vga::~vga()
{
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    m_texture  = nullptr;
    m_renderer = nullptr;
    m_window   = nullptr;
}


////////////////////////////////////////////////////////////////////////////////
void vga::blit(const std::span<const pixel_t> source)
{
    std::copy(source.begin(), source.end(), m_vram.begin());
}


////////////////////////////////////////////////////////////////////////////////
void vga::blit(const sprite& source)
{
    const std::span<const pixel_t> pixels{source.m_texture};
    auto ram_it = std::next(m_vram.begin(), xy_to_addr(source.m_x, source.m_y));

    for(auto offset{0uz}; offset != pixels.size(); offset += static_cast<std::size_t>(source.m_width))
    {
        const auto line = pixels.subspan(offset, static_cast<std::size_t>(source.m_width));
        std::copy(line.begin(), line.end(), ram_it);
        std::advance(ram_it, m_width);
    }
}


////////////////////////////////////////////////////////////////////////////////
color vga::get_color(const int index) const
{
    return m_palette.at(static_cast<std::size_t>(index));
}


////////////////////////////////////////////////////////////////////////////////
void vga::putchar(const unsigned char c, const pixel_t fg, const pixel_t bg, const int x, const int y)
{
    const auto [font_width, font_height] = m_font.size();
    sprite s(font_width, font_height, m_font.glyph(c, fg, bg));
    s.position(x, y);
    blit(s);
}


////////////////////////////////////////////////////////////////////////////////
void vga::reset_palette()
{
    auto it = std::copy(ega_palette.cbegin(), ega_palette.cend(), m_palette.begin());
    std::fill(it, m_palette.end(), color());
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_color(const int index, const color& c)
{
    m_palette.at(static_cast<std::size_t>(index)) = c;
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_mode(const vga::mode video_mode)
{
    const auto mode = vga_modes.at(video_mode);
    m_width = mode.width;
    m_height = mode.height;
    m_num_colors = mode.num_colors;

    m_vram.resize(m_width * m_height);
    m_palette.resize(m_num_colors);
    m_font = mode.font;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(m_renderer, m_width, m_height);

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);

    if(m_texture != nullptr)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING, m_width, m_height);

    if(m_texture == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_palette(const std::span<const color> colors)
{
    std::copy(colors.begin(), colors.end(), m_palette.begin());
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_pixel(const int x, const int y, const pixel_t color_index)
{
    m_vram.at(xy_to_addr(x, y)) = color_index;
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_pixel(const SDL_Point& position, const pixel_t color_index)
{
    m_vram.at(xy_to_addr(position.x, position.y)) = color_index;
}


////////////////////////////////////////////////////////////////////////////////
void vga::show()
{
    std::vector<argb_t> pixels(m_width * m_height);

    std::transform(m_vram.cbegin(), m_vram.cend(), pixels.begin(),[&](auto i)
    {
        return m_palette.at(static_cast<std::size_t>(i)).to_argb();
    });

    const int pitch = m_width * sizeof(argb_t);
    SDL_UpdateTexture(m_texture, nullptr, pixels.data(), pitch);

    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}


////////////////////////////////////////////////////////////////////////////////
constexpr std::size_t vga::xy_to_addr(const int x, const int y) noexcept
{
    return static_cast<std::size_t>(x + m_width * y);
}

}   // retro
