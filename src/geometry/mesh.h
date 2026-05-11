#ifndef MESH_H
#define MESH_H

#include "Vec3.hpp"
#include "Vec2.hpp"
#include "Vec4.hpp"

typedef struct mesh mesh_t;

typedef struct {
    Vec3<float> position;
    Vec2<float> texcoord;
    Vec3<float> normal;
    Vec4<float> tangent;
    Vec4<float> joint;
    Vec4<float> weight;
} vertex_t;

mesh_t *mesh_load(const char *filename);
void mesh_release(mesh_t *mesh);

int mesh_get_num_faces(mesh_t *mesh);
vertex_t *mesh_get_vertices(mesh_t *mesh);
Vec3<float> mesh_get_center(mesh_t *mesh);

#endif
