////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef FIRE_HPP
#define FIRE_HPP

#include <retro/retro.hpp>
#include <SDL2/SDL.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
class fire
{
  public:
    fire() : m_sdl2(retro::sdl2::subsystem::video), m_vga(retro::vga::mode::vga_13h)
    {
        // generate palette
        std::vector<retro::vga::color_t> colors(256);

        // black (0, 0, 0) to red (128, 0, 0)
        auto it = colors.begin();
        it = std::for_each_n(it, 33, [r = int(0)](auto &c) mutable
        {
            c = retro::make_color(r, 0, 0);
            r += 4;
        });

        // red (128, 0, 0) to orange (192, 128, 0)
        it = std::for_each_n(it, 32, [r = int(128), g = int(0)](auto &c) mutable
        {
            c = retro::make_color(r, g, 0);
            r += 2;
            g += 4;
        });

        // orange (192, 128, 0) to yellow (192, 190, 0)
        it = std::for_each_n(it, 31, [g = int(128)](auto &c) mutable
        {
            c = retro::make_color(192, g, 0);
            g += 2;
        });

        // yellow (192, 192, 0) to white (255, 255, 255)
        // Linear interpolation: c = c0 + (c1 - c0) * (i - i0) / (i1 - i0)
        //      red   = 192 + (255 - 192) * (i - 96) / (255 - 96)
        //      green = 192 + (255 - 192) * (i - 96) / (255 - 96)
        //      blue  = 0 + (255 - 0) * (i - 96) / (255 - 96)
        std::for_each_n(it, 160, [n = int(96)](auto &c) mutable
        {
            const auto t = (static_cast<float>(n) - 96.0f) / (255.0f - 96.0f);
            const auto r = static_cast<int>(std::lerp(192.0f, 255.0f, t));
            const auto g = static_cast<int>(std::lerp(192.0f, 255.0f, t));
            const auto b = static_cast<int>(std::lerp(0.0f, 255.0f, t));

            c = retro::make_color(r, g, b);
            ++n;
        });

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
        std::vector<int> img(width * height);

        while(m_running)
        {
            for(SDL_Event e; SDL_PollEvent(&e);)
            {
                if(e.type == SDL_QUIT)
                {
                    m_running = false;
                }
            }

            // generate hot spots on bottom row of pixels
            std::generate(img.end() - width, img.end(), [&]()
            {
                return dist(engine);
            });

            // generate fire
            for(int y{0}; y != height - 1; ++y)
            {
                for(int x{0}; x != width; ++x)
                {
                    // Left below, right below, center below, and center two below
                    // pixels. The divisor controls the height of the fire.
                    const auto left    = static_cast<std::size_t>(((x - 1 + width) % width) + width * (y + 1));
                    const auto center  = static_cast<std::size_t>(x + width * (y + 1));
                    const auto right   = static_cast<std::size_t>(((x + 1) % width) + width * (y + 1));
                    const auto twodown = static_cast<std::size_t>(x + width * ((y + 2) % height));

                    auto new_color{static_cast<float>(img[left])};
                    new_color += static_cast<float>(img[center]);
                    new_color += static_cast<float>(img[right]);
                    new_color += static_cast<float>(img[twodown]);
                    new_color /= 4.03f;

                    const auto i = static_cast<std::size_t>(x + width * y);
                    img[i] = static_cast<int>(new_color);
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
