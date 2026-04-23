#pragma once

#include "../core/Vec2.hpp"
#include "../core/Vec4.hpp"
#include "../core/Image.h"
#include <string>

enum class TextureFilter : uint8_t
{
    NEAREST,
    LINEAR
};

enum class TextureWrap : uint8_t
{
    REPEAT,
    CLAMP
};

class Texture
{
public:
    Texture();
    Texture(Image* image);
    ~Texture();

    void setImage(Image* image);
    void setFilter(TextureFilter minFilter, TextureFilter magFilter);
    void setWrap(TextureWrap wrapU, TextureWrap wrapV);

    Vec4<float> sample(Vec2<float> uv) const;
    Vec4<float> sampleNearest(Vec2<float> uv) const;
    Vec4<float> sampleLinear(Vec2<float> uv) const;

    int getWidth() const { return mWidth; }
    int getHeight() const { return mHeight; }
    bool isValid() const { return mIsValid; }

private:
    Vec4<float> sampleClamp(Vec2<float> uv) const;
    Vec4<float> sampleRepeat(Vec2<float> uv) const;

    Vec4<float> getPixelBilinear(float u, float v) const;
    Vec4<float> getPixel(int x, int y) const;

private:
    int mWidth{ 0 };
    int mHeight{ 0 };
    int mChannels{ 0 };
    unsigned char* mData{ nullptr };
    TextureFilter mMinFilter{ TextureFilter::NEAREST };
    TextureFilter mMagFilter{ TextureFilter::LINEAR };
    TextureWrap mWrapU{ TextureWrap::REPEAT };
    TextureWrap mWrapV{ TextureWrap::REPEAT };
    bool mIsValid{ false };
};
