#pragma once

#include <stdint.h>

enum class format_t : uint16_t
{
    FORMAT_LDR, 
    FORMAT_HDR
};

class Image
{
public:
    int mWidth{ 0 };
    int mHeight{ 0 };
    int mChannels{ 0 };
    format_t mFormat{ format_t::FORMAT_LDR };
    unsigned char *mLdrBuffer{ nullptr };
    float *mHdrBuffer{ nullptr };
    bool mIsInitialized{ false };

public:
   
    Image(int width, int height, int channels, format_t format);
    ~Image()
    {
        release();
    }
    void release();
};