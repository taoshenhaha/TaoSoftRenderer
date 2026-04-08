#pragma once
#include "Vec4.hpp"
#include "Mat3.hpp"

#include <iostream>


template <typename T>
class Mat4
{
public:
union
{
    struct
    {
        T m00, m01, m02, m03;
        T m10, m11, m12, m13;
        T m20, m21, m22, m23;
        T m30, m31, m32, m33;
    };
    T m[4][4];
    T m_flat[16];
    Vec4<T> rows[4];
    /* data */
};

Mat4()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = i == j ? 1 : 0;
        }
    }
}

Mat4(const Mat4& rhs)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = rhs.m[i][j];
        }
    }
}

explicit Mat4(T m_other[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = m_other[i][j];
        }
    }
}

explicit Mat4(T (&m_other)[16])
{
    memcpy(m_flat, m_other, sizeof(T) * 16);
}

Mat4<T> operator*(const Mat4<T>& rhs) const
{ 
    Mat4<T> result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] = T(0);
            for (int k = 0; k < 4; k++)
            {
                result.m[i][j] += m[i][k] * rhs.m[k][j];
            }
        }
    }
    return result;
}
Mat4<T> operator*(T rhs) const
{
    Mat4<T> result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] = m[i][j] * rhs;
        }
    }
    return result;
}

Mat4<T> operator/(T rhs) const
{
    Mat4<T> result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] = m[i][j] / rhs;
        }
    }
    return result;
}

Mat4<T> operator+(const Mat4<T>& rhs) const
{
    Mat4<T> result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] = m[i][j] + rhs.m[i][j];
        }
    }
    return result;
}
Mat4<T> operator-(const Mat4<T>& rhs) const
{
    Mat4<T> result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] = m[i][j] - rhs.m[i][j];
        }
    }
    return result;
}
Mat4<T> operator-() const
{
    Mat4<T> result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] = -m[i][j];
        }
    }
    return result;
}
Mat4<T>& operator*=(T rhs)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] *= rhs;
        }
    }
    return *this;
}
Mat4<T>& operator/=(T rhs)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] /= rhs;
        }
    }
    return *this;
}
Mat4<T>& operator+=(const Mat4<T>& rhs)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] += rhs.m[i][j];
        }
    }
    return *this;
}
Mat4<T>& operator-=(const Mat4<T>& rhs)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] -= rhs.m[i][j];
        }
    }
    return *this;
}
Vec4<T> operator*(const Vec4<T>& rhs) const
{
    Vec4<T> result;
    for (int i = 0; i < 4; i++)
    {
        result[i] = T(0);
        for (int j = 0; j < 4; j++)
        {
            result[i] += m[i][j] * rhs[j];
        }
    }
    return result;
}
Vec3<T> operator*(const Vec3<T>& rhs) const
{
    Vec3<T> result;
    Vec3<T> rhs_vec3 = rhs;
    for (int i = 0; i < 4; i++)
    {
        result[i] = T(0);
        for (int j = 0; j < 4; j++)
        {
            result[i] += m[i][j] * rhs_vec3[j];
        }
    }
    return result;
}

/*
 * angle: the angle of rotation, in radians
 *
 *  1  0  0  0
 *  0  c -s  0
 *  0  s  c  0
 *  0  0  0  1
 *
 * see http://www.songho.ca/opengl/gl_anglestoaxes.html
 */
Mat4<T> mat4RotateX(float angle) 
{
    float c = (float)cos(angle);
    float s = (float)sin(angle);
    Mat4<T> m;
    m.m[1][1] = c;
    m.m[1][2] = -s;
    m.m[2][1] = s;
    m.m[2][2] = c;
    return m;
}

/*
 * angle: the angle of rotation, in radians
 *
 *  c  0  s  0
 *  0  1  0  0
 * -s  0  c  0
 *  0  0  0  1
 *
 * see http://www.songho.ca/opengl/gl_anglestoaxes.html
 */
Mat4<T> mat4RotateY(float angle)
{
    float c = (float)cos(angle);
    float s = (float)sin(angle);
    Mat4<T> m;
    m.m[0][0] = c;
    m.m[0][2] = s;
    m.m[2][0] = -s;
    m.m[2][2] = c;
    return m;
}

/*
 * angle: the angle of rotation, in radians
 *
 *  c -s  0  0
 *  s  c  0  0
 *  0  0  1  0
 *  0  0  0  1
 *
 * see http://www.songho.ca/opengl/gl_anglestoaxes.html
 */
Mat4<T> mat4RotateZ(float angle) 
{
    float c = (float)cos(angle);
    float s = (float)sin(angle);
    Mat4<T> m;
    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;
    return m;
}

/*
 * eye: the position of the eye point
 * target: the position of the target point
 * up: the direction of the up vector
 *
 * x_axis.x  x_axis.y  x_axis.z  -dot(x_axis,eye)
 * y_axis.x  y_axis.y  y_axis.z  -dot(y_axis,eye)
 * z_axis.x  z_axis.y  z_axis.z  -dot(z_axis,eye)
 *        0         0         0                 1
 *
 * z_axis: normalize(eye-target), the backward vector
 * x_axis: normalize(cross(up,z_axis)), the right vector
 * y_axis: cross(z_axis,x_axis), the up vector
 *
 * see http://www.songho.ca/opengl/gl_camera.html
 */


};