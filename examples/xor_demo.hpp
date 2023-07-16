////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef XOR_DEMO_HPP
#define XOR_DEMO_HPP

#include "retro/types.hpp"
#include <retro/retro.hpp>
#include <SDL2/SDL_events.h>

#include <numeric>
#include <ranges>


////////////////////////////////////////////////////////////////////////////////
class xor_demo
{
  public:
    xor_demo() : m_sdl2(retro::sdl2::subsystem::video), m_vga(retro::vga::mode::vga_13h)
    {
        // generate palette
        retro::palette_t palette(256);
        std::iota(palette.begin(), palette.end(), retro::color{});
        m_vga.set_palette(palette);

        // generate XOR pattern
        for(const auto y : std::views::iota(0, 200))
        {
            for(const auto x : std::views::iota(0, 320))
            {
                const auto pixel = static_cast<retro::pixel_t>((x ^ y) & 255);
                m_vga.set_pixel(x, y, pixel);
            }
        }

        m_running = true;
    };

    ////////////////////////////////////////////////////////////////////////////
    void run()
    {
        while(m_running)
        {
            for(SDL_Event e; SDL_PollEvent(&e);)
            {
                if(e.type == SDL_QUIT)
                {
                    m_running = false;
                }
            }

            m_vga.show();
        }
    };

  private:
    bool m_running{false};

    retro::sdl2 m_sdl2;
    retro::vga m_vga;
};


#endif  // XOR_DEMO_HPP
