#include "Graphics.h"

Graphics::Graphics()
{
}
Graphics::~Graphics()
{
}
void Graphics::draw_line(Vec3<float> p1, Vec3<float> p2)
{
    //绘制直线
    if (p1.x == p2.x)
    {
        //垂直线
        for (int y = p1.y; y <= p2.y; y++)
        {
            set_pixel(p1.x, y, Vec4<float>(1.0f, 1.0f, 1.0f, 1.0f));
        }

    }
    else if (p1.y == p2.y)
    {
        //水平线
        for (int x = p1.x; x <= p2.x; x++)
        {
            set_pixel(x, p1.y, Vec4<float>(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }
    else
    {
        //斜线
        //Bresenham算法
        int dx = p2.x - p1.x;
        int dy = p2.y - p1.y;
        int err = -dy;
        int x = p1.x;
        int y = p1.y;
        while (x != p2.x || y != p2.y)
        {
            set_pixel(x, y, Vec4<float>(1.0f, 1.0f, 1.0f, 1.0f));
            err += dy;
            if (err >= dx)
            {
                err -= dx;
                x += 1;
            }
        }
    }
}
void Graphics::draw_triangle(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3)
{
}

void Graphics::set_pixel(int x, int y, Vec4<float> color)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
    {
        return;
    }
    mColorBuffer[y * mWidth + x] = color;
}