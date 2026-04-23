#pragma once
#include "FrameBuffer.h"
#include "Program.hpp"
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
    bbox_t boundingBox(Vec2<float> screen_coords[3]);
    Vec3<float> calcuateWeight(Vec2<float> p, Vec2<float> p1, Vec2<float> p2, Vec2<float> p3);
    void drawLine(Vec3<float> p1, Vec3<float> p2);
    // void drawTriangle(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3);
    void setPixel(int x, int y, Vec4<float> color);
    
    // 新增着色器相关方法
    void setProgram(std::shared_ptr<BaseProgram> program) { m_currentProgram = program; }
    std::shared_ptr<BaseProgram> getProgram() const { return m_currentProgram; }
    
    // 支持程序的三角形绘制
    void drawTriangleWithProgram(blinn_attribs_t attributes[3]);

    // 新增裁剪三角形方法
    int clipTriangle();
    int rasterizeTriangle(Vec4<float> clip_coords[3], blinn_varyings_t varyings[3]);
private:
    void drawLineBresenham(Vec3<float> p1, Vec3<float> p2);
    Vec4<float> interpolate(const Vec4<float>& a, const Vec4<float>& b, const Vec4<float>& c, const Vec3<float>& weights);
    Vec3<float> interpolate(const Vec3<float>& a, const Vec3<float>& b, const Vec3<float>& c, const Vec3<float>& weights);
    Vec2<float> interpolate(const Vec2<float>& a, const Vec2<float>& b, const Vec2<float>& c, const Vec3<float>& weights);
    void interpolate_varyings(
        blinn_varyings_t src_varyings[3], 
        blinn_varyings_t &dst_varyings,
        Vec3<float> weights, 
        float recip_w[3]);
private:
    FrameBuffer *mFrameBuffer{ nullptr };
    std::shared_ptr<BaseProgram> m_currentProgram{ nullptr};
};