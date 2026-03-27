#include "Vec4.hpp"
#include "Mat3.hpp"

template <typename T>
class Mat4
{
public:
union
{
    struct
    {
        T m00, m01, m02, m03
        T m10, m11, m12, m13
        T m20, m21, m22, m23
        T m30, m31, m32, m33
    };
    T m[4][4];
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

explicit Mat(T (&m_other)[16])
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
Mat4<T> operator*(const Vec4<T>& rhs) const
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


};