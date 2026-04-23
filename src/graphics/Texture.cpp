#include "Texture.h"
#include <cmath>
#include <cstring>

Texture::Texture()
{
}

Texture::Texture(Image* image)
{
    setImage(image);
}

Texture::~Texture()
{
    if (mData)
    {
        delete[] mData;
        mData = nullptr;
    }
}

void Texture::setImage(Image* image)
{
    if (!image || !image->mIsInitialized)
    {
        mIsValid = false;
        return;
    }

    if (mData)
    {
        delete[] mData;
    }

    mWidth = image->mWidth;
    mHeight = image->mHeight;
    mChannels = image->mChannels;

    int dataSize = mWidth * mHeight * mChannels;
    mData = new unsigned char[dataSize];

    if (image->mLdrBuffer)
    {
        memcpy(mData, image->mLdrBuffer, dataSize);
    }
    else if (image->mHdrBuffer)
    {
        for (int i = 0; i < dataSize; i++)
        {
            mData[i] = static_cast<unsigned char>(image->mHdrBuffer[i] * 255.0f);
        }
    }

    mIsValid = true;
}

void Texture::setFilter(TextureFilter minFilter, TextureFilter magFilter)
{
    mMinFilter = minFilter;
    mMagFilter = magFilter;
}

void Texture::setWrap(TextureWrap wrapU, TextureWrap wrapV)
{
    mWrapU = wrapU;
    mWrapV = wrapV;
}

Vec4<float> Texture::sample(Vec2<float> uv) const
{
    if (!mIsValid)
    {
        return Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f);
    }

    if (mMagFilter == TextureFilter::NEAREST)
    {
        return sampleNearest(uv);
    }
    else
    {
        return sampleLinear(uv);
    }
}

Vec4<float> Texture::sampleNearest(Vec2<float> uv) const
{
    Vec2<float> wrappedUV;
    if (mWrapU == TextureWrap::REPEAT)
    {
        wrappedUV.x = uv.x - std::floor(uv.x);
    }
    else
    {
        wrappedUV.x = std::max(0.0f, std::min(1.0f, uv.x));
    }

    if (mWrapV == TextureWrap::REPEAT)
    {
        wrappedUV.y = uv.y - std::floor(uv.y);
    }
    else
    {
        wrappedUV.y = std::max(0.0f, std::min(1.0f, uv.y));
    }

    int x = static_cast<int>(wrappedUV.x * mWidth);
    int y = static_cast<int>(wrappedUV.y * mHeight);

    x = std::max(0, std::min(mWidth - 1, x));
    y = std::max(0, std::min(mHeight - 1, y));

    return getPixel(x, y);
}

Vec4<float> Texture::sampleLinear(Vec2<float> uv) const
{
    if (mWrapU == TextureWrap::REPEAT)
    {
        uv.x = uv.x - std::floor(uv.x);
    }
    else
    {
        uv.x = std::max(0.0f, std::min(1.0f, uv.x));
    }

    if (mWrapV == TextureWrap::REPEAT)
    {
        uv.y = uv.y - std::floor(uv.y);
    }
    else
    {
        uv.y = std::max(0.0f, std::min(1.0f, uv.y));
    }

    float u = uv.x * mWidth - 0.5f;
    float v = uv.y * mHeight - 0.5f;

    return getPixelBilinear(u, v);
}

Vec4<float> Texture::sampleClamp(Vec2<float> uv) const
{
    uv.x = std::max(0.0f, std::min(1.0f, uv.x));
    uv.y = std::max(0.0f, std::min(1.0f, uv.y));
    return sample(uv);
}

Vec4<float> Texture::sampleRepeat(Vec2<float> uv) const
{
    uv.x = uv.x - std::floor(uv.x);
    uv.y = uv.y - std::floor(uv.y);
    return sample(uv);
}

Vec4<float> Texture::getPixelBilinear(float u, float v) const
{
    float x0 = std::floor(u);
    float y0 = std::floor(v);
    float x1 = x0 + 1.0f;
    float y1 = y0 + 1.0f;

    float sx = u - x0;
    float sy = v - y0;

    int ix0 = static_cast<int>(x0);
    int iy0 = static_cast<int>(y0);
    int ix1 = static_cast<int>(x1);
    int iy1 = static_cast<int>(y1);

    ix0 = std::max(0, std::min(mWidth - 1, ix0));
    iy0 = std::max(0, std::min(mHeight - 1, iy0));
    ix1 = std::max(0, std::min(mWidth - 1, ix1));
    iy1 = std::max(0, std::min(mHeight - 1, iy1));

    Vec4<float> p00 = getPixel(ix0, iy0);
    Vec4<float> p10 = getPixel(ix1, iy0);
    Vec4<float> p01 = getPixel(ix0, iy1);
    Vec4<float> p11 = getPixel(ix1, iy1);

    Vec4<float> row0 = p00 * (1.0f - sx) + p10 * sx;
    Vec4<float> row1 = p01 * (1.0f - sx) + p11 * sx;

    return row0 * (1.0f - sy) + row1 * sy;
}

Vec4<float> Texture::getPixel(int x, int y) const
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
    {
        return Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f);
    }

    int idx = (y * mWidth + x) * mChannels;

    float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;

    if (mChannels >= 1) r = static_cast<float>(mData[idx]) / 255.0f;
    if (mChannels >= 2) g = static_cast<float>(mData[idx + 1]) / 255.0f;
    if (mChannels >= 3) b = static_cast<float>(mData[idx + 2]) / 255.0f;
    if (mChannels >= 4) a = static_cast<float>(mData[idx + 3]) / 255.0f;

    return Vec4<float>(r, g, b, a);
}
