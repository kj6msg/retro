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
#include <iterator>
#include <map>
#include <span>
#include <stdexcept>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
static const std::array<color, 16> ega_palette
{
    color(0, 0, 0),        // black
    color(0, 0, 170),      // blue
    color(0, 170, 0),      // green
    color(0, 170, 170),    // cyan
    color(170, 0, 0),      // red
    color(170, 0, 170),    // magenta
    color(170, 85, 0),     // brown
    color(170, 170, 170),  // white
    color(85, 85, 85),     // dark gray
    color(85, 85, 255),    // bright blue
    color(85, 255, 85),    // bright green
    color(85, 255, 255),   // bright cyan
    color(255, 85, 85),    // bright red
    color(255, 85, 255),   // bright magenta
    color(255, 255, 85),   // bright yellow
    color(255, 255, 255)   // bright white
};


////////////////////////////////////////////////////////////////////////////////
struct vga_mode
{
    enum class type
    {
        text,
        graphics
    };

    int width{};
    int height{};
    int num_colors{};
    type type;
};

static constexpr vga_mode vga_03h{720, 400, 16, vga_mode::type::text};
static constexpr vga_mode ega_0dh{320, 200, 16, vga_mode::type::graphics};
static constexpr vga_mode ega_0eh{640, 200, 16, vga_mode::type::graphics};
static constexpr vga_mode vga_12h{640, 480, 16, vga_mode::type::graphics};
static constexpr vga_mode vga_13h{320, 200, 256, vga_mode::type::graphics};

static const std::map<vga::mode, vga_mode> vga_modes
{
    {vga::mode::vga_03h, vga_03h},
    {vga::mode::ega_0dh, ega_0dh},
    {vga::mode::ega_0eh, ega_0eh},
    {vga::mode::vga_12h, vga_12h},
    {vga::mode::vga_13h, vga_13h}
};


////////////////////////////////////////////////////////////////////////////////
/// \brief Convert (x,y) coordinate to linear address.
/// \param x x location
/// \param y y location
/// \param width width of a line in pixels
/// \return linear address of coordinate
////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr std::size_t addr(int x, int y, int width) noexcept
{
    return static_cast<std::size_t>(x + width * y);
}


////////////////////////////////////////////////////////////////////////////////
vga::vga(const mode video_mode)
{
    const auto mode = vga_modes.at(video_mode);
    m_width = mode.width;
    m_height = mode.height;
    m_num_colors = mode.num_colors;

    m_vram.resize(m_width * m_height);
    m_palette.resize(m_num_colors);

    SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &m_window, &m_renderer);

    if(m_window == nullptr || m_renderer == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(m_renderer, m_width, m_height);

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING, m_width, m_height);

    if(m_texture == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }

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
    std::copy(source.cbegin(), source.cend(), m_vram.begin());
}


////////////////////////////////////////////////////////////////////////////////
void vga::blit(const sprite& source)
{
    const std::span<const pixel_t> pixels{source.m_texture};
    auto ram_it = std::next(m_vram.begin(), addr(source.m_x, source.m_y, source.m_width));

    for(std::size_t offset{0}; offset != pixels.size(); offset += static_cast<std::size_t>(source.m_width))
    {
        const auto line = pixels.subspan(offset, static_cast<std::size_t>(source.m_width));
        std::copy(line.cbegin(), line.cend(), ram_it);
        std::advance(ram_it, m_width);
    }
}


////////////////////////////////////////////////////////////////////////////////
color vga::get_color(const int index) const
{
    return m_palette.at(static_cast<std::size_t>(index));
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
void vga::set_palette(const std::span<const color> colors)
{
    std::copy(colors.cbegin(), colors.cend(), m_palette.begin());
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_pixel(const int x, const int y, const pixel_t color_index)
{
    m_vram.at(addr(x, y, m_width)) = color_index;
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_pixel(const SDL_Point& position, const pixel_t color_index)
{
    m_vram.at(addr(position.x, position.y, m_width)) = color_index;
}


////////////////////////////////////////////////////////////////////////////////
void vga::show()
{
    std::vector<color::argb_t> pixels(m_width * m_height);

    std::transform(m_vram.cbegin(), m_vram.cend(), pixels.begin(),[&](auto i)
    {
        return m_palette.at(static_cast<std::size_t>(i)).to_argb();
    });

    const int pitch = m_width * sizeof(color::argb_t);
    SDL_UpdateTexture(m_texture, nullptr, pixels.data(), pitch);

    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

}   // retro
