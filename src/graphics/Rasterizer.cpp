#include "Rasterizer.h"
#include <cmath>
#include <cstdlib>

Rasterizer::~Rasterizer()
{
    delete mFramebuffer;
    mFramebuffer = nullptr;
}

void Rasterizer::draw_line(Vec3<float> p1, Vec3<float> p2)
{
    if (mFramebuffer == nullptr)
    {
        return;
    }

    draw_line_bresenham(p1, p2);
}

void Rasterizer::draw_line_bresenham(Vec3<float> p1, Vec3<float> p2)
{ 
    // 使用四舍五入而不是截断，提高精度
    int x1 = (int)std::round(p1.x);
    int y1 = (int)std::round(p1.y);
    int x2 = (int)std::round(p2.x);
    int y2 = (int)std::round(p2.y);
    
    // 检查是否为同一个点
    if (x1 == x2 && y1 == y2)
    {
        set_pixel(x1, y1, { 1.0f, 0.0f, 1.0f, 1.0f });
        return;
    }
    
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    
    // 处理特殊情况：垂直线
    if (dx == 0)
    {
        for (int y = y1; y != y2 + sy; y += sy)
        {
            set_pixel(x1, y, { 1.0f, 0.0f, 1.0f, 1.0f });
        }
        return;
    }
    
    // 处理特殊情况：水平线
    if (dy == 0)
    {
        for (int x = x1; x != x2 + sx; x += sx)
        {
            set_pixel(x, y1, { 1.0f, 0.0f, 1.0f, 1.0f });
        }
        return;
    }
    
    int err = dx - dy;
    
    // 根据斜率选择主要方向
    if (dx > dy)
    {
        // 斜率小于1，x为主要方向
        while (x1 != x2)
        {
            set_pixel(x1, y1, { 1.0f, 0.0f, 1.0f, 1.0f });
            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y1 += sy;
            }
        }
    }
    else
    {
        // 斜率大于1，y为主要方向
        while (y1 != y2)
        {
            set_pixel(x1, y1, { 1.0f, 0.0f, 1.0f, 1.0f });
            int e2 = 2 * err;
            if (e2 > -dx)
            {
                err -= dx;
                y1 += sy;
            }
            if (e2 < dy)
            {
                err += dy;
                x1 += sx;
            }
        }
    }
    
    // 绘制最后一个点
    set_pixel(x2, y2, { 1.0f, 0.0f, 1.0f, 1.0f });
}
void Rasterizer::draw_triangle(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3)
{
    if (mFramebuffer == nullptr)
    {
        return;
    }
}
void Rasterizer::set_pixel(int x, int y, Vec4<float> color)
{
    if (mFramebuffer == nullptr)
    {
        return;
    }
    if (x < 0 || x >= mFramebuffer->mWidth || y < 0 || y >= mFramebuffer->mHeight)
    {
        return;
    }
    mFramebuffer->set_pixel(x, y, color);
}

void Rasterizer::clear_color(Vec4<float> color)
{
    if (mFramebuffer == nullptr)
    {
        return;
    }
    mFramebuffer->clear_color(color);
}
