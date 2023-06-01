////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef XOR_DEMO_HPP
#define XOR_DEMO_HPP

#include <retro/retro.hpp>
#include <SDL2/SDL_events.h>

#include <numeric>


////////////////////////////////////////////////////////////////////////////////
class xor_demo
{
  public:
    xor_demo() : m_sdl2(retro::sdl2::subsystem::video), m_vga(retro::vga::mode::vga_13h)
    {
        // generate palette
        retro::vga::palette_t palette(256);
        std::iota(palette.begin(), palette.end(), retro::color{});
        m_vga.set_palette(palette);

        // generate XOR pattern
        for(int y{0}; y != 200; ++y)
        {
            for(int x{0}; x != 320; ++x)
            {
                const auto pixel = static_cast<retro::vga::pixel_t>((x ^ y) & 255);
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
