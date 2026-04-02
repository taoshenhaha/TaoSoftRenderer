#include "RenderPipe.h"
RenderPipe::RenderPipe()
{
}
RenderPipe::~RenderPipe()
{
}

void RenderPipe::initialize(int width, int height)
{
    mRasterizer = new Rasterizer(new FrameBuffer(width, height));
}

void RenderPipe::render()
{
    mRasterizer->clearColor({ 0.2f, 0.2f, 0.3f, 1.0f });
    // mRasterizer->drawLine({ 300.0f, 588.0f, 0.0f }, { 100.0f, 10.0f, 0.0f });
    mRasterizer->drawTriangle({ 300.0f, 588.0f, 0.0f }, { 100.0f, 10.0f, 0.0f }, { 500.0f, 500.0f, 0.0f });
}

FrameBuffer* RenderPipe::getFrameBuffer()
{
    return mRasterizer->getFramebuffer();
}
