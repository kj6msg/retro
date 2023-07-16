////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/font.hpp"
#include "retro/types.hpp"

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
vram_t font::glyph(const unsigned char c, const pixel_t fg, const pixel_t bg) const
{
    const auto glyph = std::next(m_glyphs.begin(), m_height * c);
    vram_t new_glyph;

    for(auto line : std::views::counted(glyph, m_height))
    {
        std::for_each_n(std::back_inserter(new_glyph), 8, [=, &line](auto& p)
        {
            p = ((std::to_integer<int>(line) & 0x80) == 0) ? bg : fg;
            line <<= 1;
        });

        if(m_width == 9)
        {
            new_glyph.emplace_back(bg);
        }
    }

    return new_glyph;
}


////////////////////////////////////////////////////////////////////////////////
std::pair<int, int> font::size() const noexcept
{
    return std::make_pair(m_width, m_height);
}

}   // retro
