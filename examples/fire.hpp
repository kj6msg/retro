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


////////////////////////////////////////////////////////////////////////////////
class fire
{
  public:
    fire()
    {
        // generate palette
        std::array<retro::color, 256> palette;

        // 33 colors: black (0, 0, 0) to red (128, 0, 0)
        for(retro::color c; auto& p : palette | std::views::take(33))
        {
            p = c;
            c += retro::color{4, 0, 0};
        }

        // 32 colors: red (128, 0, 0) to orange (192, 128, 0)
        for(retro::color c(128, 0, 0); auto& p : palette | std::views::drop(33) | std::views::take(32))
        {
            p = c;
            c += retro::color{2, 4, 0};
        }

        // 31 colors: orange (192, 128, 0) to yellow (192, 190, 0)
        for(retro::color c(192, 128, 0); auto& p : palette | std::views::drop(65) | std::views::take(31))
        {
            p = c;
            c += retro::color{0, 2, 0};
        }

        // 160 colors: yellow (192, 192, 0) to white (255, 255, 255)
        // Linear interpolation: c = c0 + (c1 - c0) * (i - i0) / (i1 - i0)
        //      red   = 192 + (255 - 192) * (i - 96) / (255 - 96)
        //      green = 192 + (255 - 192) * (i - 96) / (255 - 96)
        //      blue  = 0 + (255 - 0) * (i - 96) / (255 - 96)
        for(int n{96}; auto& color : palette | std::views::drop(96))
        {
            const auto t = (static_cast<float>(n) - 96.0f) / (255.0f - 96.0f);
            const auto r = static_cast<int>(std::lerp(192.0f, 255.0f, t));
            const auto g = static_cast<int>(std::lerp(192.0f, 255.0f, t));
            const auto b = static_cast<int>(std::lerp(0.0f, 255.0f, t));

            color = retro::color(r, g, b);
            ++n;
        }

        m_vga.set_palette(palette);

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

            // generate hot spots on bottom pixels
            for(auto& pixel : img | std::views::reverse | std::views::take(width))
            {
                pixel = dist(engine);
            }

            // generate fire
            for(int i{}; auto& pixel : img | std::views::take(std::ssize(img) - width))
            {
                const auto x = i % width;
                const auto y = i / width;
                
                // Left below, right below, center below, and center two below
                // pixels. The divisor controls the height of the fire.
                const auto left    = static_cast<std::size_t>(((x - 1 + width) % width) + width * (y + 1));
                const auto center  = static_cast<std::size_t>(x + width * (y + 1));
                const auto right   = static_cast<std::size_t>(((x + 1) % width) + width * (y + 1));
                const auto twodown = static_cast<std::size_t>(x + width * ((y + 2) % height));

                auto color_index{static_cast<double>(img[left])};
                color_index += static_cast<double>(img[center]);
                color_index += static_cast<double>(img[right]);
                color_index += static_cast<double>(img[twodown]);
                color_index /= 4.03;

                pixel = static_cast<int>(color_index);
                ++i;
            }

            m_vga.blit(img);
            m_vga.show();
        }
    };

  private:
    bool m_running{false};

    retro::sdl2 m_sdl2{retro::sdl2::subsystem::video};
    retro::vga m_vga{retro::vga::mode::vga_13h};
};

#endif  // FIRE_HPP
