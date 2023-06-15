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

    vram_t new_glyph(m_width * m_height);

    // Each line of a glyph is represented by 8-bits. Generate a pixel_t for
    // each bit.
    for(auto it = new_glyph.begin(); auto line : raw_glyph)
    {
        it = std::for_each_n(it, 8, [=, &line](auto& g)
        {
            g = ((line & std::byte{0b1000'0000}) == std::byte{0}) ? bg : fg;
            line <<= 1;
        });
    }

    return new_glyph;
}

}   // retro
