////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef PLASMA_HPP
#define PLASMA_HPP

#include <retro/retro.hpp>
#include <SDL2/SDL.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
class plasma
{
  public:
    plasma() : m_sdl2(retro::sdl2::subsystem::video), m_vga(retro::vga::mode::vga_13h), m_palette(256)
    {
        // generate palette
        auto it = std::for_each_n(m_palette.begin(), 128, [c = retro::color()](auto &color) mutable
        {
            color = c;
            c += 2;
        });
        std::reverse_copy(m_palette.begin(), it, it);
        m_vga.set_palette(m_palette);

        // generate plasma
        constexpr int width{320};
        constexpr int height{200};
        retro::vga::vram_t img(width * height);

        for(int y{0}; y != height; ++y)
        {
            for(int x{0}; x != width; ++x)
            {
                const auto pixel = (std::cosf(static_cast<float>(x) * 0.1f) +
                                    std::sinf(static_cast<float>(y) * 0.1f)) *
                                    63.5f + 128.0f;

                const auto i = static_cast<std::size_t>(x + width * y);
                img[i] = static_cast<retro::vga::pixel_t>(pixel);
            }
        }

        m_vga.blit(img);

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

            std::rotate(m_palette.begin(), m_palette.begin() + 1, m_palette.end());
            m_vga.set_palette(m_palette);
            m_vga.show();
        }
    };

  private:
    bool m_running{false};

    retro::sdl2 m_sdl2;
    retro::vga m_vga;
    retro::vga::palette_t m_palette;
};

#endif  // PLASMA_HPP
