#pragma once
#include "Vec4.hpp"

class Framebuffer 
{
public:
    Framebuffer(int width, int height)
        : mWidth(width)
        , mHeight(height)
        , mColorBuffer(new Vec4<float>[width * height]) // RGBA
        , mDepthBuffer(new float[width * height])
    {
    }

    ~Framebuffer()
    {
        delete[] mColorBuffer;
        delete[] mDepthBuffer;
    }

    void clear_color( Vec4<float> color);
    void clear_depth(float depth);
    void set_pixel(int x, int y, Vec4<float> color);
public:
    int mWidth{ 0 };
    int mHeight{ 0 };
    Vec4<float> *mColorBuffer{ nullptr };
    float *mDepthBuffer{ nullptr };
};