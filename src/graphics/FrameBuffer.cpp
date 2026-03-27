#include "FrameBuffer.h"
#include "utils/utils.h"
void Framebuffer::clear_color( Vec4<float> color) 
{
    int num_pixels = mWidth * mHeight;
    int i;
    for (i = 0; i < num_pixels; i++) 
    {
        mColorBuffer[i].x = float_to_uchar(color.x);
        mColorBuffer[i].y = float_to_uchar(color.y);
        mColorBuffer[i].z = float_to_uchar(color.z);
        mColorBuffer[i].w = float_to_uchar(color.w);
    }
}

void Framebuffer::clear_depth(float depth) 
{
    int num_pixels = mWidth * mHeight;
    int i;
    for (i = 0; i < num_pixels; i++) 
    {
        mDepthBuffer[i] = depth;
    }
}

void Framebuffer::set_pixel(int x, int y, Vec4<float> color) 
{
    mColorBuffer[y * mWidth + x].x = float_to_uchar(color.x);
    mColorBuffer[y * mWidth + x].y = float_to_uchar(color.y);
    mColorBuffer[y * mWidth + x].z = float_to_uchar(color.z);
    mColorBuffer[y * mWidth + x].w = float_to_uchar(color.w);
}