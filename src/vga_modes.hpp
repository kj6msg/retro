////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef RETRO_VGA_MODES_HPP
#define RETRO_VGA_MODES_HPP

#include "glyphs.hpp"

#include "retro/font.hpp"
#include "retro/vga.hpp"

#include <map>


////////////////////////////////////////////////////////////////////////////////
namespace
{

////////////////////////////////////////////////////////////////////////////////
struct vga_mode
{
    int width{};
    int height{};
    int num_colors{};
    const retro::font& font;
};

const retro::font vga_8x8{glyphs_8x8, 8, 8};
const retro::font ega_8x14{glyphs_8x14, 8, 14};
const retro::font vga_8x16{glyphs_8x16, 8, 16};
const retro::font vga_9x16{glyphs_8x16, 9, 16};

constexpr vga_mode vga_03h{720, 400, 16, vga_9x16};
constexpr vga_mode ega_0dh{320, 200, 16, vga_8x8};
constexpr vga_mode ega_0eh{640, 200, 16, vga_8x8};
constexpr vga_mode ega_10h{640, 350, 16, ega_8x14};
constexpr vga_mode vga_12h{640, 480, 16, vga_8x16};
constexpr vga_mode vga_13h{320, 200, 256, vga_8x8};

const std::map<retro::vga::mode, const vga_mode&> vga_modes
{
    {retro::vga::mode::vga_03h, vga_03h},
    {retro::vga::mode::ega_0dh, ega_0dh},
    {retro::vga::mode::ega_0eh, ega_0eh},
    {retro::vga::mode::ega_10h, ega_10h},
    {retro::vga::mode::vga_12h, vga_12h},
    {retro::vga::mode::vga_13h, vga_13h}
};

}   // unnamed


#endif  // RETRO_VGA_MODES_HPP
