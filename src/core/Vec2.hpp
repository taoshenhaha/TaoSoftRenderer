#pragma once

template <typename T>
class Vec2
{
public:
    T x, y;
    Vec2() = default;
    Vec2(T x_, T y_)
        : x(x_), y(y_) {}
    explicit Vec2(T scaler)
        : x(scaler), y(scaler) {}

    Vec2 operator-(const Vec2& rhs) const
    {
        return Vec2(x - rhs.x, y - rhs.y);
    }
};