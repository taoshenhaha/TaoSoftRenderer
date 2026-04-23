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

    mProgram = std::make_shared<ColorProgram>();
    
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
    mRasterizer->clearDepth(zFar);
    
    // 如果设置了着色器，使用着色器绘制额外的三角形
    if (!mProgram) {
        return;
    }
    auto program = mProgram;
    ColorProgram* colorProgram = dynamic_cast<ColorProgram*>(program.get());
    if (colorProgram) {
        colorProgram->setColor({ 1.0f, 1.0f, 0.0f });
    }
    if (colorProgram) {
        // 设置着色器到光栅化器
        mRasterizer->setProgram(program);
        
        // 设置统一变量
        program->setUniform("mvp", mvp);
        program->setUniform("model", mModelMatrix);
        program->setUniform("view", mCamera->getViewMatrix());
        program->setUniform("projection", mCamera->getProjectionMatrix(eye_fov, aspect_ratio, zNear, zFar));
        
        // 使用着色器绘制另一个三角形（偏移位置）
        Vec3<float> P1 = { 2.0f, 0.0f, -5.0f };
        Vec3<float> P2 = { 0.0f, 2.0f, -5.0f };
        Vec3<float> P3 = { -2.0f, 0.0f, -5.0f };
        
        
        // 定义法线（用于法线着色器）
        Vec3<float> normal = { 0.0f, 0.0f, 1.0f };
        
        blinn_attribs_t attributes[3];

        attributes[0].position = P1;
        attributes[0].texcoord = { 0.0f, 0.0f };
        attributes[0].normal = normal;

        attributes[1].position = P2;
        attributes[1].texcoord = { 0.0f, 0.0f };
        attributes[1].normal = normal;

        attributes[2].position = P3;
        attributes[2].texcoord = { 0.0f, 0.0f };
        attributes[2].normal = normal;

        // program->setShaderAttribs(attributes);
        mRasterizer->drawTriangleWithProgram(attributes);


        if (colorProgram) {
            colorProgram->setColor({ 1.0f, 0.0f, 0.0f });
        }
        Vec3<float> P4 = { 4.0f, 0.0f, -10.0f };
        Vec3<float> P5 = { -4.0f, 0.0f, -10.0f };
        Vec3<float> P6 = { 0.0f, 4.0f, -10.0f };
        
        attributes[0].position = P4;
        attributes[0].texcoord = { 0.0f, 0.0f };
        attributes[0].normal = normal;

        attributes[1].position = P5;
        attributes[1].texcoord = { 0.0f, 0.0f };
        attributes[1].normal = normal;

        attributes[2].position = P6;
        attributes[2].texcoord = { 0.0f, 0.0f };
        attributes[2].normal = normal;

        mRasterizer->drawTriangleWithProgram(attributes);
    }
}

FrameBuffer* RenderPipe::getFrameBuffer()
{
    return mRasterizer->getFramebuffer();
}

void RenderPipe::useColorShader(const Vec3<float>& color) {
    auto program = std::make_shared<ColorProgram>(color);
    // mRasterizer->setProgram(program);
    std::cout << "使用颜色着色器 (颜色: " << color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
}
