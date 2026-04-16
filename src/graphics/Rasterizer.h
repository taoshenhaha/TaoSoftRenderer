#pragma once
#include "FrameBuffer.h"
#include "BaseShader.hpp"
#include <memory>

/*
 * @brief Rasterizer类
 * 用于将3D模型转换为2D像素
 * @details 该类包含了所有的像素绘制代码，包括线段、三角形等
 * @note 该类依赖于Framebuffer类，用于存储渲染结果
*/

typedef struct {int min_x, min_y, max_x, max_y;} bbox_t;

class Rasterizer
{
public:
    Rasterizer(FrameBuffer *fb)
        : mFrameBuffer(fb)
    {
    }
    ~Rasterizer();
    FrameBuffer* getFramebuffer()
    {
        return mFrameBuffer;
    }
    void clearColor(Vec4<float> color);
    void clearDepth(float depth);
    bbox_t boundingBox(Vec2<float> p1, Vec2<float> p2, Vec2<float> p3);
    Vec3<float> calcuateWeight(Vec2<float> p, Vec2<float> p1, Vec2<float> p2, Vec2<float> p3);
    void drawLine(Vec3<float> p1, Vec3<float> p2);
    void drawTriangle(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3);
    void setPixel(int x, int y, Vec4<float> color);
    
    // 新增着色器相关方法
    void setShader(std::shared_ptr<BaseShader> shader) { m_currentShader = shader; }
    std::shared_ptr<BaseShader> getShader() const { return m_currentShader; }
    
    // 支持着色器的三角形绘制
    void drawTriangleWithShader(const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3,
                               const Vec3<float>& n1 = Vec3<float>(0, 0, 1), 
                               const Vec3<float>& n2 = Vec3<float>(0, 0, 1), 
                               const Vec3<float>& n3 = Vec3<float>(0, 0, 1),
                               const Vec2<float>& uv1 = Vec2<float>(0, 0),
                               const Vec2<float>& uv2 = Vec2<float>(0, 0),
                               const Vec2<float>& uv3 = Vec2<float>(0, 0));
    
private:
    void drawLineBresenham(Vec3<float> p1, Vec3<float> p2);
    Vec4<float> interpolate(const Vec4<float>& a, const Vec4<float>& b, const Vec4<float>& c, const Vec3<float>& weights);
    Vec3<float> interpolate(const Vec3<float>& a, const Vec3<float>& b, const Vec3<float>& c, const Vec3<float>& weights);
    Vec2<float> interpolate(const Vec2<float>& a, const Vec2<float>& b, const Vec2<float>& c, const Vec3<float>& weights);

private:
    FrameBuffer *mFrameBuffer{ nullptr };
    std::shared_ptr<BaseShader> m_currentShader;
};