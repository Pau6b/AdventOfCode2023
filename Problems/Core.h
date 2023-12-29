#pragma once
#include <inttypes.h>

namespace aoc
{
struct Vec2
{
    int32_t x;
    int32_t y;

    Vec2& operator+=(const Vec2& i_other)
    {
        x += i_other.x;
        y += i_other.y;
        return *this;
    }

    Vec2 operator+(const Vec2& i_other)
    {
        Vec2 result = *this;
        result += i_other;
        return result;
    }

    bool operator==(const Vec2& i_other) const
    {
        return x == i_other.x && y == i_other.y;
    }
};
}