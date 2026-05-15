#pragma once
#include "Vec3.hpp"
struct Light {
    Vec3<float> background;
    char environment[LINE_SIZE];
    char skybox[LINE_SIZE];
    char shadow[LINE_SIZE];
    float ambient;
    float punctual;
    
};