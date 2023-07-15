////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/sprite.hpp"
#include "retro/vga.hpp"

#include <SDL2/SDL_rect.h>

#include <algorithm>
#include <optional>
#include <span>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
sprite::sprite(const int width, const int height, const std::optional<std::span<const pixel_t>>& pixels)
    : m_width{width}, m_height{height}
{
    if(pixels.has_value())
    {
        const auto p = pixels.value();
        m_texture.resize(p.size());
        std::copy(p.begin(), p.end(), m_texture.begin());
    }
    else
    {
        m_texture.resize(static_cast<std::size_t>(width * height));
    }
}


////////////////////////////////////////////////////////////////////////////////
void sprite::move(const int dx, const int dy) noexcept
{
    m_x += dx;
    m_y += dy;
}


////////////////////////////////////////////////////////////////////////////////
void sprite::position(const int x, const int y) noexcept
{
    m_x = x;
    m_y = y;
}


////////////////////////////////////////////////////////////////////////////////
void sprite::position(const SDL_Point& position) noexcept
{
    m_x = position.x;
    m_y = position.y;
}


////////////////////////////////////////////////////////////////////////////////
SDL_Point sprite::position() const noexcept
{
    return SDL_Point{m_x, m_y};
}


////////////////////////////////////////////////////////////////////////////////
std::size_t sprite::size() const noexcept
{
    return m_texture.size();
}

}   // retro
