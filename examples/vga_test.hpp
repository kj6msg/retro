////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef VGA_TEST_HPP
#define VGA_TEST_HPP

#include <retro/retro.hpp>
#include <SDL2/SDL.h>

#include <vector>


////////////////////////////////////////////////////////////////////////////////
class vga_test
{
  public:
    vga_test()
    {
        std::vector<retro::vga::color_t> colors(256);

        // black to white palette
        for(int i{0}; auto &c : colors)
        {
            c = retro::make_color(i, i, i);
            ++i;
        }

        m_vga.set_palette(colors);

        m_running = true;
    };

    ////////////////////////////////////////////////////////////////////////////
    void run()
    {
        std::vector<int> pixels(320 * 200);

        // XOR pattern
        for(int y{0}; y != 200; ++y)
        {
            for(int x{0}; x != 320; ++x)
            {
                const auto addr = x + y * 320;
                pixels[addr] = (x ^ y) & 0xff;
            }
        }

        m_vga.blit(pixels);

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

    retro::sdl2 m_sdl2{retro::sdl2::subsystem::video};
    retro::vga m_vga;
};


#endif  // VGA_TEST_HPP
