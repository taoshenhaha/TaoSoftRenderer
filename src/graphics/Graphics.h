#pragma once
#include "FrameBuffer.h"
#include "Vec3.hpp"
class Graphics
{
public:
    Graphics();
    ~Graphics();
    void draw_line(Vec3<float> p1, Vec3<float> p2);
    void draw_triangle(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3);
private:
    void set_pixel(int x, int y, Vec4<float> color);
};
