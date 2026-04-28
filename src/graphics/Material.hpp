#pragma once
#include "Vec3.hpp"
#include "Texture.h"


struct Material {
    Texture *diffuse_texture;
    Texture *specular_texture;
    float shininess;
};