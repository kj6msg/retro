////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef RETRO_VGA_HPP
#define RETRO_VGA_HPP

#include <retro/font.hpp>

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
struct SDL_Point;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;


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
    enum class mode
    {
        vga_03h,
        ega_0dh,
        ega_0eh,
        ega_10h,
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
    void blit(std::span<const int> source);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Blit a sprite to the screen
    /// \param source source sprite
    ////////////////////////////////////////////////////////////////////////////
    void blit(const sprite& source);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Clear screen.
    /// \param index palette index
    ////////////////////////////////////////////////////////////////////////////
    void clear(int index);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get a color from the palette.
    /// \param index palette index (0-255)
    /// \return color
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] color get_color(int index) const;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get cursor position.
    /// \return cursor position (column, row)
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] std::pair<int, int> get_cursor() const noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get character font.
    /// \return character font
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] font get_font() const noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get color palette.
    /// \return color palette
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] std::vector<color> get_palette() const noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get pixel.
    /// \param x the x location of pixel
    /// \param y the y location of pixel
    /// \return indexed color
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] int get_pixel(int x, int y) const;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Print string.
    /// \param s string
    /// \param col column
    /// \param row row
    /// \param fg foreground color
    /// \param update_cursor if true, cursor position is updated
    ////////////////////////////////////////////////////////////////////////////
    void print(std::string_view s, int col, int row, int fg, bool update_cursor);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Write character at cursor position.
    /// \param c character
    /// \param fg foreground color
    ////////////////////////////////////////////////////////////////////////////
    void putchar(unsigned char c, int fg);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Reset to the default palette.
    ////////////////////////////////////////////////////////////////////////////
    void reset_palette();

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Scroll down window.
    /// \param lines number of lines to scroll down
    ////////////////////////////////////////////////////////////////////////////
    void scroll_down(int lines = 1);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Scroll up window.
    /// \param lines number of lines to scroll up
    ////////////////////////////////////////////////////////////////////////////
    void scroll_up(int lines = 1);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Set an indexed color in the palette.
    /// \param index palette index (0-255)
    /// \param color ARGB formatted color
    ////////////////////////////////////////////////////////////////////////////
    void set_color(int index, const color& c);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Set cursor position.
    /// \param col column
    /// \param row row
    ////////////////////////////////////////////////////////////////////////////
    void set_cursor(int col, int row);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Set character font.
    /// \param f character font
    ////////////////////////////////////////////////////////////////////////////
    void set_font(const font& f);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Set video mode.
    /// \param video_mode standard video mode
    ////////////////////////////////////////////////////////////////////////////
    void set_mode(mode video_mode);

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
    void set_pixel(int x, int y, int color_index);

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
    ////////////////////////////////////////////////////////////////////////////
    /// \brief Convert (x,y) coordinate to linear address.
    /// \param x x location
    /// \param y y location
    /// \return linear address of coordinate
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr std::size_t xy_to_index(int x, int y) const noexcept;

    int m_width{};
    int m_height{};
    int m_num_colors{};
    
    int m_columns{};
    int m_rows{};
    int m_cursor_col{};
    int m_cursor_row{};

    SDL_Window*   m_window{nullptr};
    SDL_Renderer* m_renderer{nullptr};
    SDL_Texture*  m_texture{nullptr};

    std::vector<int> m_vram;
    std::vector<color> m_palette;
    font m_font;

    std::vector<std::uint32_t> m_pixels;
};

}   // retro


#endif  // RETRO_VGA_HPP
