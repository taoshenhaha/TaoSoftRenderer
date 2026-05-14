#pragma once

#include "../core/Vec2.hpp"
#include "../core/Vec3.hpp"
#include "../core/Vec4.hpp"
#include "../core/Mat4.hpp"
#include "../graphics/Texture.h"
#include "../graphics/Material.hpp"
#include "Light.hpp"
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
    BlinnPhongProgram(std::shared_ptr<Material> material, std::shared_ptr<Light> light) 
        : m_material(material), mLight(light) {}
    
    Vec4<float> vertexShader(blinn_attribs_t attribs) override {
        Mat4<float> mvp = getMatrixUniform("mvp");
        Mat4<float> model = getMatrixUniform("model");
        Mat3<float> normalModelMatrix = Mat4<float>::mat3_from_mat4(model);
        Mat3<float> normalModel = Mat3<float>::mat3_inverse_transpose(normalModelMatrix);


        Vec4<float> clipPosition = mvp * Vec4<float>(attribs.position, 1.0f);
        shader_varyings.world_position = (model * Vec4<float>(attribs.position, 1.0f)).toVec3();
        
        shader_varyings.normal = (normalModel * attribs.normal.normalize()).normalize();
        
        shader_varyings.texcoord = attribs.texcoord;
        shader_varyings.depth_position = clipPosition.toVec3();
        
        return clipPosition;
    }
    
    Vec4<float> fragmentShader(blinn_varyings_t varyings) override {

        if(!m_material || !mLight){
            return Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f);
        }
        Vec3<float> N = varyings.normal.normalize();
        
        Vec3<float> lightPos = getVectorUniform("lightPosition");
        Vec3<float> cameraPos = getVectorUniform("cameraPosition");
        
        Vec3<float> L = (lightPos - varyings.world_position).normalize();
        Vec3<float> V = (cameraPos - varyings.world_position).normalize();
        Vec3<float> H = (L + V).normalize(); //半程向量
        
        float NdotL = std::max(N.dot(L), 0.0f);
        float NdotH = std::max(N.dot(H), 0.0f);
        
        if(NdotL < 0.00001)
        {
            printf("ddd");
        }
        Vec3<float> ambient = mLight->ambient;
        
        // 漫反射
        Vec3<float> diffuseColor = Vec3<float>(1.0f, 1.0f, 1.0f);
        float alpha = 1.0f;
        if (m_material->diffuse_texture) {
            Vec4<float> texel = m_material->diffuse_texture->sample(varyings.texcoord);
            diffuseColor = Vec3<float>(texel.x, texel.y, texel.z);
            alpha = texel.w;
        }

        Vec3<float> diffuse = mLight->diffuse.hadamard(diffuseColor) * NdotL;
        // 镜面反射
        float spec = std::pow(NdotH, m_material->shininess);
        
        Vec3<float> specularColor = Vec3<float>(0.0f, 1.0f, 1.0f);
        
        if (m_material->specular_texture) {
            Vec4<float> texel = m_material->specular_texture->sample(varyings.texcoord);
            specularColor = Vec3<float>(texel.x, texel.y, texel.z);
        }
        Vec3<float> specular = mLight->specular.hadamard(specularColor) * spec;
        Vec3<float> result = /*mLight->ambient.hadamard(diffuseColor)  +*/ diffuse /*+ specular*/;
        if(result.x < 0.001 || result.y < 0.001 || result.z < 0.001)
        {
            // printf("ddd");
        }
        
        // return Vec4<float>(result, alpha);
        return Vec4<float>(NdotL, NdotL, NdotL, 1.0f);
    }
    
    void setMaterial(std::shared_ptr<Material> material) {
        m_material = material;
    }

public:
    std::shared_ptr<Material> m_material; // 材质
    std::shared_ptr<Light> mLight;
};