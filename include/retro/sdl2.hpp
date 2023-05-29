////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#ifndef RETRO_SDL2_HPP
#define RETRO_SDL2_HPP

#include <SDL2/SDL.h>


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
class sdl2
{
public:
    enum class subsystem : Uint32
    {
        timer           = SDL_INIT_TIMER,
        audio           = SDL_INIT_AUDIO,
        video           = SDL_INIT_VIDEO,
        joystick        = SDL_INIT_JOYSTICK,
        haptic          = SDL_INIT_HAPTIC,
        game_controller = SDL_INIT_GAMECONTROLLER,
        events          = SDL_INIT_EVENTS,
        everything      = SDL_INIT_EVERYTHING,
        no_parachute    = SDL_INIT_NOPARACHUTE
    };

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Initialize the SDL2 library.
    /// \param flags subsystems initalization flags
    /// \throw std::runtime_error if initialization fails.
    ////////////////////////////////////////////////////////////////////////////
    explicit sdl2(subsystem flags);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Clean up all initialized subsystems.
    ////////////////////////////////////////////////////////////////////////////
    ~sdl2();

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Compatibility function to initialize the SDL library.
    /// \param flags subsystems initalization flags
    /// \throw std::runtime_error if initialization fails.
    ////////////////////////////////////////////////////////////////////////////
    void init_subsystem(subsystem flags) const;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get a mask of the specified subsystems currently initialized.
    /// \param flags subsystems initalization flags
    /// \return Returns a mask of all initialized subsystems if \a flags is 0,
    /// otherwise it returns the initialization status of the specified
    /// subsystems.
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] subsystem was_init(subsystem flags) const noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Shut down specific SDL subsystems.
    /// \param flags subsystems initalization flags
    ////////////////////////////////////////////////////////////////////////////
    void quit_subsystem(subsystem flags) const noexcept;

    sdl2() = delete;
    sdl2(const sdl2&) = delete;
    sdl2(sdl2&&) = delete;
    sdl2& operator=(const sdl2&) = delete;
    sdl2& operator=(sdl2&&) = delete;
};


////////////////////////////////////////////////////////////////////////////////
/// \brief Overload of bit-wise OR operator.
/// \param lhs left operand
/// \param rhs right operand
/// \return Result of \a lhs | \a rhs
////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr sdl2::subsystem operator|(sdl2::subsystem lhs, sdl2::subsystem rhs);


////////////////////////////////////////////////////////////////////////////////
/// \brief Overload of bit-wise AND operator.
/// \param lhs left operand
/// \param rhs right operand
/// \return Result of \a lhs & \a rhs
////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr sdl2::subsystem operator&(sdl2::subsystem lhs, sdl2::subsystem rhs);

}   // retro

#endif  // RETRO_SDL2_HPP
