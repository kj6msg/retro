////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef VGA_TEST_HPP
#define VGA_TEST_HPP

#include <retro/retro.hpp>
#include <SDL2/SDL.h>


////////////////////////////////////////////////////////////////////////////////
class vga_test
{
  public:
    vga_test()
    {
        m_running = true;
    };

    ////////////////////////////////////////////////////////////////////////////
    void run()
    {
        // color bars using set_pixel
        int color{0};
        for(int y{0}; y != 200; ++y)
        {
            for(int x{0}; x < 320; x += 20)
            {
                for(int c{0}; c != 20; ++c)
                {
                    m_vga.set_pixel(x + c, y, color);
                }

                ++color;
            }

            color = 0;
        }

        // black box using blit
        std::vector<int> box(40 * 40, 0);
        m_vga.blit(box, SDL_Rect{40, 40, 40, 40});

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
