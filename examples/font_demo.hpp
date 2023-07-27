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
        for(const auto c : std::views::iota(0, 256))
        {
            m_vga.putchar(static_cast<unsigned char>(c), 7);

            const auto col = c % 16;
            const auto row = c / 16;
            m_vga.set_cursor(col, row);
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
