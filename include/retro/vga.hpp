////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef VGA_HPP
#define VGA_HPP

#include <cstdint>
#include <span>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;
struct SDL_Point;
struct SDL_Texture;


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
class color;
class sprite;


////////////////////////////////////////////////////////////////////////////////
class vga
{
  public:
    using palette_t = std::vector<color>;
    using pixel_t = std::uint8_t;
    using vram_t = std::vector<pixel_t>;

    enum class mode
    {
        vga_03h,
        ega_0dh,
        ega_0eh,
        vga_12h,
        vga_13h
    };

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Create a VGA device.
    /// \param video_mode standard video mode
    ////////////////////////////////////////////////////////////////////////////
    explicit vga(mode video_mode);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Destroy the associated window, renderer, and texture.
    ////////////////////////////////////////////////////////////////////////////
    ~vga();

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Blit a full size indexed image to the screen.
    /// \param source indexed pixels
    ////////////////////////////////////////////////////////////////////////////
    void blit(std::span<const pixel_t> source);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Blit a sprite to the screen
    /// \param source source sprite
    ////////////////////////////////////////////////////////////////////////////
    void blit(const sprite& source);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get a color from the palette.
    /// \param index palette index (0-255)
    /// \return color
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] color get_color(int index) const;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Reset to the default palette.
    ////////////////////////////////////////////////////////////////////////////
    void reset_palette();

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Set an indexed color in the palette.
    /// \param index palette index (0-255)
    /// \param color ARGB formatted color
    ////////////////////////////////////////////////////////////////////////////
    void set_color(int index, const color& c);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Set the entire color palette.
    /// \param colors vector of 256 ARGB formatted colors
    ////////////////////////////////////////////////////////////////////////////
    void set_palette(std::span<const color> colors);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Set a pixel to an indexed color.
    /// \param x the x location of the pixel
    /// \param y the y location of the pixel
    /// \param color_index indexed color (0-255)
    ////////////////////////////////////////////////////////////////////////////
    void set_pixel(int x, int y, pixel_t color_index);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Set a pixel to an indexed color.
    /// \param position position of the pixel
    /// \param color_index indexed color (0-255)
    ////////////////////////////////////////////////////////////////////////////
    void set_pixel(const SDL_Point& position, pixel_t color_index);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Show the screen.
    ////////////////////////////////////////////////////////////////////////////
    void show();

    vga() = delete;
    vga(const vga&) = delete;
    vga(vga&&) = delete;
    vga& operator=(const vga&) = delete;
    vga& operator=(vga&&) = delete;

  private:
    int m_width{};
    int m_height{};
    int m_num_colors{};

    SDL_Window*   m_window{nullptr};
    SDL_Renderer* m_renderer{nullptr};
    SDL_Texture*  m_texture{nullptr};

    vram_t m_ram;
    palette_t m_palette;
};

}   // retro


#endif  // VGA_HPP
