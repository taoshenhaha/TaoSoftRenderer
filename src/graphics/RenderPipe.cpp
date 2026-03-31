#include "RenderPipe.h"
RenderPipe::RenderPipe()
{
}
RenderPipe::~RenderPipe()
{
}

void RenderPipe::initialize(int width, int height)
{
    mRasterizer = new Rasterizer(new Framebuffer(width, height));
}

void RenderPipe::render()
{
    mRasterizer->clear_color({ 0.2f, 0.2f, 0.3f, 1.0f });
    mRasterizer->draw_line({ 300.0f, 588.0f, 0.0f }, { 100.0f, 10.0f, 0.0f });
}

Framebuffer* RenderPipe::getFrameBuffer()
{
    return mRasterizer->getFramebuffer();
}
