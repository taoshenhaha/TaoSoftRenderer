#include "RenderPipe.h"
#include "Program.hpp"
#include "Mat4.hpp"
#include "utils.h"
#include <iostream>

RenderPipe::RenderPipe()
{
}
RenderPipe::~RenderPipe()
{
}

void RenderPipe::initialize(int width, int height)
{
    mRasterizer = new Rasterizer(new FrameBuffer(width, height));
    mCamera = new Camera({ 0.0f, 1.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

    // 创建着色器
    std::shared_ptr<Material> material = std::make_shared<Material>();
    // 创建光源
    std::shared_ptr<Light> light = std::make_shared<Light>();
    light->position = Vec3<float>(10.0f, 10.0f, 10.0f);
    light->color = Vec3<float>(1.0f, 1.0f, 1.0f);
    light->specular = Vec3<float>(1.0f, 1.0f, 1.0f);
    light->ambient = Vec3<float>(0.2f, 0.2f, 0.2f);
    light->diffuse = Vec3<float>(0.5f, 0.5f, 0.5f);

    //设置材质
    material->diffuse_texture = Texture::createFromFile("/Users/bigo/Desktop/jianli/TaoSoftRenderer/assets/container2.tga", TextureUsage::LDR_COLOR);
    material->specular_texture = Texture::createFromFile("/Users/bigo/Desktop/jianli/TaoSoftRenderer/assets/container2_specular.tga", TextureUsage::LDR_COLOR);
    material->shininess = 32.0f;
    mProgram = std::make_shared<BlinnPhongProgram>(material, light);

    // 初始化立方体顶点数据
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    for (int i = 0; i < 288; i++) {
        mVertices[i] = vertices[i];
    }
}
static float origin = 0.0f;
static float lastFrame = 0.0f;
static float deltaTime = 0.0f;
static bool isfirstframe = true;
void RenderPipe::render()
{
    float currentFrame = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() / 1000.0f;
    if(isfirstframe) {
        isfirstframe = false;
        lastFrame = currentFrame;   
    } else {
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
    
    mFPSCounter.update();
    drawFPS();
    float modelmovespeed = static_cast<float>(5 * deltaTime);
    

    float eye_fov = 45.0f;
    float aspect_ratio = (float)mRasterizer->getFramebuffer()->mWidth / mRasterizer->getFramebuffer()->mHeight;
    float zNear = 0.1f;
    float zFar = 50.0f;
    //先使用mvp矩阵乘法，将顶点坐标转换为屏幕坐标
    // origin += modelmovespeed;
    float angle = -origin * MY_PI / 180.0f; // 绕X轴逆时针旋转90度
    float c = (float)cos(angle);
    float s = (float)sin(angle);
    Mat4<float> mModelMatrix;
    mModelMatrix.m[1][1] = c;
    mModelMatrix.m[1][2] = -s;
    mModelMatrix.m[2][1] = s;
    mModelMatrix.m[2][2] = c;
    Mat4<float> mvp = mCamera->getProjectionMatrix(eye_fov, aspect_ratio, zNear, zFar) * mCamera->getViewMatrix() * mModelMatrix;
    mRasterizer->clearColor({ 0.2f, 0.2f, 0.3f, 1.0f });
    mRasterizer->clearDepth(zFar);
    
    // 如果设置了着色器，使用着色器绘制额外的三角形
    if (!mProgram) {
        return;
    }
    auto program = mProgram;
    BlinnPhongProgram* colorProgram = dynamic_cast<BlinnPhongProgram*>(program.get());
    if (colorProgram) {
        // 设置着色器到光栅化器
        mRasterizer->setProgram(program);
        
        // 设置 Uniform
        program->setUniform("mvp", mvp);
        program->setUniform("model", mModelMatrix);
        program->setUniform("view", mCamera->getViewMatrix());
        program->setUniform("lightPosition", Vec3<float>(0, 5.0f, 4.0f));
        program->setUniform("cameraPosition", mCamera->getPosition()); 
        program->setUniform("projection", mCamera->getProjectionMatrix(eye_fov, aspect_ratio, zNear, zFar));  


        // 使用着色器绘制立方体（36个顶点，12个三角形）
        blinn_attribs_t attributes[3];

        for (int i = 0; i < 12; i += 3) {
            Vec3<float> pos1 = { mVertices[i*8], mVertices[i*8+1], mVertices[i*8+2] };
            Vec3<float> pos2 = { mVertices[(i+1)*8], mVertices[(i+1)*8+1], mVertices[(i+1)*8+2] };
            Vec3<float> pos3 = { mVertices[(i+2)*8], mVertices[(i+2)*8+1], mVertices[(i+2)*8+2] };

            Vec3<float> normal1 = { mVertices[i*8+3], mVertices[i*8+4], mVertices[i*8+5] };
            Vec3<float> normal2 = { mVertices[(i+1)*8+3], mVertices[(i+1)*8+4], mVertices[(i+1)*8+5] };
            Vec3<float> normal3 = { mVertices[(i+2)*8+3], mVertices[(i+2)*8+4], mVertices[(i+2)*8+5] };

            Vec2<float> tex1 = { mVertices[i*8+6], mVertices[i*8+7] };
            Vec2<float> tex2 = { mVertices[(i+1)*8+6], mVertices[(i+1)*8+7] };
            Vec2<float> tex3 = { mVertices[(i+2)*8+6], mVertices[(i+2)*8+7] };

            attributes[0].position = pos1;
            attributes[0].normal = normal1;
            attributes[0].texcoord = tex1;

            attributes[1].position = pos2;
            attributes[1].normal = normal2;
            attributes[1].texcoord = tex2;

            attributes[2].position = pos3;
            attributes[2].normal = normal3;
            attributes[2].texcoord = tex3;

            mRasterizer->drawTriangleWithProgram(attributes);
        }
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

static const unsigned char digit_3x5[10][5] = {
    {0x7, 0x5, 0x5, 0x5, 0x7},
    {0x2, 0x6, 0x2, 0x2, 0x7},
    {0x7, 0x1, 0x7, 0x4, 0x7},
    {0x7, 0x1, 0x7, 0x1, 0x7},
    {0x5, 0x5, 0x7, 0x1, 0x1},
    {0x7, 0x4, 0x7, 0x1, 0x7},
    {0x7, 0x4, 0x7, 0x5, 0x7},
    {0x7, 0x1, 0x2, 0x4, 0x4},
    {0x7, 0x5, 0x7, 0x5, 0x7},
    {0x7, 0x5, 0x7, 0x1, 0x7}
};

void RenderPipe::drawFPS()
{
    int fps = mFPSCounter.getFPS();
    if (fps <= 0) return;
    
    std::cout << "Drawing FPS: " << fps << std::endl;
    
    FrameBuffer* fb = mRasterizer->getFramebuffer();
    int startX = 10;
    int startY = 10;
    Vec4<float> color = {1.0f, 1.0f, 0.0f, 1.0f};
    
    if (fps >= 100) {
        int hundreds = fps / 100;
        int tens = (fps / 10) % 10;
        int ones = fps % 10;
        
        for (int d = 0; d < 3; ++d) {
            int digit = (d == 0) ? hundreds : (d == 1) ? tens : ones;
            for (int row = 0; row < 5; ++row) {
                unsigned char pattern = digit_3x5[digit][row];
                for (int col = 0; col < 3; ++col) {
                    if (pattern & (1 << (2 - col))) {
                        fb->setPixel(startX + d * 4 + col, startY + row, color);
                    }
                }
            }
        }
    } else if (fps >= 10) {
        int tens = fps / 10;
        int ones = fps % 10;
        
        for (int d = 0; d < 2; ++d) {
            int digit = (d == 0) ? tens : ones;
            for (int row = 0; row < 5; ++row) {
                unsigned char pattern = digit_3x5[digit][row];
                for (int col = 0; col < 3; ++col) {
                    if (pattern & (1 << (2 - col))) {
                        fb->setPixel(startX + d * 4 + col, startY + row, color);
                    }
                }
            }
        }
    } else {
        for (int row = 0; row < 5; ++row) {
            unsigned char pattern = digit_3x5[fps][row];
            for (int col = 0; col < 3; ++col) {
                if (pattern & (1 << (2 - col))) {
                    fb->setPixel(startX + col, startY + row, color);
                }
            }
        }
    }
}
