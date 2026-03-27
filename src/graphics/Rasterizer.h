#include "Framebuffer.h"
//所有的画直线以及画三角形的代码都写在这个类中，里面存有Framebuffer的指针
class Rasterizer
{
public:
    Rasterizer(Framebuffer *fb)
        : mFramebuffer(fb)
    {
    }
    ~Rasterizer()
    {
    }
private:
    Framebuffer *mFramebuffer{ nullptr };
};