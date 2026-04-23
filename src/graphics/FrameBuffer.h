#pragma once
#include "Vec4.hpp"
#include "Vec2.hpp"
/*
 * @brief Framebuffer类
 * 用于存储渲染结果的缓冲区
 * @details 该类包含了颜色缓冲区和深度缓冲区，用于存储像素颜色和深度信息
 * @note 该类依赖于Vec4类，用于存储像素颜色信息
 */
class FrameBuffer 
{
public:
    FrameBuffer(int width, int height)
        : mWidth(width)
        , mHeight(height)
        , mColorBuffer(new Vec4<float>[width * height]) // RGBA
        , mDepthBuffer(new float[width * height])
    {
    }

    ~FrameBuffer()
    {
        delete[] mColorBuffer;
        delete[] mDepthBuffer;
    }

    void clearColor( Vec4<float> color);
    void clearDepth(float depth);
    void setPixel(int x, int y, Vec4<float> color);
    void setDepth(int x, int y, float depth);
    float getDepth(int x, int y);
   public:
    int mWidth{ 0 };
    int mHeight{ 0 };
    Vec4<float> *mColorBuffer{ nullptr };
    float *mDepthBuffer{ nullptr };
};