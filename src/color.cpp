////////////////////////////////////////////////////////////////////////////////
// Retro - Retro Computing Library
// Copyright (c) 2023 Ryan Clarke
////////////////////////////////////////////////////////////////////////////////

#include "retro/color.hpp"

#include <algorithm>
#include <cstdint>


////////////////////////////////////////////////////////////////////////////////
namespace retro
{

////////////////////////////////////////////////////////////////////////////////
color color::operator+(const color& b) const noexcept
{
    color temp;

    temp.m_r = std::min(m_r + b.m_r, UINT8_MAX);
    temp.m_g = std::min(m_g + b.m_g, UINT8_MAX);
    temp.m_b = std::min(m_b + b.m_b, UINT8_MAX);

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator+(const int b) const noexcept
{
    color temp;

    temp.m_r = std::clamp(m_r + b, 0, UINT8_MAX);
    temp.m_g = std::clamp(m_g + b, 0, UINT8_MAX);
    temp.m_b = std::clamp(m_b + b, 0, UINT8_MAX);

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator-(const color& b) const noexcept
{
    color temp;

    temp.m_r = std::max(0, m_r - b.m_r);
    temp.m_g = std::max(0, m_g - b.m_g);
    temp.m_b = std::max(0, m_b - b.m_b);

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator-(const int b) const noexcept
{
    color temp;

    temp.m_r = std::clamp(m_r - b, 0, UINT8_MAX);
    temp.m_g = std::clamp(m_g - b, 0, UINT8_MAX);
    temp.m_b = std::clamp(m_b - b, 0, UINT8_MAX);

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator*(const color& b) const noexcept
{
    color temp;

    temp.m_r = std::min(m_r * b.m_r, UINT8_MAX);
    temp.m_g = std::min(m_g * b.m_g, UINT8_MAX);
    temp.m_b = std::min(m_b * b.m_b, UINT8_MAX);

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color color::operator*(const int b) const noexcept
{
    color temp;

    temp.m_r = std::clamp(m_r * b, 0, UINT8_MAX);
    temp.m_g = std::clamp(m_g * b, 0, UINT8_MAX);
    temp.m_b = std::clamp(m_b * b, 0, UINT8_MAX);

    return temp;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator+=(const color& b) noexcept
{
    m_r = std::min(m_r + b.m_r, UINT8_MAX);
    m_g = std::min(m_g + b.m_g, UINT8_MAX);
    m_b = std::min(m_b + b.m_b, UINT8_MAX);

    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator+=(const int b) noexcept
{
    m_r = std::clamp(m_r + b, 0, UINT8_MAX);
    m_g = std::clamp(m_g + b, 0, UINT8_MAX);
    m_b = std::clamp(m_b + b, 0, UINT8_MAX);

    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator-=(const color& b) noexcept
{
    m_r = std::max(0, m_r - b.m_r);
    m_g = std::max(0, m_g - b.m_g);
    m_b = std::max(0, m_b - b.m_b);

    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator-=(const int b) noexcept
{
    m_r = std::clamp(m_r - b, 0, UINT8_MAX);
    m_g = std::clamp(m_g - b, 0, UINT8_MAX);
    m_b = std::clamp(m_b - b, 0, UINT8_MAX);

    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator*=(const color& b) noexcept
{
    m_r = std::min(m_r * b.m_r, UINT8_MAX);
    m_g = std::min(m_g * b.m_g, UINT8_MAX);
    m_b = std::min(m_b * b.m_b, UINT8_MAX);

    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator*=(const int b) noexcept
{
    m_r = std::clamp(m_r * b, 0, UINT8_MAX);
    m_g = std::clamp(m_g * b, 0, UINT8_MAX);
    m_b = std::clamp(m_b * b, 0, UINT8_MAX);

    return *this;
}


////////////////////////////////////////////////////////////////////////////////
color& color::operator++() noexcept
{
    m_r = std::min(m_r + 1, UINT8_MAX);
    m_g = std::min(m_g + 1, UINT8_MAX);
    m_b = std::min(m_b + 1, UINT8_MAX);
    
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
    m_r = std::max(0, m_r - 1);
    m_g = std::max(0, m_g - 1);
    m_b = std::max(0, m_b - 1);
    
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
