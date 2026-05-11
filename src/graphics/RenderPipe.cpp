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
    mCamera = new Camera({ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

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
    material->diffuse_texture = Texture::createFromFile("/Users/bigo/Desktop/jianli/TaoSoftRenderer/assets/centaur/body_diffuse.tga", TextureUsage::LDR_COLOR);
    material->specular_texture = Texture::createFromFile("/Users/bigo/Desktop/jianli/TaoSoftRenderer/assets/centaur/body_specular.tga", TextureUsage::LDR_COLOR);
    material->shininess = 32.0f;
    mProgram = std::make_shared<BlinnPhongProgram>(material, light);

    // 加载模型
    mMesh = mesh_load("/Users/bigo/Desktop/jianli/TaoSoftRenderer/assets/centaur/body.obj");
    if (mMesh) {
        std::cout << "Loaded mesh: " << mesh_get_num_faces(mMesh) << " faces" << std::endl;
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
    float angle = origin * MY_PI / 180.0f; // 绕X轴逆时针旋转90度
    float c = (float)cos(angle);
    float s = (float)sin(angle);
    Mat4<float> mModelMatrix;

    // Mat4<float> translate = Mat4<float>::mat4Translate(-0.285f, 0.780f, 0.572f);
    // Mat4<float> rotation = Mat4<float>::mat4RotateY(TO_RADIANS(180));
    // Mat4<float> scale = Mat4<float>::mat4Scale(0.167f, 0.167f, 0.167f);
    // mModelMatrix =  scale * rotation * translate;

    Mat4<float> translation = Mat4<float>::mat4Translate(0.154f, -7.579f, -30.749f);
    Mat4<float> rotation_x = Mat4<float>::mat4RotateX(TO_RADIANS(-90));
    Mat4<float> rotation_y = Mat4<float>::mat4RotateY(TO_RADIANS(-60));
    Mat4<float> rotation = rotation_y * rotation_x;
    Mat4<float> scale = Mat4<float>::mat4Scale(0.016f, 0.016f, 0.016f);
    mModelMatrix = scale * rotation * translation;


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
        // 绘制加载的模型
        drawMesh();
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

void RenderPipe::drawMesh()
{
    if (!mMesh || !mProgram) return;
    
    auto program = mProgram;
    BlinnPhongProgram* colorProgram = dynamic_cast<BlinnPhongProgram*>(program.get());
    if (!colorProgram) return;
    
    vertex_t* vertices = mesh_get_vertices(mMesh);
    int numFaces = mesh_get_num_faces(mMesh);
    
    blinn_attribs_t attributes[3];
    
    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < 3; j++) {
            vertex_t& v = vertices[i * 3 + j];
            attributes[j].position = v.position;
            attributes[j].normal = v.normal;
            attributes[j].texcoord = v.texcoord;
        }
        mRasterizer->drawTriangleWithProgram(attributes);
    }
}
