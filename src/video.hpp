////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "retro/color.hpp"
#include "retro/vga.hpp"

#include <array>
#include <map>


////////////////////////////////////////////////////////////////////////////////
namespace retro
{
namespace video
{

////////////////////////////////////////////////////////////////////////////////
static const std::array<color, 16> ega_palette
{
    color(0, 0, 0),        // black
    color(0, 0, 170),      // blue
    color(0, 170, 0),      // green
    color(0, 170, 170),    // cyan
    color(170, 0, 0),      // red
    color(170, 0, 170),    // magenta
    color(170, 85, 0),     // brown
    color(170, 170, 170),  // white
    color(85, 85, 85),     // dark gray
    color(85, 85, 255),    // bright blue
    color(85, 255, 85),    // bright green
    color(85, 255, 255),   // bright cyan
    color(255, 85, 85),    // bright red
    color(255, 85, 255),   // bright magenta
    color(255, 255, 85),   // bright yellow
    color(255, 255, 255)   // bright white
};


////////////////////////////////////////////////////////////////////////////////
struct mode
{
    enum class type
    {
        text,
        graphics
    };

    int width{};
    int height{};
    int num_colors{};
    type type;
};


////////////////////////////////////////////////////////////////////////////////
static constexpr mode vga_03h{720, 400, 16, mode::type::text};
static constexpr mode ega_0dh{320, 200, 16, mode::type::graphics};
static constexpr mode ega_0eh{640, 200, 16, mode::type::graphics};
static constexpr mode vga_12h{640, 480, 16, mode::type::graphics};
static constexpr mode vga_13h{320, 200, 256, mode::type::graphics};

static const std::map<vga::mode, mode> modes
{
    {vga::mode::vga_03h, vga_03h},
    {vga::mode::ega_0dh, ega_0dh},
    {vga::mode::ega_0eh, ega_0eh},
    {vga::mode::vga_12h, vga_12h},
    {vga::mode::vga_13h, vga_13h}
};

}   // video
}   // retro


#endif  // VIDEO_HPP
