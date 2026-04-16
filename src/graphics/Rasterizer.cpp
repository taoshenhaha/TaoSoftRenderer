#include "Rasterizer.h"
#include <cmath>
#include <cstdlib>

Rasterizer::~Rasterizer()
{
    delete mFrameBuffer;
    mFrameBuffer = nullptr;
}

void Rasterizer::drawLine(Vec3<float> p1, Vec3<float> p2)
{
    if (mFrameBuffer == nullptr)
    {
        return;
    }

    drawLineBresenham(p1, p2);
}

void Rasterizer::drawLineBresenham(Vec3<float> p1, Vec3<float> p2)
{ 
    // 使用四舍五入而不是截断，提高精度
    int x1 = (int)std::round(p1.x);
    int y1 = (int)std::round(p1.y);
    int x2 = (int)std::round(p2.x);
    int y2 = (int)std::round(p2.y);
    
    // 检查是否为同一个点
    if (x1 == x2 && y1 == y2)
    {
        setPixel(x1, y1, { 1.0f, 0.0f, 1.0f, 1.0f });
        return;
    }
    
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    
    // 处理特殊情况：垂直线
    if (dx == 0)
    {
        for (int y = y1; y != y2 + sy; y += sy)
        {
            setPixel(x1, y, { 1.0f, 0.0f, 1.0f, 1.0f });
        }
        return;
    }
    
    // 处理特殊情况：水平线
    if (dy == 0)
    {
        for (int x = x1; x != x2 + sx; x += sx)
        {
            setPixel(x, y1, { 1.0f, 0.0f, 1.0f, 1.0f });
        }
        return;
    }
    
    int err = dx - dy;
    
    // 根据斜率选择主要方向
    if (dx > dy)
    {
        // 斜率小于1，x为主要方向
        while (x1 != x2)
        {
            setPixel(x1, y1, { 1.0f, 0.0f, 1.0f, 1.0f });
            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y1 += sy;
            }
        }
    }
    else
    {
        // 斜率大于1，y为主要方向
        while (y1 != y2)
        {
            setPixel(x1, y1, { 1.0f, 0.0f, 1.0f, 1.0f });
            int e2 = 2 * err;
            if (e2 > -dx)
            {
                err -= dx;
                y1 += sy;
            }
            if (e2 < dy)
            {
                err += dy;
                x1 += sx;
            }
        }
    }
    
    // 绘制最后一个点
    setPixel(x2, y2, { 1.0f, 0.0f, 1.0f, 1.0f });
}
void Rasterizer::drawTriangle(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3)
{
    if (mFrameBuffer == nullptr)
    {
        return;
    }

    Vec2<float> p1_2 = { p1.x, p1.y };
    Vec2<float> p2_2 = { p2.x, p2.y };
    Vec2<float> p3_2 = { p3.x, p3.y };
    
    bbox_t bbox = boundingBox(p1_2, p2_2, p3_2);    

    for (int y = bbox.min_y; y <= bbox.max_y; y++)
    {
        for (int x = bbox.min_x; x <= bbox.max_x; x++)
        {
            Vec2<float> p = { (float)x + 0.5f, (float)y + 0.5f };
            Vec3<float> weight = calcuateWeight(p, p1_2, p2_2, p3_2);
            if (weight.x > 0.0f && weight.y > 0.0f && weight.z > 0.0f)
            {
                setPixel(x, y, { 1.0f, 1.0f, 1.0f, 1.0f });
            }
        }
    }
}
void Rasterizer::setPixel(int x, int y, Vec4<float> color)
{
    if (mFrameBuffer == nullptr)
    {
        return;
    }
    if (x < 0 || x >= mFrameBuffer->mWidth || y < 0 || y >= mFrameBuffer->mHeight)
    {
        return;
    }
    mFrameBuffer->setPixel(x, y, color);
}

void Rasterizer::clearColor(Vec4<float> color)
{
    if (mFrameBuffer == nullptr)
    {
        return;
    }
    mFrameBuffer->clearColor(color);
}

void Rasterizer::clearDepth(float depth)
{
    if (mFrameBuffer == nullptr)
    {
        return;
    }
    mFrameBuffer->clearDepth(depth);
}

bbox_t Rasterizer::boundingBox(Vec2<float> p1, Vec2<float> p2, Vec2<float> p3)
{ 
    float minX = std::min(std::min(p1.x, p2.x), p3.x);
    float minY = std::min(std::min(p1.y, p2.y), p3.y);
    float maxX = std::max(std::max(p1.x, p2.x), p3.x);
    float maxY = std::max(std::max(p1.y, p2.y), p3.y);

    bbox_t bbox;
    bbox.min_x = std::max((int)std::ceil(minX), 0);
    bbox.min_y = std::max((int)std::ceil(minY), 0);
    bbox.max_x = std::min((int)std::floor(maxX), mFrameBuffer->mWidth - 1);
    bbox.max_y = std::min((int)std::floor(maxY), mFrameBuffer->mHeight - 1);
    
    return bbox;
}

Vec3<float> Rasterizer::calcuateWeight(Vec2<float> p, Vec2<float> p1, Vec2<float> p2, Vec2<float> p3)
{
    Vec3<float> weight{ 0.0, 0.0, 0.0 };
    Vec2<float> a = p1;
    Vec2<float> b = p2;
    Vec2<float> c = p3;
    Vec2<float> ab = b - a;
    Vec2<float> ac = c - a;
    Vec2<float> ap = p - a;
    float factor = 1 / (ab.x * ac.y - ab.y * ac.x);
    float s = (ac.y * ap.x - ac.x * ap.y) * factor;
    float t = (ab.x * ap.y - ab.y * ap.x) * factor;
    weight = { 1 - s - t, s, t };
    return weight;
}

// 插值函数实现
Vec4<float> Rasterizer::interpolate(const Vec4<float>& a, const Vec4<float>& b, const Vec4<float>& c, const Vec3<float>& weights) {
    return a * weights.x + b * weights.y + c * weights.z;
}

Vec3<float> Rasterizer::interpolate(const Vec3<float>& a, const Vec3<float>& b, const Vec3<float>& c, const Vec3<float>& weights) {
    return a * weights.x + b * weights.y + c * weights.z;
}

Vec2<float> Rasterizer::interpolate(const Vec2<float>& a, const Vec2<float>& b, const Vec2<float>& c, const Vec3<float>& weights) {
    return a * weights.x + b * weights.y + c * weights.z;
}

// 支持着色器的三角形绘制
void Rasterizer::drawTriangleWithShader(const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3,
                                       const Vec3<float>& n1, const Vec3<float>& n2, const Vec3<float>& n3,
                                       const Vec2<float>& uv1, const Vec2<float>& uv2, const Vec2<float>& uv3) {
    if (mFrameBuffer == nullptr || m_currentShader == nullptr) {
        return;
    }
    
    // 应用顶点着色器
    Vec4<float> v1 = m_currentShader->vertexShader(p1, n1, uv1);
    Vec4<float> v2 = m_currentShader->vertexShader(p2, n2, uv2);
    Vec4<float> v3 = m_currentShader->vertexShader(p3, n3, uv3);
    
    // 透视除法
    if (v1.w != 0.0f) { v1 /= v1.w; }
    if (v2.w != 0.0f) { v2 /= v2.w; }
    if (v3.w != 0.0f) { v3 /= v3.w; }
    
    // 屏幕映射（从NDC [-1,1] 到屏幕坐标）
    float zNear = 0.1f;
    float zFar = 50.0f;
    float f1 = (zFar - zNear) / 2.0;
    float f2 = (zFar + zNear) / 2.0;

    int width = getFramebuffer()->mWidth;
    int height = getFramebuffer()->mHeight;
    v1.x = 0.5*width*(v1.x+1.0);
    v1.y = 0.5*height*(v1.y+1.0);
    v1.z = v1.z * f1 + f2;
    v2.x = 0.5*width*(v2.x+1.0);
    v2.y = 0.5*height*(v2.y+1.0);
    v2.z = v2.z * f1 + f2;
    v3.x = 0.5*width*(v3.x+1.0);
    v3.y = 0.5*height*(v3.y+1.0);
    v3.z = v3.z * f1 + f2;
    Vec2<float> vertex11 = { v1.x, v1.y };
    Vec2<float> vertex21 = { v2.x, v2.y };
    Vec2<float> vertex31 = { v3.x, v3.y };
    
    // 计算边界框
    bbox_t bbox = boundingBox(vertex11, vertex21, vertex31);
    
    // 光栅化
    for (int y = bbox.min_y; y <= bbox.max_y; y++) {
        for (int x = bbox.min_x; x <= bbox.max_x; x++) {
            Vec2<float> p = { (float)x + 0.5f, (float)y + 0.5f };
            Vec3<float> weights = calcuateWeight(p, vertex11, vertex21, vertex31);
            
            if (weights.x >= 0.0f && weights.y >= 0.0f && weights.z >= 0.0f) {
                // 插值顶点属性
                Vec2<float> position = interpolate(vertex11, vertex21, vertex31, weights);
                //暂时不用normal和texcoord
                // Vec3<float> normal = interpolate(n1, n2, n3, weights);
                // Vec2<float> texcoord = interpolate(uv1, uv2, uv3, weights);
                Vec3<float> position3 = { position.x, position.y, 0.0f };
                // 应用片段着色器
                Vec4<float> color = m_currentShader->fragmentShader(position3, {}, {});
                
                setPixel(x, y, color);
            }
        }
    }
}
