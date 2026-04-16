#pragma once
#include "Rasterizer.h"
#include "Camera.h"
#include "Program.hpp"
#include <memory>

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
    
    Rasterizer* getRasterizer() { return mRasterizer; }
    Camera* getCamera() { return mCamera; }
    
    
    // 创建预定义着色器
    void useColorShader(const Vec3<float>& color = Vec3<float>(1.0f, 0.0f, 0.0f));
    
private:
    Rasterizer *mRasterizer{ nullptr };
    Camera *mCamera{ nullptr };
    Program *mProgram{ nullptr };
};
