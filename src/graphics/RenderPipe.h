#pragma once
#include "Rasterizer.h"
#include "Camera.h"

/*
渲染管线
包含：1、光栅化
     2、shader管理

*/
class RenderPipe
{
public:
    RenderPipe();
    ~RenderPipe();
    void render();
    FrameBuffer* getFrameBuffer();
    void initialize(int width, int height);
    Rasterizer* getRasterizer()
    {
        return mRasterizer;
    }
private:
    Rasterizer *mRasterizer{ nullptr };
    Camera *mCamera{ nullptr };
    
};
