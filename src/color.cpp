////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/color.hpp"
#include "retro/types.hpp"


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
color::color() noexcept
{
}


////////////////////////////////////////////////////////////////////////////////
color::color(const color_channel_t r, const color_channel_t g, const color_channel_t b) noexcept : m_r(r), m_g(g), m_b(b)
{
}


////////////////////////////////////////////////////////////////////////////////
color::color(const argb_t argb) noexcept
{
    m_r = static_cast<color_channel_t>((argb & 0xff0000u) >> 16u);
    m_g = static_cast<color_channel_t>((argb & 0xff00u) >> 8u);
    m_b = static_cast<color_channel_t>(argb & 0xffu);
}


////////////////////////////////////////////////////////////////////////////////
argb_t color::to_argb() const noexcept
{
    return make_argb(m_r, m_g, m_b);
}


////////////////////////////////////////////////////////////////////////////////
color color::operator+(const color& b) const noexcept
{
    color temp;

    temp.m_r = m_r + b.m_r;
    temp.m_g = m_g + b.m_g;
    temp.m_b = m_b + b.m_b;

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator+(const color_channel_t b) const noexcept
{
    color temp;

    temp.m_r = m_r + b;
    temp.m_g = m_g + b;
    temp.m_b = m_b + b;

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator-(const color& b) const noexcept
{
    color temp;

    temp.m_r = m_r - b.m_r;
    temp.m_g = m_g - b.m_g;
    temp.m_b = m_b - b.m_b;

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator-(const color_channel_t b) const noexcept
{
    color temp;

    temp.m_r = m_r - b;
    temp.m_g = m_g - b;
    temp.m_b = m_b - b;

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator*(const color& b) const noexcept
{
    color temp;

    temp.m_r = m_r * b.m_r;
    temp.m_g = m_g * b.m_g;
    temp.m_b = m_b * b.m_b;

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator*(const color_channel_t b) const noexcept
{
    color temp;

    temp.m_r = m_r * b;
    temp.m_g = m_g * b;
    temp.m_b = m_b * b;

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator+=(const color& b) noexcept
{
    m_r += b.m_r;
    m_g += b.m_g;
    m_b += b.m_b;

    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator+=(const color_channel_t b) noexcept
{
    m_r += b;
    m_g += b;
    m_b += b;

    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator-=(const color& b) noexcept
{
    m_r -= b.m_r;
    m_g -= b.m_g;
    m_b -= b.m_b;

    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator-=(const color_channel_t b) noexcept
{
    m_r -= b;
    m_g -= b;
    m_b -= b;

    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator++() noexcept
{
    ++m_r;
    ++m_g;
    ++m_b;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator++(int) noexcept
{
    color temp = *this;
    ++*this;
    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator--() noexcept
{
    --m_r;
    --m_g;
    --m_b;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator--(int) noexcept
{
    color temp = *this;
    --*this;
    return temp;
}

}   // retro
