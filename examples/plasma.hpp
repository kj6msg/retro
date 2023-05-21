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
    plasma() : m_colors(256)
    {
        // generate palette
        auto it = std::for_each_n(m_colors.begin(), 128, [n = int(0)](auto &c) mutable
        {
            c = retro::make_color(n, n, n);
            n += 2;
        });
        std::reverse_copy(m_colors.begin(), it, it);
        m_vga.set_palette(m_colors);

        // generate plasma
        constexpr int width{320};
        constexpr int height{200};
        std::vector<int> img(width * height);

        for(int y{0}; y != height; ++y)
        {
            for(int x{0}; x != width; ++x)
            {
                const auto color = (std::cosf(static_cast<float>(x) * 0.1f) +
                                    std::sinf(static_cast<float>(y) * 0.1f)) *
                                    63.5f + 128.0f;

                const auto i = static_cast<std::size_t>(x + width * y);
                img[i] = static_cast<int>(color);
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

            std::rotate(m_colors.begin(), m_colors.begin() + 1, m_colors.end());
            m_vga.set_palette(m_colors);
            m_vga.show();
        }
    };

  private:
    bool m_running{false};

    std::vector<retro::vga::color_t> m_colors;

    retro::sdl2 m_sdl2{retro::sdl2::subsystem::video};
    retro::vga m_vga;
};

#endif  // PLASMA_HPP
