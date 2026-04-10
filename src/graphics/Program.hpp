/*
 * @brief Program类
 * 用于管理渲染程序，包括顶点着色器、片段着色器等
 * @details 该类包含了顶点着色器、片段着色器等渲染程序组件，用于渲染场景
 * @note 该类依赖于Shader类，用于加载和编译着色器
 **/
#pragma once
#include "Shader.hpp"
class Program
{
public:
    Program();
    ~Program();
    void addShader(ShaderType type, std::string path);
    void link();
    void use();
    void draw();
private:
    std::vector<Shader*> mShaders;
};
#endif
