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
    retro::color::black,
    retro::color::blue,
    retro::color::green,
    retro::color::cyan,
    retro::color::red,
    retro::color::magenta,
    retro::color::brown,
    retro::color::white,
    retro::color::dark_gray,
    retro::color::bright_blue,
    retro::color::bright_green,
    retro::color::bright_cyan,
    retro::color::bright_red,
    retro::color::bright_magenta,
    retro::color::bright_yellow,
    retro::color::bright_white,
};

}   // unnamed


#endif  // RETRO_EGA_PALETTE_HPP
