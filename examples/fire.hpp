////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef FIRE_HPP
#define FIRE_HPP

#include <retro/retro.hpp>
#include <SDL2/SDL.h>

#include <array>
#include <cmath>
#include <cstddef>
#include <random>
#include <ranges>
#include <span>


////////////////////////////////////////////////////////////////////////////////
class fire
{
  public:
    fire() : m_sdl2(retro::sdl2::subsystem::video), m_vga(retro::vga::mode::vga_13h)
    {
        // generate palette
        std::array<retro::color, 256> palette;
        const std::span<retro::color> colors{palette};

        // 33 colors: black (0, 0, 0) to red (128, 0, 0)
        for(retro::color c, inc(4, 0, 0); auto& color : colors.subspan(0, 33))
        {
            color = c;
            c += inc;
        }

        // 32 colors: red (128, 0, 0) to orange (192, 128, 0)
        for(retro::color c(128, 0, 0), inc(2, 4, 0); auto& color : colors.subspan(33, 32))
        {
            color = c;
            c += inc;
        }

        // 31 colors: orange (192, 128, 0) to yellow (192, 190, 0)
        for(retro::color c(192, 128, 0), inc(0, 2, 0); auto& color : colors.subspan(65, 31))
        {
            color = c;
            c += inc;
        }

        // 160 colors: yellow (192, 192, 0) to white (255, 255, 255)
        // Linear interpolation: c = c0 + (c1 - c0) * (i - i0) / (i1 - i0)
        //      red   = 192 + (255 - 192) * (i - 96) / (255 - 96)
        //      green = 192 + (255 - 192) * (i - 96) / (255 - 96)
        //      blue  = 0 + (255 - 0) * (i - 96) / (255 - 96)
        for(int n{96}; auto& color : colors.subspan(96, 160))
        {
            const auto t = (static_cast<float>(n) - 96.0f) / (255.0f - 96.0f);
            const auto r = static_cast<int>(std::lerp(192.0f, 255.0f, t));
            const auto g = static_cast<int>(std::lerp(192.0f, 255.0f, t));
            const auto b = static_cast<int>(std::lerp(0.0f, 255.0f, t));

            color = retro::color(r, g, b);
            ++n;
        }

        m_vga.set_palette(colors);

        m_running = true;
    };

    ////////////////////////////////////////////////////////////////////////////
    void run()
    {
        std::default_random_engine engine(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, 255);

        constexpr int width{320};
        constexpr int height{200};
        std::array<int, width * height> img;
        img.fill(0);

        while(m_running)
        {
            for(SDL_Event e; SDL_PollEvent(&e);)
            {
                if(e.type == SDL_QUIT)
                {
                    m_running = false;
                }
            }

            for(auto& i : img | std::views::reverse | std::views::take(width))
            {
                i = dist(engine);
            }

            // generate fire
            for(const auto y : std::views::iota(0, height - 1))
            {
                for(const auto x : std::views::iota(0, width))
                {
                    // Left below, right below, center below, and center two below
                    // pixels. The divisor controls the height of the fire.
                    const auto left    = static_cast<std::size_t>(((x - 1 + width) % width) + width * (y + 1));
                    const auto center  = static_cast<std::size_t>(x + width * (y + 1));
                    const auto right   = static_cast<std::size_t>(((x + 1) % width) + width * (y + 1));
                    const auto twodown = static_cast<std::size_t>(x + width * ((y + 2) % height));

                    auto pixel{static_cast<float>(img[left])};
                    pixel += static_cast<float>(img[center]);
                    pixel += static_cast<float>(img[right]);
                    pixel += static_cast<float>(img[twodown]);
                    pixel /= 4.03f;

                    const auto i = static_cast<std::size_t>(x + width * y);
                    img[i] = static_cast<int>(pixel);
                }
            }

            m_vga.blit(img);
            m_vga.show();
        }
    };

  private:
    bool m_running{false};

    retro::sdl2 m_sdl2;
    retro::vga m_vga;
};

#endif  // FIRE_HPP
