#include "Image.h"
#include <stdlib.h>
#include <string.h>

Image::Image(int width, int height, int channels, format_t format)
: mWidth(width), 
  mHeight(height), 
  mChannels(channels), 
  mFormat(format)

{   
    if (mFormat == format_t::FORMAT_LDR) {
        mLdrBuffer = (unsigned char*)malloc(width * height * channels);
        mHdrBuffer = NULL;
        memset(mLdrBuffer, 0, width * height * channels);
    } else {
        mHdrBuffer = (float*)malloc(width * height * channels * sizeof(float));
        mLdrBuffer = NULL;
        memset(mHdrBuffer, 0, width * height * channels * sizeof(float));
    }
    mIsInitialized = true;
}

void Image::release()
{
    if(!mIsInitialized) return;
    if (mLdrBuffer) free(mLdrBuffer);
    if (mHdrBuffer) free(mHdrBuffer);
}