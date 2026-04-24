#include "Texture.h"
#include "utils.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <algorithm>

namespace
{

inline float floatFromUChar(unsigned char value)
{
    return static_cast<float>(value) / 255.0f;
}

inline float floatSRGB2Linear(float value)
{
    if (value <= 0.04045f)
        return value / 12.92f;
    return std::pow((value + 0.055f) / 1.055f, 2.4f);
}

inline float floatLinear2SRGB(float value)
{
    if (value <= 0.0031308f)
        return value * 12.92f;
    return 1.055f * std::pow(value, 1.0f / 2.4f) - 0.055f;
}

inline float floatACES(float value)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp1((value * (a * value + b)) / (value * (c * value + d) + e), 0.0f, 1.0f);
    
}

}

Texture::Texture()
{
}

Texture::Texture(int width, int height)
    : mWidth(width)
    , mHeight(height)
{
    mBuffer = new Vec4<float>[width * height];
}

Texture::~Texture()
{
    release();
}

Texture* Texture::createFromFile(const char* filename, TextureUsage usage)
{
    image_t* image = image_load(filename);
    if (!image)
        return nullptr;

    Texture* texture = createFromImage(image, usage);
    image_release(image);
    return texture;
}

Texture* Texture::createFromImage(image_t* image, TextureUsage usage)
{
    if (!image)
        return nullptr;

    Texture* texture = new Texture(image->width, image->height);

    int numPixels = image->width * image->height;
    bool isLDR = (image->format == FORMAT_LDR);

    for (int i = 0; i < numPixels; i++)
    {
        Vec4<float> texel(0.0f, 0.0f, 0.0f, 1.0f);

        if (isLDR)
        {
            unsigned char* pixel = &image->ldr_buffer[i * image->channels];
            if (image->channels == 1)
            {
                texel.x = texel.y = texel.z = floatFromUChar(pixel[0]);
            }
            else if (image->channels == 2)
            {
                texel.x = texel.y = texel.z = floatFromUChar(pixel[0]);
                texel.w = floatFromUChar(pixel[1]);
            }
            else if (image->channels == 3)
            {
                texel.x = floatFromUChar(pixel[0]);
                texel.y = floatFromUChar(pixel[1]);
                texel.z = floatFromUChar(pixel[2]);
            }
            else
            {
                texel.x = floatFromUChar(pixel[0]);
                texel.y = floatFromUChar(pixel[1]);
                texel.z = floatFromUChar(pixel[2]);
                texel.w = floatFromUChar(pixel[3]);
            }

            if (usage == TextureUsage::HDR_COLOR)
            {
                texel.x = floatSRGB2Linear(texel.x);
                texel.y = floatSRGB2Linear(texel.y);
                texel.z = floatSRGB2Linear(texel.z);
            }
        }
        else
        {
            float* pixel = &image->hdr_buffer[i * image->channels];
            if (image->channels == 1)
            {
                texel.x = texel.y = texel.z = pixel[0];
            }
            else if (image->channels == 2)
            {
                texel.x = texel.y = texel.z = pixel[0];
                texel.w = pixel[1];
            }
            else if (image->channels == 3)
            {
                texel.x = pixel[0];
                texel.y = pixel[1];
                texel.z = pixel[2];
            }
            else
            {
                texel.x = pixel[0];
                texel.y = pixel[1];
                texel.z = pixel[2];
                texel.w = pixel[3];
            }

            if (usage == TextureUsage::LDR_COLOR)
            {
                texel.x = floatLinear2SRGB(floatACES(texel.x));
                texel.y = floatLinear2SRGB(floatACES(texel.y));
                texel.z = floatLinear2SRGB(floatACES(texel.z));
            }
        }

        texture->mBuffer[i] = texel;
    }

    return texture;
}

void Texture::release()
{
    if (mBuffer)
    {
        delete[] mBuffer;
        mBuffer = nullptr;
    }
    mWidth = 0;
    mHeight = 0;
}

void Texture::fromColorbuffer(void* framebuffer)
{
}

void Texture::fromDepthbuffer(void* framebuffer)
{
}

Vec4<float> Texture::sampleRepeat(Vec2<float> texcoord) const
{
    float u = texcoord.x - std::floor(texcoord.x);
    float v = texcoord.y - std::floor(texcoord.y);
    int c = static_cast<int>((mWidth - 1) * u);
    int r = static_cast<int>((mHeight - 1) * v);
    int index = r * mWidth + c;
    return mBuffer[index];
}

Vec4<float> Texture::sampleClamp(Vec2<float> texcoord) const
{
    float u = clamp1(texcoord.x, 0.0f, 1.0f);
    float v = clamp1(texcoord.y, 0.0f, 1.0f);
    int c = static_cast<int>((mWidth - 1) * u);
    int r = static_cast<int>((mHeight - 1) * v);
    int index = r * mWidth + c;
    return mBuffer[index];
}

Vec4<float> Texture::sample(Vec2<float> texcoord) const
{
    return sampleRepeat(texcoord);
}

Cubemap::Cubemap()
{
    for (int i = 0; i < 6; i++)
    {
        mFaces[i] = nullptr;
    }
}

Cubemap::~Cubemap()
{
    release();
}

Cubemap* Cubemap::createFromFiles(const char* positive_x, const char* negative_x,
                                  const char* positive_y, const char* negative_y,
                                  const char* positive_z, const char* negative_z,
                                  TextureUsage usage)
{
    Cubemap* cubemap = new Cubemap();
    cubemap->mFaces[0] = Texture::createFromFile(positive_x, usage);
    cubemap->mFaces[1] = Texture::createFromFile(negative_x, usage);
    cubemap->mFaces[2] = Texture::createFromFile(positive_y, usage);
    cubemap->mFaces[3] = Texture::createFromFile(negative_y, usage);
    cubemap->mFaces[4] = Texture::createFromFile(positive_z, usage);
    cubemap->mFaces[5] = Texture::createFromFile(negative_z, usage);
    return cubemap;
}

void Cubemap::release()
{
    for (int i = 0; i < 6; i++)
    {
        if (mFaces[i])
        {
            delete mFaces[i];
            mFaces[i] = nullptr;
        }
    }
}

int Cubemap::selectFace(Vec3<float> direction, Vec2<float>& texcoord) const
{
    float absX = std::fabs(direction.x);
    float absY = std::fabs(direction.y);
    float absZ = std::fabs(direction.z);
    float ma, sc, tc;
    int faceIndex;

    if (absX > absY && absX > absZ)
    {
        ma = absX;
        if (direction.x > 0)
        {
            faceIndex = 0;
            sc = -direction.z;
            tc = -direction.y;
        }
        else
        {
            faceIndex = 1;
            sc = direction.z;
            tc = -direction.y;
        }
    }
    else if (absY > absZ)
    {
        ma = absY;
        if (direction.y > 0)
        {
            faceIndex = 2;
            sc = direction.x;
            tc = direction.z;
        }
        else
        {
            faceIndex = 3;
            sc = direction.x;
            tc = -direction.z;
        }
    }
    else
    {
        ma = absZ;
        if (direction.z > 0)
        {
            faceIndex = 4;
            sc = direction.x;
            tc = -direction.y;
        }
        else
        {
            faceIndex = 5;
            sc = -direction.x;
            tc = -direction.y;
        }
    }

    texcoord.x = (sc / ma + 1.0f) / 2.0f;
    texcoord.y = (tc / ma + 1.0f) / 2.0f;
    return faceIndex;
}

Vec4<float> Cubemap::sampleRepeat(Vec3<float> direction) const
{
    Vec2<float> texcoord;
    int faceIndex = selectFace(direction, texcoord);
    texcoord.y = 1.0f - texcoord.y;
    return mFaces[faceIndex]->sampleRepeat(texcoord);
}

Vec4<float> Cubemap::sampleClamp(Vec3<float> direction) const
{
    Vec2<float> texcoord;
    int faceIndex = selectFace(direction, texcoord);
    texcoord.y = 1.0f - texcoord.y;
    return mFaces[faceIndex]->sampleClamp(texcoord);
}

Vec4<float> Cubemap::sample(Vec3<float> direction) const
{
    return sampleRepeat(direction);
}
