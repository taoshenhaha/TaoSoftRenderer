#include "FrameBuffer.h"
//所有的画直线以及画三角形的代码都写在这个类中，里面存有Framebuffer的指针
class Rasterizer
{
public:
    Rasterizer(Framebuffer *fb)
        : mFramebuffer(fb)
    {
    }
    ~Rasterizer();
    Framebuffer* getFramebuffer()
    {
        return mFramebuffer;
    }
    void clear_color(Vec4<float> color);
    void draw_line(Vec3<float> p1, Vec3<float> p2);
    void draw_triangle(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3);
    void set_pixel(int x, int y, Vec4<float> color);
private:
    void draw_line_bresenham(Vec3<float> p1, Vec3<float> p2);

private:
    Framebuffer *mFramebuffer{ nullptr };
};