////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/color.hpp"
#include "retro/sprite.hpp"
#include "retro/vga.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <map>
#include <ranges>
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
const retro::font vga_9x16{glyphs_8x16, 9, 16};     // uses 8x16 glyphs


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
void vga::blit(const std::span<const int> source)
{
    std::ranges::copy(source, m_vram.begin());
}


////////////////////////////////////////////////////////////////////////////////
void vga::blit(const sprite& source)
{
    const auto [x0, y0] = source.position();
    const auto [width, height] = source.size();

    // sprite completely out of bounds
    if((x0 + width) < 0 || x0 >= m_width || (y0 + height) < 0 || y0 >= m_height)
    {
        return;
    }

    const auto pixels = source.pixels();

    // bounding box and on-screen adjusted coordinates
    const auto l0 = std::max(0, -y0);                   // first visible line
    const auto l1 = std::min(height, m_height - y0);    // last visible line
    const auto w0 = std::max(0, -x0);                   // line start
    const auto w1 = std::min(width - w0, m_width - x0); // line end
    const auto x1 = std::max(0, x0);                    // adjust on-screen x
    const auto y1 = std::max(0, y0);                    // adjust on-screen y

    for(const auto line : std::views::iota(l0, l1))
    {
        const auto p = pixels | std::views::drop(width * line + w0) | std::views::take(w1);
        const auto v = m_vram | std::views::drop(xy_to_index(x1, y1 + (line - l0)));
        std::ranges::copy(p, v.begin());
    }
}


////////////////////////////////////////////////////////////////////////////////
color vga::get_color(const int index) const
{
    if(index < 0 || index >= std::ssize(m_palette))
    {
        std::invalid_argument("vga::get_color has an invalid argument");
    }

    return m_palette[static_cast<std::size_t>(index)];
}


////////////////////////////////////////////////////////////////////////////////
std::pair<int, int> vga::get_cursor() const noexcept
{
    return std::make_pair(m_cursor_col, m_cursor_row);
}


////////////////////////////////////////////////////////////////////////////////
font vga::get_font() const noexcept
{
    return m_font;
}


////////////////////////////////////////////////////////////////////////////////
std::vector<color> vga::get_palette() const noexcept
{
    return m_palette;
}


////////////////////////////////////////////////////////////////////////////////
int vga::get_pixel(const int x, const int y) const
{
    if(x < 0 || y < 0)
    {
        throw std::invalid_argument("vga::get_pixel has an invalid argument");
    }
    
    return m_vram[xy_to_index(x % m_width, y % m_height)];
}


////////////////////////////////////////////////////////////////////////////////
void vga::putchar(const unsigned char c, const int fg)
{
    if(fg < 0 || fg >= std::ssize(m_palette))
    {
        throw std::invalid_argument("vga::putchar has an invalid argument");
    }
    
    const auto [width, height] = m_font.size();
    const auto glyph = m_font.glyph(c, fg, 0);
    sprite s{width, height, glyph};

    const auto x = width * m_cursor_col;
    const auto y = height * m_cursor_row;
    s.position(x, y);

    blit(s);
}


////////////////////////////////////////////////////////////////////////////////
void vga::reset_palette()
{
    auto it = std::ranges::copy(ega_palette, m_palette.begin());
    std::fill(it.out, m_palette.end(), color::black);
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_color(const int index, const color& c)
{
    if(index < 0 || index >= std::ssize(m_palette))
    {
        throw std::invalid_argument("vga::set_color has an invalid argument");
    }
    
    m_palette[static_cast<std::size_t>(index)] = c;
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_cursor(const int col, const int row)
{
    m_cursor_col = col % m_columns;
    m_cursor_row = row % m_rows;
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_font(const font& f)
{
    m_font = f;
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_mode(const vga::mode video_mode)
{
    const auto mode = vga_modes.at(video_mode);
    m_width = mode.width;
    m_height = mode.height;
    m_num_colors = mode.num_colors;
    m_vram.resize(static_cast<std::size_t>(m_width * m_height));
    m_palette.resize(static_cast<std::size_t>(m_num_colors));
    
    m_font = mode.font;
    const auto [font_w, font_h] = m_font.size();
    m_columns = m_width / font_w;
    m_rows = m_height / font_h;
    m_cursor_col = 0;
    m_cursor_row = 0;

    m_pixels.resize(static_cast<std::size_t>(m_width * m_height));

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
    if(colors.size() > m_palette.size())
    {
        throw std::invalid_argument("vga::set_palette has an invalid argument");
    }

    std::ranges::copy(colors, m_palette.begin());
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_pixel(const int x, const int y, const int color_index)
{
    if(color_index < 0 || color_index >= std::ssize(m_palette))
    {
        throw std::invalid_argument("vga::set_pixel has an invalid argument");
    }

    if(x < 0 || y < 0)
    {
        return;
    }

    m_vram[xy_to_index(x % m_width, y % m_height)] = color_index;
}


////////////////////////////////////////////////////////////////////////////////
void vga::show()
{
    std::ranges::transform(m_vram, m_pixels.begin(), [&](auto i)
    {
        return m_palette[static_cast<std::size_t>(i)].to_argb();
    });

    const auto pitch = m_width * static_cast<int>(sizeof(std::uint32_t));
    SDL_UpdateTexture(m_texture, nullptr, m_pixels.data(), pitch);

    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}


////////////////////////////////////////////////////////////////////////////////
constexpr std::size_t vga::xy_to_index(const int x, const int y) const noexcept
{
    return static_cast<std::size_t>(x + m_width * y);
}

}   // retro
