////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef RETRO_EGA_PALETTE_HPP
#define RETRO_EGA_PALETTE_HPP

#include "retro/color.hpp"

#include <array>


////////////////////////////////////////////////////////////////////////////////
namespace
{

////////////////////////////////////////////////////////////////////////////////
const std::array<retro::color, 16> ega_palette
{
    retro::color(0, 0, 0),        // black
    retro::color(0, 0, 170),      // blue
    retro::color(0, 170, 0),      // green
    retro::color(0, 170, 170),    // cyan
    retro::color(170, 0, 0),      // red
    retro::color(170, 0, 170),    // magenta
    retro::color(170, 85, 0),     // brown
    retro::color(170, 170, 170),  // white
    retro::color(85, 85, 85),     // dark gray
    retro::color(85, 85, 255),    // bright blue
    retro::color(85, 255, 85),    // bright green
    retro::color(85, 255, 255),   // bright cyan
    retro::color(255, 85, 85),    // bright red
    retro::color(255, 85, 255),   // bright magenta
    retro::color(255, 255, 85),   // bright yellow
    retro::color(255, 255, 255)   // bright white
};

}   // unnamed


#endif  // RETRO_EGA_PALETTE_HPP
