////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef FONT_DEMO_HPP
#define FONT_DEMO_HPP

#include <retro/retro.hpp>
#include <SDL2/SDL.h>

#include <ranges>


////////////////////////////////////////////////////////////////////////////////
class font_demo
{
  public:
    font_demo()
    {
        for(int x{}, y{}; const auto c : std::views::iota(0, 256))
        {
            m_vga.putchar(static_cast<unsigned char>(c), 7, 0, x, y);

            constexpr int font_width{8};
            constexpr int font_height{8};
            constexpr int columns{16};

            x += font_width;

            if(x == font_width * columns)
            {
                x = 0;
                y += font_height;
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

    retro::sdl2 m_sdl2{retro::sdl2::subsystem::video};
    retro::vga m_vga{retro::vga::mode::vga_13h};
};

#endif  // FONT_DEMO_HPP
