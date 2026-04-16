#include "Program.hpp"

void Program::addShader(ShaderType type, std::shared_ptr<BaseShader> shader)
{
    mShaderMap[type] = shader;
}

std::shared_ptr<BaseShader> Program::getShader(ShaderType type)
{
    return mShaderMap[type];
}
