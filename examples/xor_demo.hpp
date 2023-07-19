////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef XOR_DEMO_HPP
#define XOR_DEMO_HPP

#include <retro/retro.hpp>
#include <SDL2/SDL_events.h>

#include <array>
#include <numeric>
#include <ranges>


////////////////////////////////////////////////////////////////////////////////
class xor_demo
{
  public:
    xor_demo()
    {
        // generate palette
        std::array<retro::color, 256> palette;
        std::iota(palette.begin(), palette.end(), retro::color{});
        m_vga.set_palette(palette);

        // generate XOR pattern
        constexpr int width{320};
        constexpr int height{200};

        for(const auto y : std::views::iota(0, height))
        {
            for(const auto x : std::views::iota(0, width))
            {
                const auto color_index  = static_cast<int>((x ^ y) & 255);
                m_vga.set_pixel(x, y, color_index);
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


#endif  // XOR_DEMO_HPP
