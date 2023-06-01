////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef RETRO_SPRITE_HPP
#define RETRO_SPRITE_HPP

#include <retro/vga.hpp>

#include <optional>
#include <span>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
struct SDL_Point;


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
class sprite
{
  public:
    friend class vga;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Create a sprite.
    /// \param width width in pixels
    /// \param height height in pixels
    /// \param pixels pixel data [optional]
    ////////////////////////////////////////////////////////////////////////////
    sprite(int width, int height, const std::optional<std::span<const vga::pixel_t>>& pixels = std::nullopt);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor.
    /// \param copy instance to copy
    ////////////////////////////////////////////////////////////////////////////
    sprite(const sprite& copy) = default;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator.
    /// \param right instance to assign
    ////////////////////////////////////////////////////////////////////////////
    sprite& operator=(const sprite& right) = default;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor.
    /// \param other instance to move
    ////////////////////////////////////////////////////////////////////////////
    sprite(sprite&& other) = default;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator.
    /// \param other instance to move
    ////////////////////////////////////////////////////////////////////////////
    sprite& operator=(sprite&& other) = default;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Move sprite by a given offset.
    /// \param dx x offset
    /// \param dy y offset
    ////////////////////////////////////////////////////////////////////////////
    void move(int dx, int dy) noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Set sprite position.
    /// \param x x coordinate
    /// \param y y coordinate
    ////////////////////////////////////////////////////////////////////////////
    void position(int x, int y) noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Set sprite position.
    /// \param position (x,y) coordinate
    ////////////////////////////////////////////////////////////////////////////
    void position(const SDL_Point& position) noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get sprite position
    /// \return (x,y) coordinate
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] SDL_Point position() const noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get sprite size
    /// \return sprite size in pixels
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] std::size_t size() const noexcept;

    sprite() = delete;

  private:
    int m_width{};
    int m_height{};
    int m_x{};
    int m_y{};

    std::vector<vga::pixel_t> m_texture;
};

}   // retro


#endif  // RETRO_SPRITE_HPP
