#pragma once

#include "Vec4.hpp"
#include <cassert>

constexpr float MY_PI = 3.1415927f;
constexpr float EPSILON = 1e-5f;
constexpr int LINE_SIZE = 256;
constexpr int PATH_SIZE = 256;
unsigned char float_to_uchar(float value);
int is_vertex_visible(Vec4<float> v);

template<typename T>
T lerp(T a, T b, T ratio)
{
    return a + (b - a) * ratio;
}

template<typename T>
T interpolate1(T a, T b, T c, const Vec3<float>& weights) {
    return a * weights.x + b * weights.y + c * weights.z;
}

template<typename T>
T clamp1(const T& value, const T& low, const T& high) {
    assert(low <= high);
    if (value < low) return low;
    if (value > high) return high;
    return value;
}