#pragma once
#include "Vec3.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>

template <typename T>
class Mat3
{
public:
    union
    {
        T m[3][3]; //
        T m_flat[9];

        Vec3<T> row[3]; // row major

        struct
        {   
            T m00, m01, m02;
            T m10, m11, m12;
            T m20, m21, m22;
        };
        
        /* data */
    };

public:

Mat3()
{
    m00 = m11 = m22 = 1;
    m01 = m02 = m10 = m12 = m20 = m21 = 0;
}

Mat3(T m00, T m01, T m02,
      T m10, T m11, T m12,
      T m20, T m21, T m22)
{
    this->m00 = m00;
    this->m01 = m01;
    this->m02 = m02;
    this->m10 = m10;
    this->m11 = m11;
    this->m12 = m12;
    this->m20 = m20;
    this->m21 = m21;
    this->m22 = m22;
}

Mat3(const Vec3<T>& row0, const Vec3<T>& row1, const Vec3<T>& row2)
{
    this->row[0] = row0;
    this->row[1] = row1;
    this->row[2] = row2;
}

explicit Mat3(T (&m_other)[9])
{
    memcpy(m_flat, m_other, sizeof(T) * 9);
}

explicit Mat3(T m_other[3][3])
{
    memcpy(m[0], m_other[0], sizeof(T) * 3);
    memcpy(m[1], m_other[1], sizeof(T) * 3);
    memcpy(m[2], m_other[2], sizeof(T) * 3);
}

Mat3(const Mat3& other)
{
    memcpy(m_flat, other.m_flat, sizeof(T) * 9);
}

Mat3& operator=(const Mat3& other)
{
    memcpy(m_flat, other.m_flat, sizeof(T) * 9);
    return *this;
}

Mat3(T scalar)
{
    memset(m_flat, scalar, sizeof(T) * 9);
}

// 元素访问
T& operator()(int row, int col)
{
    return m[row][col];
}
T operator()(int row, int col) const
{
    return m[row][col];
}
T& at(int row, int col)       // 带边界检查
{
    assert(row >= 0 && row < 3);
    assert(col >= 0 && col < 3);
    if (row < 0 || row >= 3 || col < 0 || col >= 3)
    {
        return T(0);
    }
    return m[row][col];
}

// 行/列访问
Vec3<T> getRow(int index) const
{
    return row[index];
}
Vec3<T> getCol(int index) const
{
    return Vec3<T>(m[0][index], m[1][index], m[2][index]);
}
void setRow(int index, const Vec3<T>& row)
{
    this->row[index] = row;
}
void setCol(int index, const Vec3<T>& col)
{
    m[0][index] = col.x;
    m[1][index] = col.y;
    m[2][index] = col.z;
} 

// 数据访问
const T* data() const          // 获取原始数据指针
{
    return m_flat;
}

T* data()          // 获取原始数据指针
{
    return m_flat;
}

// 一元运算
Mat3 operator+() const
{
    return *this;
}
Mat3 operator-() const
{
    return Mat3(-m00, -m01, -m02,
                -m10, -m11, -m12,
                -m20, -m21, -m22);
}

// 矩阵-矩阵运算
Mat3 operator+(const Mat3& rhs) const
{
    return Mat3(m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02,
                m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12,
                m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22);
}
Mat3 operator-(const Mat3& rhs) const
{
    return Mat3(m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02,
                m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12,
                m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22);
}
Mat3 operator*(const Mat3& rhs) const
{
    //矩阵乘法
    return Mat3(m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20,
                m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21,
                m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22,
                m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20,
                m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21,
                m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22,
                m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20,
                m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21,
                m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22);
}

Mat3& operator+=(const Mat3& rhs)
{
    m00 += rhs.m00;
    m01 += rhs.m01;
    m02 += rhs.m02;
    m10 += rhs.m10;
    m11 += rhs.m11;
    m12 += rhs.m12;
    m20 += rhs.m20;
    m21 += rhs.m21;
    m22 += rhs.m22;
    return *this;
}
Mat3& operator-=(const Mat3& rhs)
{
    m00 -= rhs.m00;
    m01 -= rhs.m01;
    m02 -= rhs.m02;
    m10 -= rhs.m10;
    m11 -= rhs.m11;
    m12 -= rhs.m12;
    m20 -= rhs.m20;
    m21 -= rhs.m21;
    m22 -= rhs.m22;
    return *this;
}
Mat3& operator*=(const Mat3& rhs)
{
    *this = *this * rhs;
    return *this;
}

// 矩阵-标量运算
Mat3 operator*(float scalar) const
{
    return Mat3(m00 * scalar, m01 * scalar, m02 * scalar,
                m10 * scalar, m11 * scalar, m12 * scalar,
                m20 * scalar, m21 * scalar, m22 * scalar);
}
Mat3 operator/(float scalar) const
{
    return Mat3(m00 / scalar, m01 / scalar, m02 / scalar,
                m10 / scalar, m11 / scalar, m12 / scalar,
                m20 / scalar, m21 / scalar, m22 / scalar);
}
Mat3& operator*=(float scalar)
{
    m00 *= scalar;
    m01 *= scalar;
    m02 *= scalar;
    m10 *= scalar;
    m11 *= scalar;
    m12 *= scalar;
    m20 *= scalar;
    m21 *= scalar;
    m22 *= scalar;
    return *this;
}
Mat3& operator/=(float scalar)
{
    m00 /= scalar;
    m01 /= scalar;
    m02 /= scalar;
    m10 /= scalar;
    m11 /= scalar;
    m12 /= scalar;
    m20 /= scalar;
    m21 /= scalar;
    m22 /= scalar;
    return *this;
}

friend Mat3 operator*(float scalar, const Mat3& m)  // 标量前置
{
    return m * scalar;
}

Vec3<T> operator*(const Vec3<T>& v) const  // 矩阵×向量
{
    return Vec3<T>(m00 * v.x + m01 * v.y + m02 * v.z,
                   m10 * v.x + m11 * v.y + m12 * v.z,
                   m20 * v.x + m21 * v.y + m22 * v.z);
}

Vec3<T> transform(const Vec3<T>& v) const  // 明确的变换函数
{
    return *this * v;
}

T determinant() const          // 行列式
{
    return m00 * m11 * m22 + m01 * m12 * m20 + m02 * m10 * m21 -
           m02 * m11 * m20 - m01 * m10 * m22 - m00 * m12 * m21;
}

T trace() const                // 迹
{
    return m00 + m11 + m22;
}

bool isInvertible() const          // 是否可逆
{
    return std::abs(determinant()) > std::numeric_limits<T>::epsilon();
}

Mat3 inverse() const               // 逆矩阵
{
    if (!isInvertible())
    {
        return Mat3();
    }
    return adjugate() / determinant();
}

Mat3 transpose() const             // 转置
{
    return Mat3(m00, m10, m20,
                m01, m11, m21,
                m02, m12, m22);
}


Mat3 adjugate() const              // 伴随矩阵
{
    return Mat3(m11 * m22 - m12 * m21, m02 * m21 - m01 * m22, m01 * m12 - m02 * m11,
                m12 * m20 - m10 * m22, m00 * m22 - m02 * m20, m02 * m10 - m00 * m12,
                m10 * m21 - m11 * m20, m01 * m20 - m00 * m21, m00 * m11 - m01 * m10);
}

void getScaleRotation(Vec3<T>& scale, Mat3<T>& rotation) const  // 缩放和旋转分离
{
    rotation = getRotation();
    scale = getScale();
}


Mat3 getRotation() const           // 提取旋转部分，这个矩阵包含了错切变换
{
    Vec3<T> r0 = row[0];
    Vec3<T> r1 = row[1];
    Vec3<T> r2 = row[2];
    
    float q1_len = std::sqrt(r0.x * r0.x + r0.y * r0.y + r0.z * r0.z);
    float q2_len = std::sqrt(r1.x * r1.x + r1.y * r1.y + r1.z * r1.z);
    float q3_len = std::sqrt(r2.x * r2.x + r2.y * r2.y + r2.z * r2.z);
    
    if (q1_len < 1e-6f || q2_len < 1e-6f || q3_len < 1e-6f)
    {
        return Mat3();
    }
    
    Vec3<T> q1 = r0 / q1_len;
    
    Vec3<T> temp = r1 - q1 * (r1.x * q1.x + r1.y * q1.y + r1.z * q1.z);
    float temp_len = std::sqrt(temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
    Vec3<T> q2 = (temp_len > 1e-6f) ? temp / temp_len : Vec3<T>(0, 0, 0);
    
    temp = r2 - q1 * (r2.x * q1.x + r2.y * q1.y + r2.z * q1.z)
             - q2 * (r2.x * q2.x + r2.y * q2.y + r2.z * q2.z);
    temp_len = std::sqrt(temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
    Vec3<T> q3 = (temp_len > 1e-6f) ? temp / temp_len : Vec3<T>(0, 0, 0);
    
    Mat3 Q;
    Q.row[0] = q1;
    Q.row[1] = q2;
    Q.row[2] = q3;
    
    if (Q.determinant() < 0)
    {
        Q.row[0] = -Q.row[0];
        Q.row[1] = -Q.row[1];
        Q.row[2] = -Q.row[2];
    }
    
    return Q;
}


Vec3<T> getScale() const              // 提取缩放
{
    Mat3 Q = getRotation();
    Mat3 Q_T = Q.transpose();
    
    Mat3 R = Q_T * (*this);
    
    float s0 = std::sqrt(R.m00 * R.m00 + R.m01 * R.m01 + R.m02 * R.m02);
    float s1 = std::sqrt(R.m10 * R.m10 + R.m11 * R.m11 + R.m12 * R.m12);
    float s2 = std::sqrt(R.m20 * R.m20 + R.m21 * R.m21 + R.m22 * R.m22);
    
    return Vec3<T>(s0, s1, s2);
}
Vec3<T> getEulerAngles() const         // 获取欧拉角 (XYZ 顺序，返回 roll, pitch, yaw)
{
    Mat3 R = getRotation();
    
    float sy = std::sqrt(R.m00 * R.m00 + R.m10 * R.m10);
    
    bool singular = sy < 1e-6f;
    
    float x, y, z;
    if (!singular)
    {
        x = std::atan2(R.m21, R.m22);
        y = std::atan2(-R.m20, sy);
        z = std::atan2(R.m10, R.m00);
    }
    else
    {
        x = std::atan2(-R.m12, R.m11);
        y = std::atan2(-R.m20, sy);
        z = 0;
    }
    
    return Vec3<T>(x, y, z);
}









    
};