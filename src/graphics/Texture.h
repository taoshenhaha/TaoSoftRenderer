#pragma once

#include "Graphics.h"
#include "../core/image.h"
#include <string>

enum class TextureUsage
{
    LDR_COLOR,
    LDR_DATA,
    HDR_COLOR,
    HDR_DATA
};

enum class TextureFilter
{
    NEAREST,
    LINEAR
};

class Texture
{
public:
    Texture();
    Texture(int width, int height);
    ~Texture();

    static Texture* createFromFile(const char* filename, TextureUsage usage);
    static Texture* createFromImage(image_t* image, TextureUsage usage);

    void release();

    void fromColorbuffer(void* framebuffer);
    void fromDepthbuffer(void* framebuffer);

    void setFilter(TextureFilter filter) { mFilter = filter; }
    TextureFilter getFilter() const { return mFilter; }

    Vec4<float> sampleRepeat(Vec2<float> texcoord) const;
    Vec4<float> sampleClamp(Vec2<float> texcoord) const;
    Vec4<float> sample(Vec2<float> texcoord) const;

    Vec4<float> sampleBilinearRepeat(Vec2<float> texcoord) const;
    Vec4<float> sampleBilinearClamp(Vec2<float> texcoord) const;
    Vec4<float> sampleBilinear(Vec2<float> texcoord) const;

    bool saveToFile(const char* filename);

private:
    Vec4<float> getPixel(int x, int y) const;

    int getWidth() const { return mWidth; }
    int getHeight() const { return mHeight; }
    Vec4<float>* getBuffer() { return mBuffer; }

private:
    int mWidth{ 0 };
    int mHeight{ 0 };
    Vec4<float>* mBuffer{ nullptr };
    TextureFilter mFilter{ TextureFilter::NEAREST };
};

class Cubemap
{
public:
    Cubemap();
    ~Cubemap();

    static Cubemap* createFromFiles(const char* positive_x, const char* negative_x,
                                    const char* positive_y, const char* negative_y,
                                    const char* positive_z, const char* negative_z,
                                    TextureUsage usage);

    void release();

    Vec4<float> sampleRepeat(Vec3<float> direction) const;
    Vec4<float> sampleClamp(Vec3<float> direction) const;
    Vec4<float> sample(Vec3<float> direction) const;

private:
    int selectFace(Vec3<float> direction, Vec2<float>& texcoord) const;

private:
    Texture* mFaces[6];
};
