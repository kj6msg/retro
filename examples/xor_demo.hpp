////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef XOR_DEMO_HPP
#define XOR_DEMO_HPP

#include <retro/retro.hpp>
#include <SDL2/SDL_events.h>

#include <vector>


////////////////////////////////////////////////////////////////////////////////
class xor_demo
{
  public:
    xor_demo() : m_sdl2(retro::sdl2::subsystem::video), m_vga(retro::vga::mode::vga_13h)
    {
        m_running = true;
    };

    ////////////////////////////////////////////////////////////////////////////
    void run()
    {
        // generate palette
        retro::vga::palette_t palette(256);

        for(retro::color c; auto &color : palette)
        {
            color = c;
            ++c;
        }

        m_vga.set_palette(palette);

        // generate XOR pattern
        for(int y{0}; y != 200; ++y)
        {
            for(int x{0}; x != 320; ++x)
            {
                m_vga.set_pixel(x, y, (x ^ y) & 255);
            }
        }

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
