#pragma once
#include "FrameBuffer.h"

class RenderPipe
{
public:
    RenderPipe();
    ~RenderPipe();
    void render();
    Framebuffer* getFrameBuffer();
    void initialize(int width, int height);
private:
    Framebuffer *mBuffer{ nullptr };
};
