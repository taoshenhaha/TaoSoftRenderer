#pragma once

#include "BaseShader.hpp"
#include "../core/Vec3.hpp"
#include "../core/Vec4.hpp"
#include "../core/Mat4.hpp"
#include <string>
#include <vector>
#include <unordered_map>

/**
 * @brief Program类 - 管理着色器程序
 * @details 将多个着色器链接成完整的着色器程序，提供统一变量管理
 */
enum class ShaderType
 {
    COLOR,
    NORMAL,
    GEOMETRY,
    TEXTURE
};
class Program {

public:
    Program() = default;
    ~Program() = default;
    void addShader(ShaderType type, std::shared_ptr<BaseShader> shader);
    std::shared_ptr<BaseShader> getShader(ShaderType type);

private:
    std::unordered_map<ShaderType, std::shared_ptr<BaseShader>> mShaderMap;
};