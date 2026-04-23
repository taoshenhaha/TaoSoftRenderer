#pragma once
#include <algorithm>

template <typename T>
class Vec3
{
public:
    T x, y, z;
    Vec3() = default;
    Vec3(T x_, T y_, T z_)
        : x(x_), y(y_), z(z_) {}
    explicit Vec3(T scaler)
        : x(scaler), y(scaler), z(scaler) {}
    
    T& operator[](int index)
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            return x;
        }
    }
    Vec3 operator+(const Vec3& rhs) const
    {
        return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Vec3 operator-(const Vec3& rhs)
    {
        return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Vec3 operator*(T scaler) const
    {
        return Vec3(x * scaler, y * scaler, z * scaler);
    }

    Vec3 operator/(T scaler)
    {
        return Vec3(x / scaler, y / scaler, z / scaler);
    }
     Vec3 operator-()
    {
        return Vec3(-x, -y, -z);
    }
     Vec3& operator+=(const Vec3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

     Vec3& operator-=(const Vec3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
     Vec3& operator*=(T scaler)
    {
        x *= scaler;
        y *= scaler;
        z *= scaler;
        return *this;
    }
     Vec3& operator/=(T scaler)
    {
        x /= scaler;
        y /= scaler;
        z /= scaler;
        return *this;
    }
     bool operator==(const Vec3& rhs)
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
     bool operator!=(const Vec3& rhs)
    {
        return !(*this == rhs);
    }
     T dot(const Vec3& rhs)
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }
     Vec3 cross(const Vec3& rhs)
    {
        return Vec3(y * rhs.z - z * rhs.y,
                    z * rhs.x - x * rhs.z,
                    x * rhs.y - y * rhs.x);
    }
     Vec3 normalize()
    {
        int len = length();
        if (len == 0)
        {
            return *this;
        }
        return *this / T(len);
    }
     T length()
    {
        return T(sqrt(dot(*this)));
    }
     Vec3 reflect(const Vec3& normal)
    {
        return *this - normal * (normal.dot(*this) * T(2));
    }
    //计算一个垂直于normal的向量
     Vec3 orthogonal(const Vec3& normal)
    { 
        Vec3 v = Vec3(T(1), T(1), T(1));
        if (dot(normal) > T(0))
        {
            v = -v;
        }
        return (v - normal * dot(v)).normalize();
    }
     Vec3 refract(const Vec3& normal, T eta)
    { 
    }
     Vec3 clamp(T min, T max)
    {
        x = std::min(std::max(x, min), max);
        y = std::min(std::max(y, min), max);
        z = std::min(std::max(z, min), max);
        return *this;
    }
     Vec3& clamp(Vec3 min, Vec3 max)
    {
        x = std::min(std::max(x, min.x), max.x);
        y = std::min(std::max(y, min.y), max.y);
        z = std::min(std::max(z, min.z), max.z);
        return *this;
    }

    Vec3 lerp(const Vec3& rhs, T ratio) const
    {
        return Vec3(x + (rhs.x - x) * ratio, y + (rhs.y - y) * ratio, z + (rhs.z - z) * ratio);
    }








};