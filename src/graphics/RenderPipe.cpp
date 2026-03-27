#include "RenderPipe.h"
RenderPipe::RenderPipe()
{
}
RenderPipe::~RenderPipe()
{
}

void RenderPipe::initialize(int width, int height)
{
    mBuffer = new Framebuffer(width, height);
}

void RenderPipe::render()
{
    mBuffer->clear_color({ 0.0f, 0.0f, 1.0f, 1.0f });
}

Framebuffer* RenderPipe::getFrameBuffer()
{
    return mBuffer;
}
