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
    mCamera = new Camera({ 0.0f, 30.0f, 15.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
}

void RenderPipe::render()
{
    //这里可以使用shader进行渲染    

    float eye_fov = 45.0f;
    float aspect_ratio = (float)mRasterizer->getFramebuffer()->mWidth / mRasterizer->getFramebuffer()->mHeight;
    float zNear = 0.1f;
    float zFar = 50.0f;
    //先使用mvp矩阵乘法，将顶点坐标转换为屏幕坐标
    Mat4<float> mModelMatrix;
    Mat4<float> mvp = mCamera->getProjectionMatrix(eye_fov, aspect_ratio, zNear, zFar) * mCamera->getViewMatrix() * mModelMatrix;
    mRasterizer->clearColor({ 0.2f, 0.2f, 0.3f, 1.0f });
    // mRasterizer->drawLine({ 300.0f, 588.0f, 0.0f }, { 100.0f, 10.0f, 0.0f });
    // 这里可以使用mvp矩阵进行渲染
    Vec4<float> vertex1 = { 2, 0, -10, 1.0f };
    Vec4<float> vertex2 = { 0, 2, -10, 1.0f };
    Vec4<float> vertex3 = { -2, 0, -10, 1.0f };
    vertex1 = mvp * vertex1;
    vertex2 = mvp * vertex2;
    vertex3 = mvp * vertex3;
    
    vertex1.x /= vertex1.w;
    vertex1.y /= vertex1.w;

    vertex2.x /= vertex2.w;
    vertex2.y /= vertex2.w;

    vertex3.x /= vertex3.w;
    vertex3.y /= vertex3.w;

    float f1 = (zFar - zNear) / 2.0;
    float f2 = (zFar + zNear) / 2.0;

    int width = mRasterizer->getFramebuffer()->mWidth;
    int height = mRasterizer->getFramebuffer()->mHeight;
    vertex1.x = 0.5*width*(vertex1.x+1.0);
    vertex1.y = 0.5*height*(vertex1.y+1.0);
    vertex1.z = vertex1.z * f1 + f2;
    vertex2.x = 0.5*width*(vertex2.x+1.0);
    vertex2.y = 0.5*height*(vertex2.y+1.0);
    vertex2.z = vertex2.z * f1 + f2;
    vertex3.x = 0.5*width*(vertex3.x+1.0);
    vertex3.y = 0.5*height*(vertex3.y+1.0);
    vertex3.z = vertex3.z * f1 + f2;
    Vec3<float> vertex11 = { vertex1.x, vertex1.y, vertex1.z };
    Vec3<float> vertex21 = { vertex2.x, vertex2.y, vertex2.z };
    Vec3<float> vertex31 = { vertex3.x, vertex3.y, vertex3.z };

    mRasterizer->drawTriangle(vertex11, vertex21, vertex31);
}

FrameBuffer* RenderPipe::getFrameBuffer()
{
    return mRasterizer->getFramebuffer();
}
