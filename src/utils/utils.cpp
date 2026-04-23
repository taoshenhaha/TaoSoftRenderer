#include "utils.h"

unsigned char float_to_uchar(float value) 
{
    return (unsigned char)(value * 255);
}

int is_vertex_visible(Vec4<float> v) {
    return fabs(v.x) <= v.w && fabs(v.y) <= v.w && fabs(v.z) <= v.w;
}