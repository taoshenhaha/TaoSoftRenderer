#pragma once
#include "FrameBuffer.h"
/*
 * @brief Rasterizer类
 * 用于将3D模型转换为2D像素
 * @details 该类包含了所有的像素绘制代码，包括线段、三角形等
 * @note 该类依赖于Framebuffer类，用于存储渲染结果
*/

typedef struct {int min_x, min_y, max_x, max_y;} bbox_t;

class Rasterizer
{
public:
    Rasterizer(FrameBuffer *fb)
        : mFrameBuffer(fb)
    {
    }
    ~Rasterizer();
    FrameBuffer* getFramebuffer()
    {
        return mFrameBuffer;
    }
    void clearColor(Vec4<float> color);
    void clearDepth(float depth);
    bbox_t boundingBox(Vec2<float> p1, Vec2<float> p2, Vec2<float> p3);
    Vec3<float> calcuateWeight(Vec2<float> p, Vec2<float> p1, Vec2<float> p2, Vec2<float> p3);
    void drawLine(Vec3<float> p1, Vec3<float> p2);
    void drawTriangle(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3);
    void setPixel(int x, int y, Vec4<float> color);
private:
    void drawLineBresenham(Vec3<float> p1, Vec3<float> p2);

private:
    FrameBuffer *mFrameBuffer{ nullptr };
};