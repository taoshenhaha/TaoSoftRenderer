#include "RenderPipe.h"
#include "Program.hpp"
#include "Mat4.hpp"
#include "utils.h"

RenderPipe::RenderPipe()
{
}
RenderPipe::~RenderPipe()
{
}

void RenderPipe::initialize(int width, int height)
{
    mRasterizer = new Rasterizer(new FrameBuffer(width, height));
    mCamera = new Camera({ 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

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
    
}
static float origin = 0.0f;
static float lastFrame = 0.0f;
static float deltaTime = 0.0f;
static bool isfirstframe = true;
void RenderPipe::render()
{
    //这里可以使用shader进行渲染    
    float currentFrame = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() / 1000.0f;
    if(isfirstframe) {
        isfirstframe = false;
        lastFrame = currentFrame;   
        return;
    }
    
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
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


        // 使用着色器绘制另一个三角形（偏移位置）
        Vec3<float> P1 = { -2.0f, -2.0f, 0.0f };
        Vec3<float> P2 = { 2.0f, -2.0f, 0.0f };
        Vec3<float> P3 = { -2.0f, 2.0f, 0.0f };
        Vec2<float> texcoord1 = { 0.0f, 0.0f };
        Vec2<float> texcoord2 = { 1.0f, 0.0f };
        Vec2<float> texcoord3 = { 0.0f, 1.0f };

        // 定义法线（用于法线着色器）
        Vec3<float> normal = { 0.0f, 0.0f, 1.0f };
        
        blinn_attribs_t attributes[3];

        attributes[0].position = P1;
        attributes[0].texcoord = texcoord1;
        attributes[0].normal = normal;

        attributes[1].position = P2;
        attributes[1].texcoord = texcoord2;
        attributes[1].normal = normal;

        attributes[2].position = P3;
        attributes[2].texcoord = texcoord3;
        attributes[2].normal = normal;

        // program->setShaderAttribs(attributes);
        mRasterizer->drawTriangleWithProgram(attributes);

        Vec3<float> P4 = { -2.0f, 2.0f, 0.0f };
        Vec3<float> P5 = { 2.0f, -2.0f, 0.0f };
        Vec3<float> P6 = { 2.0f, 2.0f, 0.0f };
        Vec2<float> texcoord4 = { 0.0f, 1.0f };
        Vec2<float> texcoord5 = { 1.0f, 0.0f };
        Vec2<float> texcoord6 = { 1.0f, 1.0f };

        attributes[0].position = P4;
        attributes[0].texcoord = texcoord4;
        attributes[0].normal = normal;

        attributes[1].position = P5;
        attributes[1].texcoord = texcoord5;
        attributes[1].normal = normal;

        attributes[2].position = P6;
        attributes[2].texcoord = texcoord6;
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
