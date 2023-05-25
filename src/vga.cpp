////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/vga.hpp"
#include "video.hpp"

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
/// \brief Convert (x,y) coordinate to linear address.
/// \param x x location
/// \param y y location
/// \param width width of a line in pixels
/// \return linear address of coordinate
////////////////////////////////////////////////////////////////////////////////
constexpr std::size_t addr(int x, int y, int width) noexcept
{
    return static_cast<std::size_t>(x + width * y);
}


////////////////////////////////////////////////////////////////////////////////
vga::vga(const mode video_mode)
{
    const auto mode = video::modes.at(video_mode);
    m_width = mode.width;
    m_height = mode.height;
    m_num_colors = mode.num_colors;

    m_ram.resize(m_width * m_height);
    m_palette.resize(m_num_colors);
    m_pixels.resize(m_width * m_height);

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
void vga::blit(const std::span<const int> source)
{
    std::copy(source.cbegin(), source.cend(), m_ram.begin());
}


////////////////////////////////////////////////////////////////////////////////
void vga::blit(const std::span<const int> source, const SDL_Point& pos, const int w)
{
    auto ram_it = std::next(m_ram.begin(), addr(pos.x, pos.y, m_width));

    for(std::size_t offset{0}; offset < source.size(); offset += static_cast<std::size_t>(w))
    {
        const auto line = source.subspan(offset, static_cast<std::size_t>(w));
        std::copy(line.begin(), line.end(), ram_it);
        std::advance(ram_it, m_width);
    }
}


////////////////////////////////////////////////////////////////////////////////
vga::color_t vga::get_color(const int index) const
{
    return m_palette.at(static_cast<std::size_t>(index));
}


////////////////////////////////////////////////////////////////////////////////
void vga::reset_palette()
{
    auto it = std::copy(video::ega_palette.cbegin(), video::ega_palette.cend(), m_palette.begin());
    std::fill(it, m_palette.end(), make_color(0, 0, 0));
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_color(const int index, const color_t color)
{
    m_palette.at(static_cast<std::size_t>(index)) = color;
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_palette(const std::span<const color_t> colors)
{
    std::copy(colors.cbegin(), colors.cend(), m_palette.begin());
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_pixel(const int x, const int y, const int color_index)
{
    m_ram.at(addr(x, y, m_width)) = color_index;
}


////////////////////////////////////////////////////////////////////////////////
void vga::show()
{
    std::transform(m_ram.cbegin(), m_ram.cend(), m_pixels.begin(),[&](auto i)
    {
        return m_palette.at(static_cast<std::size_t>(i));
    });

    const int pitch = m_width * sizeof(color_t);
    SDL_UpdateTexture(m_texture, nullptr, m_pixels.data(), pitch);

    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

}   // retro
