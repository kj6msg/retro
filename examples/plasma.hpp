////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef PLASMA_HPP
#define PLASMA_HPP

#include <retro/retro.hpp>
#include <SDL2/SDL.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <numbers>
#include <ranges>


////////////////////////////////////////////////////////////////////////////////
class plasma
{
  public:
    plasma() : m_sdl2(retro::sdl2::subsystem::video), m_vga(retro::vga::mode::vga_13h)
    {
        // generate palette - sinusoidal black to white to black
        for(auto n{0.0}; auto &color : m_palette)
        {
            constexpr auto a{256.0};
            constexpr auto b = std::numbers::pi / 255.0;
            const auto rgb = static_cast<int>(a * std::sin(b * n));
            color = retro::color(rgb, rgb, rgb);
            n += 1.0;
        }
        m_vga.set_palette(m_palette);

        // generate plasma
        constexpr int width{320};
        constexpr int height{200};
        std::array<int, width * height> img;

        for(const auto y : std::views::iota(0, height))
        {
            for(const auto x : std::views::iota(0, width))
            {
                const auto pixel = (std::cos(static_cast<double>(x) * 0.1) +
                                    std::sin(static_cast<double>(y) * 0.1)) *
                                    63.5 + 128.0;

                const auto i = static_cast<std::size_t>(x + width * y);
                img[i] = static_cast<int>(pixel);
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

            std::ranges::rotate(m_palette, m_palette.begin() + 1);
            m_vga.set_palette(m_palette);
            m_vga.show();
        }
    };

  private:
    bool m_running{false};

    retro::sdl2 m_sdl2;
    retro::vga m_vga;
    std::array<retro::color, 256> m_palette;
};

#endif  // PLASMA_HPP
