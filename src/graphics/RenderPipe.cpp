#include "RenderPipe.h"
#include "Program.hpp"

RenderPipe::RenderPipe()
{
}
RenderPipe::~RenderPipe()
{
}

void RenderPipe::initialize(int width, int height)
{
    mRasterizer = new Rasterizer(new FrameBuffer(width, height));
    mCamera = new Camera({ 0.0f, 0.0f, 4.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

    mProgram = new Program();
    mProgram->addShader(ShaderType::COLOR, std::make_shared<ColorShader>());
    
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
    // Vec4<float> vertex1 = { 2, 0, -10, 1.0f };
    // Vec4<float> vertex2 = { 0, 2, -10, 1.0f };
    // Vec4<float> vertex3 = { -2, 0, -10, 1.0f };
    // vertex1 = mvp * vertex1;
    // vertex2 = mvp * vertex2;
    // vertex3 = mvp * vertex3;
    
    // vertex1.x /= vertex1.w;
    // vertex1.y /= vertex1.w;

    // vertex2.x /= vertex2.w;
    // vertex2.y /= vertex2.w;

    // vertex3.x /= vertex3.w;
    // vertex3.y /= vertex3.w;

    // float f1 = (zFar - zNear) / 2.0;
    // float f2 = (zFar + zNear) / 2.0;

    // int width = mRasterizer->getFramebuffer()->mWidth;
    // int height = mRasterizer->getFramebuffer()->mHeight;
    // vertex1.x = 0.5*width*(vertex1.x+1.0);
    // vertex1.y = 0.5*height*(vertex1.y+1.0);
    // vertex1.z = vertex1.z * f1 + f2;
    // vertex2.x = 0.5*width*(vertex2.x+1.0);
    // vertex2.y = 0.5*height*(vertex2.y+1.0);
    // vertex2.z = vertex2.z * f1 + f2;
    // vertex3.x = 0.5*width*(vertex3.x+1.0);
    // vertex3.y = 0.5*height*(vertex3.y+1.0);
    // vertex3.z = vertex3.z * f1 + f2;
    // Vec3<float> vertex11 = { vertex1.x, vertex1.y, vertex1.z };
    // Vec3<float> vertex21 = { vertex2.x, vertex2.y, vertex2.z };
    // Vec3<float> vertex31 = { vertex3.x, vertex3.y, vertex3.z };

    // // 使用传统方式绘制三角形（无着色器）
    // mRasterizer->drawTriangle(vertex11, vertex21, vertex31);
    
    // 如果设置了着色器，使用着色器绘制额外的三角形
    if (!mProgram) {
        return;
    }
    auto shader = mProgram->getShader(ShaderType::COLOR);
    ColorShader* colorShader = dynamic_cast<ColorShader*>(shader.get());
    if (colorShader) {
        colorShader->setColor({ 1.0f, 1.0f, 0.0f });
    }
    if (shader) {
        // 设置着色器到光栅化器
        mRasterizer->setShader(shader);
        
        // 设置统一变量
        shader->setUniform("mvp", mvp);
        shader->setUniform("model", mModelMatrix);
        shader->setUniform("view", mCamera->getViewMatrix());
        shader->setUniform("projection", mCamera->getProjectionMatrix(eye_fov, aspect_ratio, zNear, zFar));
        
        // 使用着色器绘制另一个三角形（偏移位置）
        Vec3<float> P1 = { 2.0f, 0.0f, -5.0f };
        Vec3<float> P2 = { -2.0f, 0.0f, -5.0f };
        Vec3<float> P3 = { 0.0f, 2.0f, -5.0f };
        
        // 定义法线（用于法线着色器）
        Vec3<float> normal = { 0.0f, 0.0f, 1.0f };
        
        mRasterizer->drawTriangleWithShader(P1, P2, P3, normal, normal, normal);
    }
}

FrameBuffer* RenderPipe::getFrameBuffer()
{
    return mRasterizer->getFramebuffer();
}

void RenderPipe::useColorShader(const Vec3<float>& color) {
    auto shader = std::make_shared<ColorShader>(color);
    // mRasterizer->setShader(shader);
    std::cout << "使用颜色着色器 (颜色: " << color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
}
