////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/font.hpp"
#include "retro/types.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
font::font(const std::span<const std::byte> glyphs, const int width, const int height)
    : m_width(width), m_height(height)
{
    m_glyphs.resize(glyphs.size());
    std::copy(glyphs.cbegin(), glyphs.cend(), m_glyphs.begin());
}


////////////////////////////////////////////////////////////////////////////////
vram_t font::glyph(const unsigned char c, const pixel_t fg, const pixel_t bg) const
{
    const auto offset = static_cast<std::size_t>(c * m_height);
    const auto raw_glyph = std::span(m_glyphs).subspan(offset, m_height);

    vram_t new_glyph{};

    for(auto line : raw_glyph)
    {
        for(int i{0}; i != 8; ++i)
        {
            const auto p = ((std::to_integer<int>(line) & 0x80) == 0) ? bg : fg;
            new_glyph.emplace_back(p);
            line <<= 1;
        }

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
