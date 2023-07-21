////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/font.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <span>
#include <utility>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
font::font(const std::span<const std::byte> glyphs, const int width, const int height)
    : m_width{width}, m_height{height}, m_glyphs(glyphs.size())
{
    std::ranges::copy(glyphs, m_glyphs.begin());
}


////////////////////////////////////////////////////////////////////////////////
std::vector<int> font::glyph(const unsigned char c, const int fg, const int bg) const
{
    const auto glyph = m_glyphs | std::views::drop(m_height * c) | std::views::take(m_height);
    std::vector<int> color_glyph;

    for(auto line : glyph)
    {
        std::for_each_n(std::back_inserter(color_glyph), 8, [=, &line](auto& color)
        {
            color = ((std::to_integer<std::uint8_t>(line) & 0x80u) == 0u) ? bg : fg;
            line <<= 1;
        });

        if(m_width == 9)
        {
            color_glyph.emplace_back(bg);
        }
    }

    return color_glyph;
}


////////////////////////////////////////////////////////////////////////////////
std::pair<int, int> font::size() const noexcept
{
    return std::make_pair(m_width, m_height);
}

}   // retro
