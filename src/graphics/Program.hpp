#pragma once

#include "../core/Vec2.hpp"
#include "../core/Vec3.hpp"
#include "../core/Vec4.hpp"
#include "../core/Mat4.hpp"
#include "../graphics/Texture.h"
#include <string>
#include <unordered_map>

#define MAX_VARYINGS 16
struct 
blinn_varyings_t // 顶点着色器输出的变量
{
    Vec3<float> world_position;
    Vec3<float> depth_position;
    Vec2<float> texcoord;
    Vec3<float> normal;
};

struct
blinn_attribs_t // 顶点着色器输入的变量
 {
    Vec3<float> position;
    Vec2<float> texcoord;
    Vec3<float> normal;
    Vec4<float> joint;
    Vec4<float> weight;
};

struct blinn_uniforms_t // 片段着色器输入的变量
{
    Mat4<float> mvp;
    Mat4<float> model;
    Mat4<float> view;
    Mat4<float> projection;
    Vec3<float> light_position;
    Vec3<float> camera_position;
    Vec3<float> light_color;
    Vec3<float> ambient_color;
    Vec3<float> diffuse_color;
};

/**
 * @brief BaseProgram类 - 基础程序基类
 * @details 提供统一的着色器接口，支持顶点和片段着色器阶段
 */
class BaseProgram {
public:
    virtual ~BaseProgram() = default;
    
    // 顶点着色器阶段
    virtual Vec4<float> vertexShader(blinn_attribs_t attribs) = 0;
    
    // 片段着色器阶段
    virtual Vec4<float> fragmentShader(blinn_varyings_t varyings) = 0;
    
    // 统一变量设置
    void setUniform(const std::string& name, const Mat4<float>& value) {
        m_matrixUniforms[name] = value;
    }
    
    void setUniform(const std::string& name, const Vec3<float>& value) {
        m_vectorUniforms[name] = value;
    }
    
    void setUniform(const std::string& name, const Vec4<float>& value) {
        m_vector4Uniforms[name] = value;
    }
    
    void setUniform(const std::string& name, float value) {
        m_floatUniforms[name] = value;
    }
    
    void setUniform(const std::string& name, int value) {
        m_intUniforms[name] = value;
    }
    
    // 获取统一变量
    Mat4<float> getMatrixUniform(const std::string& name) const {
        auto it = m_matrixUniforms.find(name);
        return it != m_matrixUniforms.end() ? it->second : Mat4<float>();
    }
    
    Vec3<float> getVectorUniform(const std::string& name) const {
        auto it = m_vectorUniforms.find(name);
        return it != m_vectorUniforms.end() ? it->second : Vec3<float>(0, 0, 0);
    }
    
    Vec4<float> getVector4Uniform(const std::string& name) const {
        auto it = m_vector4Uniforms.find(name);
        return it != m_vector4Uniforms.end() ? it->second : Vec4<float>(0, 0, 0, 0);
    }
    
    float getFloatUniform(const std::string& name) const {
        auto it = m_floatUniforms.find(name);
        return it != m_floatUniforms.end() ? it->second : 0.0f;
    }
    
    int getIntUniform(const std::string& name) const {
        auto it = m_intUniforms.find(name);
        return it != m_intUniforms.end() ? it->second : 0;
    }
    
    // 属性设置（用于顶点属性）
    void setAttribute(const std::string& name, int location) {
        m_attributeLocations[name] = location;
    }
    
    int getAttributeLocation(const std::string& name) const {
        auto it = m_attributeLocations.find(name);
        return it != m_attributeLocations.end() ? it->second : -1;
    }

    void setShaderAttribs(blinn_attribs_t attribs[3]) {
        for (int i = 0; i < 3; i++) {
            shader_attribs[i] = attribs[i];
        }
    }

public:
    blinn_attribs_t shader_attribs[3];
    blinn_varyings_t shader_varyings;
    void *shader_uniforms;
    /* for clipping */
    Vec4<float> in_coords[MAX_VARYINGS];
    Vec4<float> out_coords[MAX_VARYINGS];
    blinn_varyings_t in_varyings[MAX_VARYINGS];
    blinn_varyings_t out_varyings[MAX_VARYINGS];
    //测试texture
    Texture *m_texture{ nullptr };


protected:
    std::unordered_map<std::string, Mat4<float>> m_matrixUniforms;
    std::unordered_map<std::string, Vec3<float>> m_vectorUniforms;
    std::unordered_map<std::string, Vec4<float>> m_vector4Uniforms;
    std::unordered_map<std::string, float> m_floatUniforms;
    std::unordered_map<std::string, int> m_intUniforms;
    std::unordered_map<std::string, int> m_attributeLocations;
    
};

/**
 * @brief ColorShader类 - 简单颜色着色器
 */
class ColorProgram : public BaseProgram {
public:
    ColorProgram(const Vec3<float>& color = Vec3<float>(1.0f, 0.0f, 0.0f)) 
        : m_color(color) {}
    
    Vec4<float> vertexShader(blinn_attribs_t attribs) override {
        // 应用 MVP 变换
        Mat4<float> mvp = getMatrixUniform("mvp");
        Mat4<float> model = getMatrixUniform("model");
        Vec4<float> clip_position = mvp * Vec4<float>(attribs.position, 1.0f);
        // 计算插值变量
        shader_varyings.world_position = model * attribs.position;
        shader_varyings.normal = attribs.normal;
        shader_varyings.texcoord = attribs.texcoord; 
        shader_varyings.depth_position = clip_position.toVec3();

        return clip_position;
    }
    
    Vec4<float> fragmentShader(blinn_varyings_t varyings) override {

        // 从纹理采样颜色
        Vec4<float> texel = m_texture->sample(varyings.texcoord);
        return texel;
    }
    
    void setColor(const Vec3<float>& color) { m_color = color; }
    
private:
    Vec3<float> m_color;
};

/**
 * @brief BlinnPhongProgram类 - Blinn-Phong光照着色器
 */
class BlinnPhongProgram : public BaseProgram {
public:
    BlinnPhongProgram() 
        : mAmbientColor(0.1f, 0.1f, 0.1f)
        , mDiffuseColor(0.8f, 0.8f, 0.8f)
        , mSpecularColor(0.0f, 1.0f, 1.0f)
        , mShininess(128.0f) {}
    
    Vec4<float> vertexShader(blinn_attribs_t attribs) override {
        Mat4<float> mvp = getMatrixUniform("mvp");
        Mat4<float> model = getMatrixUniform("model");
        
        Vec4<float> clipPosition = mvp * Vec4<float>(attribs.position, 1.0f);
        shader_varyings.world_position = (model * Vec4<float>(attribs.position, 1.0f)).toVec3();
        
        shader_varyings.normal = (model * Vec4<float>(attribs.normal, 0.0f)).toVec3().normalize();
        
        shader_varyings.texcoord = attribs.texcoord;
        shader_varyings.depth_position = clipPosition.toVec3();
        
        return clipPosition;
    }
    
    Vec4<float> fragmentShader(blinn_varyings_t varyings) override {
        Vec3<float> N = varyings.normal.normalize();
        
        Vec3<float> lightPos = getVectorUniform("lightPosition");
        Vec3<float> cameraPos = getVectorUniform("cameraPosition");
        
        Vec3<float> L = (lightPos - varyings.world_position).normalize();
        Vec3<float> V = (cameraPos - varyings.world_position).normalize();
        Vec3<float> H = (L + V).normalize(); //半程向量
        
        float NdotL = std::max(N.dot(L), 0.0f);
        float NdotH = std::max(N.dot(H), 0.0f);
        
        Vec3<float> ambient = mAmbientColor;
        
        Vec3<float> diffuse = mDiffuseColor * NdotL;
        
        float spec = std::pow(NdotH, mShininess);
        Vec3<float> specular = mSpecularColor * spec * 0.5f;
        
        Vec3<float> baseColor = Vec3<float>(0.0f, 1.0f, 1.0f);
        float alpha = 1.0f;
        // if (m_texture) {
        //     Vec4<float> texel = m_texture->sample(varyings.texcoord);
        //     baseColor = Vec3<float>(texel.x, texel.y, texel.z);
        //     alpha = texel.w;
        // }
        
        Vec3<float> result = /*ambient.hadamard(baseColor) + diffuse.hadamard(baseColor) + */specular;
        // Vec3<float> result = ambient.hadamard(baseColor)+ diffuse.hadamard(baseColor) ;
        return Vec4<float>(result, alpha);
    }
    
    void setMaterial(const Vec3<float>& ambient, const Vec3<float>& diffuse, 
                    const Vec3<float>& specular, float shininess) {
        mAmbientColor = ambient;
        mDiffuseColor = diffuse;
        mSpecularColor = specular;
        mShininess = shininess;
    }
    
    void setTexture(Texture* texture) {
        m_texture = texture;
    }

private:
    Vec3<float> mAmbientColor;
    Vec3<float> mDiffuseColor;
    Vec3<float> mSpecularColor;
    float mShininess;
};