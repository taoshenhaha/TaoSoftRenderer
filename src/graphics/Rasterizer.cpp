#include "Rasterizer.h"
#include "utils.h"

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
// void Rasterizer::drawTriangle(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3)
// {
//     if (mFrameBuffer == nullptr)
//     {
//         return;
//     }

//     Vec2<float> p1_2 = { p1.x, p1.y };
//     Vec2<float> p2_2 = { p2.x, p2.y };
//     Vec2<float> p3_2 = { p3.x, p3.y };
    
//     bbox_t bbox = boundingBox({ p1_2, p2_2, p3_2 });    

//     for (int y = bbox.min_y; y <= bbox.max_y; y++)
//     {
//         for (int x = bbox.min_x; x <= bbox.max_x; x++)
//         {
//             Vec2<float> p = { (float)x + 0.5f, (float)y + 0.5f };
//             Vec3<float> weight = calcuateWeight(p, p1_2, p2_2, p3_2);
//             if (weight.x > 0.0f && weight.y > 0.0f && weight.z > 0.0f)
//             {
//                 setPixel(x, y, { 1.0f, 1.0f, 1.0f, 1.0f });
//             }
//         }
//     }
// }
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

bbox_t Rasterizer::boundingBox(Vec2<float> screen_coords[3])
{ 
    Vec2<float> p1 = screen_coords[0];
    Vec2<float> p2 = screen_coords[1];
    Vec2<float> p3 = screen_coords[2];
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

/*
 * for perspective correct interpolation, see
 * https://www.comp.nus.edu.sg/~lowkl/publications/lowk_persp_interp_techrep.pdf
 * https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf
 *
 * equation 15 in reference 1 (page 2) is a simplified 2d version of
 * equation 3.5 in reference 2 (page 58) which uses barycentric coordinates
 */
void Rasterizer::interpolate_varyings(
        blinn_varyings_t src_varyings[3], 
        blinn_varyings_t &dst_varyings,
        Vec3<float> weights, 
        float recip_w[3]) 
{
    //透视矫正插值函数
    float weight0 = recip_w[0] * weights.x;
    float weight1 = recip_w[1] * weights.y;
    float weight2 = recip_w[2] * weights.z;
    float normalizer = 1 / (weight0 + weight1 + weight2);

    Vec3<float> sum = src_varyings[0].depth_position * weight0 + src_varyings[1].depth_position * weight1 + src_varyings[2].depth_position * weight2;
    dst_varyings.depth_position = sum * normalizer;
    
    Vec3<float> sum_normal = src_varyings[0].normal * weight0 + src_varyings[1].normal * weight1 + src_varyings[2].normal * weight2;
    dst_varyings.normal = sum_normal * normalizer;
    Vec2<float> sum_texcoord = src_varyings[0].texcoord * weight0 + src_varyings[1].texcoord * weight1 + src_varyings[2].texcoord * weight2;
    dst_varyings.texcoord = sum_texcoord * normalizer;
    
    Vec3<float> sum_world_position = src_varyings[0].world_position * weight0 + src_varyings[1].world_position * weight1 + src_varyings[2].world_position * weight2;
    dst_varyings.world_position = sum_world_position * normalizer;    
}

// 支持程序的三角形绘制
void Rasterizer::drawTriangleWithProgram(blinn_attribs_t attributes[3]) {
    if (mFrameBuffer == nullptr || m_currentProgram == nullptr) {
        return;
    }

    // 应用顶点着色器
    for(int i = 0; i < 3; i++) {
        Vec4<float> clip_position = m_currentProgram->vertexShader(attributes[i]);
        m_currentProgram->in_varyings[i] = m_currentProgram->shader_varyings;
        m_currentProgram->in_coords[i] = clip_position;
    }

    //clip三角形
    //todo
    int num_vertices = clipTriangle();
    if (num_vertices == 0) {
        return;
    }

    /* triangle assembly */
    for (int i = 0; i < num_vertices - 2; i++) {
        int index0 = 0;
        int index1 = i + 1;
        int index2 = i + 2;
        Vec4<float> clip_coords[3];
        blinn_varyings_t varyings[3];
        int is_culled;

        clip_coords[0] = m_currentProgram->out_coords[index0];
        clip_coords[1] = m_currentProgram->out_coords[index1];
        clip_coords[2] = m_currentProgram->out_coords[index2];
        varyings[0] = m_currentProgram->out_varyings[index0];
        varyings[1] = m_currentProgram->out_varyings[index1];
        varyings[2] = m_currentProgram->out_varyings[index2];

        is_culled = rasterizeTriangle(clip_coords, varyings);

                                       
        if (is_culled) {
            break;
        }
    }

}

/*
 * for triangle clipping, see
 * http://fabiensanglard.net/polygon_codec/
 * http://graphics.idav.ucdavis.edu/education/GraphicsNotes/Clipping.pdf
 */

static int is_back_facing(Vec3<float> ndc_coords[3]) {
    Vec3<float> a = ndc_coords[0];
    Vec3<float> b = ndc_coords[1];
    Vec3<float> c = ndc_coords[2];
    float signed_area = a.x * b.y - a.y * b.x +
                        b.x * c.y - b.y * c.x +
                        c.x * a.y - c.y * a.x;
    return signed_area <= 0;
}

enum class clip_plane_t : int 
{
    POSITIVE_W,
    POSITIVE_X,
    NEGATIVE_X,
    POSITIVE_Y,
    NEGATIVE_Y,
    POSITIVE_Z,
    NEGATIVE_Z
};

static int is_inside_plane(Vec4<float> coord, clip_plane_t plane) {
    switch (plane) {
        case clip_plane_t::POSITIVE_W:
            return coord.w >= EPSILON;
        case clip_plane_t::POSITIVE_X:
            return coord.x <= +coord.w;
        case clip_plane_t::NEGATIVE_X:
            return coord.x >= -coord.w;
        case clip_plane_t::POSITIVE_Y:
            return coord.y <= +coord.w;
        case clip_plane_t::NEGATIVE_Y:
            return coord.y >= -coord.w;
        case clip_plane_t::POSITIVE_Z:
            return coord.z <= +coord.w;
        case clip_plane_t::NEGATIVE_Z:
            return coord.z >= -coord.w;
        default:
            assert(0);
            return 0;
    }
}

static float get_intersect_ratio(Vec4<float> prev, Vec4<float> curr, clip_plane_t plane) {
    switch (plane) {
        case clip_plane_t::POSITIVE_W:
            return (prev.w - EPSILON) / (prev.w - curr.w);
        case clip_plane_t::POSITIVE_X:
            return (prev.w - prev.x) / ((prev.w - prev.x) - (curr.w - curr.x));
        case clip_plane_t::NEGATIVE_X:
            return (prev.w + prev.x) / ((prev.w + prev.x) - (curr.w + curr.x));
        case clip_plane_t::POSITIVE_Y:
            return (prev.w - prev.y) / ((prev.w - prev.y) - (curr.w - curr.y));
        case clip_plane_t::NEGATIVE_Y:
            return (prev.w + prev.y) / ((prev.w + prev.y) - (curr.w + curr.y));
        case clip_plane_t::POSITIVE_Z:
            return (prev.w - prev.z) / ((prev.w - prev.z) - (curr.w - curr.z));
        case clip_plane_t::NEGATIVE_Z:
            return (prev.w + prev.z) / ((prev.w + prev.z) - (curr.w + curr.z));
        default:
            assert(0);
            return 0;
    }
}

static int clip_against_plane(
        clip_plane_t plane, int in_num_vertices, int varying_num_floats,
        Vec4<float> (&in_coords)[MAX_VARYINGS], blinn_varyings_t (&in_varyings)[MAX_VARYINGS],
        Vec4<float> (&out_coords)[MAX_VARYINGS], blinn_varyings_t (&out_varyings)[MAX_VARYINGS]) {
    int out_num_vertices = 0;
    int i, j;

    assert(in_num_vertices >= 3 && in_num_vertices <= MAX_VARYINGS);
    for (i = 0; i < in_num_vertices; i++) {
        int prev_index = (i - 1 + in_num_vertices) % in_num_vertices;
        int curr_index = i;
        Vec4<float> &prev_coord = in_coords[prev_index];
        Vec4<float> &curr_coord = in_coords[curr_index];
        blinn_varyings_t &prev_varyings = in_varyings[prev_index];
        blinn_varyings_t &curr_varyings = in_varyings[curr_index];
        int prev_inside = is_inside_plane(prev_coord, plane);
        int curr_inside = is_inside_plane(curr_coord, plane);

        if (prev_inside != curr_inside) {
            Vec4<float> &dest_coord = out_coords[out_num_vertices];
            blinn_varyings_t &dest_varyings = out_varyings[out_num_vertices];
            float ratio = get_intersect_ratio(prev_coord, curr_coord, plane);

            dest_coord = prev_coord.lerp(curr_coord, ratio);
            /*
             * since this computation is performed in clip space before
             * division by w, clipped varying values are perspective-correct
             */

            dest_varyings.world_position = prev_varyings.world_position.lerp(curr_varyings.world_position, ratio);
            dest_varyings.depth_position = prev_varyings.depth_position.lerp(curr_varyings.depth_position, ratio);
            dest_varyings.texcoord = prev_varyings.texcoord.lerp(curr_varyings.texcoord, ratio);
            dest_varyings.normal = prev_varyings.normal.lerp(curr_varyings.normal, ratio);
            
            out_num_vertices += 1;
        }

        if (curr_inside) {
            Vec4<float> &dest_coord = out_coords[out_num_vertices];
            blinn_varyings_t &dest_varyings = out_varyings[out_num_vertices];
            int sizeof_varyings = varying_num_floats * sizeof(float);

            dest_coord = curr_coord;
            dest_varyings = curr_varyings;
            out_num_vertices += 1;
        }
    }
    assert(out_num_vertices <= MAX_VARYINGS);
    return out_num_vertices;
}

#define CLIP_IN2OUT(plane)                                                  \
    do {                                                                    \
        num_vertices = clip_against_plane(                                  \
            plane, num_vertices, varying_num_floats,                        \
            in_coords, in_varyings, out_coords, out_varyings);              \
        if (num_vertices < 3) {                                             \
            return 0;                                                       \
        }                                                                   \
    } while (0)

#define CLIP_OUT2IN(plane)                                                  \
    do {                                                                    \
        num_vertices = clip_against_plane(                                  \
            plane, num_vertices, varying_num_floats,                        \
            out_coords, out_varyings, in_coords, in_varyings);              \
        if (num_vertices < 3) {                                             \
            return 0;                                                       \
        }                                                                   \
    } while (0)


int Rasterizer::clipTriangle() {
    Vec4<float> (&in_coords)[MAX_VARYINGS] = m_currentProgram->in_coords;
    blinn_varyings_t (&in_varyings)[MAX_VARYINGS] = m_currentProgram->in_varyings;
    Vec4<float> (&out_coords)[MAX_VARYINGS] = m_currentProgram->out_coords;
    blinn_varyings_t (&out_varyings)[MAX_VARYINGS] = m_currentProgram->out_varyings; 

    int v0_visible = is_vertex_visible(in_coords[0]);
    int v1_visible = is_vertex_visible(in_coords[1]);
    int v2_visible = is_vertex_visible(in_coords[2]);
    if (v0_visible && v1_visible && v2_visible) {
        out_coords[0] = in_coords[0];
        out_coords[1] = in_coords[1];
        out_coords[2] = in_coords[2];
        out_varyings[0] = in_varyings[0];
        out_varyings[1] = in_varyings[1];
        out_varyings[2] = in_varyings[2];
        return 3;
    } else {
        int varying_num_floats = sizeof(blinn_varyings_t) / sizeof(float);
        int num_vertices = 3;
        CLIP_IN2OUT(clip_plane_t::POSITIVE_W);
        CLIP_OUT2IN(clip_plane_t::POSITIVE_X);
        CLIP_IN2OUT(clip_plane_t::NEGATIVE_X);
        CLIP_OUT2IN(clip_plane_t::POSITIVE_Y);
        CLIP_IN2OUT(clip_plane_t::NEGATIVE_Y);
        CLIP_OUT2IN(clip_plane_t::POSITIVE_Z);
        CLIP_IN2OUT(clip_plane_t::NEGATIVE_Z);
        return num_vertices;
    }
}

int Rasterizer::rasterizeTriangle(Vec4<float> clip_coords[3], blinn_varyings_t varyings[3]) 
{
   
/* reciprocals of w */
    float recip_w[3];
    Vec3<float> ndc_coords[3];
    Vec2<float> screen_coords[3];
    float screen_depth[3];
    for (int i = 0; i < 3; i++) {
        recip_w[i] = 1 / clip_coords[i].w;
    }

    // 屏幕映射（从NDC [-1,1] 到屏幕坐标）
    float zNear = 0.1f;
    float zFar = 50.0f;
    float f1 = (zFar - zNear) / 2.0;
    float f2 = (zFar + zNear) / 2.0;
    int width = getFramebuffer()->mWidth;
    int height = getFramebuffer()->mHeight;

    for (int i = 0; i < 3; i++) {
        ndc_coords[i] = clip_coords[i].toVec3() / clip_coords[i].w;
        screen_coords[i] = { (0.5f * ndc_coords[i].x + 0.5f) * width, (0.5f * ndc_coords[i].y + 0.5f) * height };
        screen_depth[i] = ndc_coords[i].z * f1 + f2;
    }
    // 检查是否为背面向
    if (is_back_facing(ndc_coords)) {
        return 1;
    }

    // 计算边界框
    bbox_t bbox = boundingBox(screen_coords);
    
    // 光栅化
    for (int y = bbox.min_y; y <= bbox.max_y; y++) {
        for (int x = bbox.min_x; x <= bbox.max_x; x++) {
            Vec2<float> p = { (float)x + 0.5f, (float)y + 0.5f };
            Vec3<float> weights = calcuateWeight(p, screen_coords[0], screen_coords[1], screen_coords[2]);
            
            if (weights.x >= 0.0f && weights.y >= 0.0f && weights.z >= 0.0f) {
                //插值深度
                float depth = interpolate1(screen_depth[0], screen_depth[1], screen_depth[2], weights);
                //深度测试
                if (mFrameBuffer->getDepth(x, y) < depth) {
                    continue;
                }
                // 更新深度缓冲区
                mFrameBuffer->setDepth(x, y, depth);
                
                blinn_varyings_t &out_varyings = m_currentProgram->shader_varyings;
                interpolate_varyings(varyings, out_varyings, weights, recip_w);
                // // 应用片段着色器
                Vec4<float> color = m_currentProgram->fragmentShader(out_varyings);
                setPixel(x, y, color);
            }
        }
    }
    return 0;
}