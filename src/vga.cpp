////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/vga.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <stdexcept>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
static constexpr int width{320};
static constexpr int height{200};


////////////////////////////////////////////////////////////////////////////////
vga::vga() : m_pixels(width * height), m_palette(256)
{
    SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &m_window, &m_renderer);

    if(m_window == nullptr || m_renderer == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING, width, height);

    if(m_texture == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(m_renderer, width, height);

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);
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
    std::transform(source.begin(), source.end(), m_pixels.begin(),[&](auto i){ return m_palette.at(i); });
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
    const auto index = x + width * y;
    m_pixels.at(index) = m_palette.at(color_index);
}


////////////////////////////////////////////////////////////////////////////////
void vga::show() noexcept
{
    constexpr int pitch = width * sizeof(color_t);
    SDL_UpdateTexture(m_texture, nullptr, m_pixels.data(), pitch);

    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

}   // retro
