////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/vga.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <array>
#include <stdexcept>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
static constexpr int width{320};
static constexpr int height{200};

static constexpr std::array<vga::color_t, 16> default_palette
{
    make_color(0, 0, 0),        // black
    make_color(0, 0, 170),      // blue
    make_color(0, 170, 0),      // green
    make_color(0, 170, 170),    // cyan
    make_color(170, 0, 0),      // red
    make_color(170, 0, 170),    // magenta
    make_color(170, 85, 0),     // brown
    make_color(170, 170, 170),  // light gray
    make_color(85, 85, 85),     // dark gray
    make_color(85, 85, 255),    // light blue
    make_color(85, 255, 85),    // light green
    make_color(85, 255, 255),   // light cyan
    make_color(255, 85, 85),    // light red
    make_color(255, 85, 255),   // light magenta
    make_color(255, 255, 85),   // yellow
    make_color(255, 255, 255)   // white
};


////////////////////////////////////////////////////////////////////////////////
vga::vga() : m_ram(width * height), m_palette(256), m_pixels(width * height)
{
    SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &m_window, &m_renderer);

    if(m_window == nullptr || m_renderer == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(m_renderer, width, height);

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING, width, height);

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
void vga::blit(const std::vector<int>& source)
{
    std::copy(source.begin(), source.end(), m_ram.begin());
}


////////////////////////////////////////////////////////////////////////////////
void vga::reset_palette()
{
    auto it = std::copy(default_palette.begin(), default_palette.end(), m_palette.begin());
    std::fill(it, m_palette.end(), make_color(0, 0, 0));
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_color(const int index, const color_t color)
{
    m_palette.at(index) = color;
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_palette(const std::vector<color_t>& colors)
{
    std::copy(colors.begin(), colors.end(), m_palette.begin());
}


////////////////////////////////////////////////////////////////////////////////
void vga::set_pixel(const int x, const int y, const int color_index)
{
    const auto addr = x + width * y;
    m_ram.at(addr) = color_index;
}


////////////////////////////////////////////////////////////////////////////////
void vga::show()
{
    std::transform(m_ram.begin(), m_ram.end(), m_pixels.begin(),[&](auto i){ return m_palette.at(i); });

    constexpr int pitch = width * sizeof(color_t);
    SDL_UpdateTexture(m_texture, nullptr, m_pixels.data(), pitch);

    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

}   // retro
