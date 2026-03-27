#pragma once    
#include "Vec3.hpp"

template <typename T>
class Vec4
{
public:
    T x, y, z, w;
    Vec4() = default;
    Vec4(T x_, T y_, T z_, T w_)
        : x(x_), y(y_), z(z_), w(w_) {}
    explicit Vec4(T scaler)
        : x(scaler), y(scaler), z(scaler), w(scaler) {}

    Vec4(const Vec3<T>& vec3, T w_)
        : x(vec3.x), y(vec3.y), z(vec3.z), w(w_) {}
    
    
    bool operator==(const Vec4& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
    }
    
    bool operator!=(const Vec4& rhs) const
    {
        return !(*this == rhs);
    }
    Vec4 operator+(const Vec4& rhs) const
    {
        return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }
    
    Vec4 operator-(const Vec4& rhs) const
    {
        return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }
    
    Vec4 operator*(T scaler) const
    {
        return Vec4(x * scaler, y * scaler, z * scaler, w * scaler);
    }
    
    Vec4 operator/(T scaler) const
    {
        return Vec4(x / scaler, y / scaler, z / scaler, w / scaler);
    }
    
    Vec4 operator-() const
    {
        return Vec4(-x, -y, -z, -w);
    }
    
    Vec4& operator+=(const Vec4& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }
    
    Vec4& operator-=(const Vec4& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }
    
    Vec4& operator*=(T scaler)
    {
        x *= scaler;
        y *= scaler;
        z *= scaler;
        w *= scaler;
        return *this;
    }
    
    Vec4& operator/=(T scaler)
    {
        x /= scaler;
        y /= scaler;
        z /= scaler;
        w /= scaler;
        return *this;
    }

    Vec4 operator+(const Vec3<T>& rhs) const
    {
        return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w);
    }
    
    Vec4 operator-(const Vec3<T>& rhs) const
    {
        return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w);
    }


    T dot(const Vec4& rhs) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }

    T length() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    
    Vec4 normalize() const
    {
        return *this / length();
    }

    Vec3<T> toVec3() const
    {
        return Vec3<T>(x, y, z);
    }

    Vec4<T> toVec4() const
    {
        return Vec4<T>(x, y, z, w);
    }






        







};
