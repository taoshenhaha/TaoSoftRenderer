#pragma once
#include "Rasterizer.h"
#include "Camera.h"
#include "Program.hpp"
#include <memory>
#include <chrono>

class FPSCounter
{
public:
    FPSCounter() : frameCount(0), lastTime(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()), fps(0) {}
    
    void update()
    {
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();
        
        if (elapsed - lastTime >= 1000)
        {
            fps = frameCount;
            frameCount = 0;
            lastTime = elapsed;
        }
    }
    
    int getFPS() const { return fps; }
    
private:
    int frameCount;
    long long lastTime;
    int fps;
};

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
    std::shared_ptr<BaseProgram> mProgram{ nullptr };
    FPSCounter mFPSCounter;
    
    void drawFPS();
    float mVertices[288];

};
