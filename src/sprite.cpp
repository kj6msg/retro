////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/sprite.hpp"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
sprite::sprite(const int width, const int height, const std::optional<std::span<const int>>& pixels)
    : m_width{width}, m_height{height}
{
    if(width < 1 || height < 1)
    {
        throw std::invalid_argument("sprite ctor has an invalid argument");
    }
    
    if(pixels.has_value())
    {
        const auto p = pixels.value();
        m_texture.resize(p.size());
        std::ranges::copy(p, m_texture.begin());
    }
    else
    {
        m_texture.resize(static_cast<std::size_t>(width * height));
    }
}


////////////////////////////////////////////////////////////////////////////////
void sprite::fill(const int color)
{
    if(color < 0)
    {
        throw std::invalid_argument("sprite::fill has an invalid argument");
    }
    
    std::ranges::fill(m_texture, color);
}


////////////////////////////////////////////////////////////////////////////////
void sprite::move(const int dx, const int dy) noexcept
{
    m_x += dx;
    m_y += dy;
}


////////////////////////////////////////////////////////////////////////////////
const std::vector<int>& sprite::pixels() const noexcept
{
    return m_texture;
}


////////////////////////////////////////////////////////////////////////////////
void sprite::position(const int x, const int y) noexcept
{
    m_x = x;
    m_y = y;
}


////////////////////////////////////////////////////////////////////////////////
std::pair<int, int> sprite::position() const noexcept
{
    return std::make_pair(m_x, m_y);
}


////////////////////////////////////////////////////////////////////////////////
std::pair<int, int> sprite::size() const noexcept
{
    return std::make_pair(m_width, m_height);
}

}   // retro
