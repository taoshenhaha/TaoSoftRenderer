#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "macro.h"
#include "mesh.h"
#include "Vec3.hpp"
#include "Vec2.hpp"

struct mesh {
    int num_faces;
    vertex_t *vertices;
    Vec3<float> center;
};

static mesh_t *build_mesh(
        Vec3<float> *positions, Vec2<float> *texcoords, Vec3<float> *normals,
        int *position_indices, int *texcoord_indices, int *normal_indices,
        int num_positions, int num_texcoords, int num_normals, int num_indices) {
    Vec3<float> bbox_min(+1e6f, +1e6f, +1e6f);
    Vec3<float> bbox_max(-1e6f, -1e6f, -1e6f);
    int num_faces = num_indices / 3;
    vertex_t *vertices;
    mesh_t *mesh;
    int i;

    assert(num_faces > 0 && num_faces * 3 == num_indices);

    vertices = (vertex_t*)malloc(sizeof(vertex_t) * num_indices);
    for (i = 0; i < num_indices; i++) {
        int position_index = position_indices[i];
        int texcoord_index = texcoord_indices[i];
        int normal_index = normal_indices[i];
        assert(position_index >= 0 && position_index < num_positions);
        assert(texcoord_index >= 0 && texcoord_index < num_texcoords);
        assert(normal_index >= 0 && normal_index < num_normals);
        vertices[i].position = positions[position_index];
        vertices[i].texcoord = texcoords[texcoord_index];
        vertices[i].normal = normals[normal_index];
        vertices[i].tangent = Vec4<float>(1, 0, 0, 1);
        vertices[i].joint = Vec4<float>(0, 0, 0, 0);
        vertices[i].weight = Vec4<float>(0, 0, 0, 0);

        bbox_min.x = std::min(bbox_min.x, vertices[i].position.x);
        bbox_min.y = std::min(bbox_min.y, vertices[i].position.y);
        bbox_min.z = std::min(bbox_min.z, vertices[i].position.z);
        bbox_max.x = std::max(bbox_max.x, vertices[i].position.x);
        bbox_max.y = std::max(bbox_max.y, vertices[i].position.y);
        bbox_max.z = std::max(bbox_max.z, vertices[i].position.z);
    }

    mesh = (mesh_t*)malloc(sizeof(mesh_t));
    mesh->num_faces = num_faces;
    mesh->vertices = vertices;
    mesh->center = (bbox_min + bbox_max) / 2.0f;

    return mesh;
}

static mesh_t *load_obj(const char *filename) {
    const int MAX_LINE_SIZE = 256;
    const int MAX_VERTS = 20000;
    const int MAX_INDICES = 100000;

    Vec3<float> *positions = (Vec3<float>*)malloc(sizeof(Vec3<float>) * MAX_VERTS);
    Vec2<float> *texcoords = (Vec2<float>*)malloc(sizeof(Vec2<float>) * MAX_VERTS);
    Vec3<float> *normals = (Vec3<float>*)malloc(sizeof(Vec3<float>) * MAX_VERTS);

    int *position_indices = (int*)malloc(sizeof(int) * MAX_INDICES);
    int *texcoord_indices = (int*)malloc(sizeof(int) * MAX_INDICES);
    int *normal_indices = (int*)malloc(sizeof(int) * MAX_INDICES);

    int num_positions = 0;
    int num_texcoords = 0;
    int num_normals = 0;
    int num_indices = 0;

    char line[MAX_LINE_SIZE];
    FILE *file;

    file = fopen(filename, "r");
    if (!file) {
        free(positions);
        free(texcoords);
        free(normals);
        free(position_indices);
        free(texcoord_indices);
        free(normal_indices);
        return nullptr;
    }

    while (fgets(line, MAX_LINE_SIZE, file) != nullptr) {
        if (strncmp(line, "v ", 2) == 0) {
            Vec3<float> position;
            if (sscanf(line + 2, "%f %f %f", &position.x, &position.y, &position.z) == 3) {
                assert(num_positions < MAX_VERTS);
                positions[num_positions++] = position;
            }
        } else if (strncmp(line, "vt ", 3) == 0) {
            Vec2<float> texcoord;
            if (sscanf(line + 3, "%f %f", &texcoord.x, &texcoord.y) == 2) {
                assert(num_texcoords < MAX_VERTS);
                texcoords[num_texcoords++] = texcoord;
            }
        } else if (strncmp(line, "vn ", 3) == 0) {
            Vec3<float> normal;
            if (sscanf(line + 3, "%f %f %f", &normal.x, &normal.y, &normal.z) == 3) {
                assert(num_normals < MAX_VERTS);
                normals[num_normals++] = normal;
            }
        } else if (strncmp(line, "f ", 2) == 0) {
            int pos_indices[3], uv_indices[3], n_indices[3];
            int count = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                               &pos_indices[0], &uv_indices[0], &n_indices[0],
                               &pos_indices[1], &uv_indices[1], &n_indices[1],
                               &pos_indices[2], &uv_indices[2], &n_indices[2]);
            if (count == 9) {
                for (int i = 0; i < 3; i++) {
                    assert(num_indices < MAX_INDICES);
                    position_indices[num_indices] = pos_indices[i] - 1;
                    texcoord_indices[num_indices] = uv_indices[i] - 1;
                    normal_indices[num_indices] = n_indices[i] - 1;
                    num_indices++;
                }
            }
        }
    }
    fclose(file);

    mesh_t *mesh = build_mesh(positions, texcoords, normals,
                              position_indices, texcoord_indices, normal_indices,
                              num_positions, num_texcoords, num_normals, num_indices);

    free(positions);
    free(texcoords);
    free(normals);
    free(position_indices);
    free(texcoord_indices);
    free(normal_indices);

    return mesh;
}

mesh_t *mesh_load(const char *filename) {
    const char *extension = strrchr(filename, '.');
    if (extension && strcmp(extension, ".obj") == 0) {
        return load_obj(filename);
    }
    assert(0);
    return nullptr;
}

void mesh_release(mesh_t *mesh) {
    if (mesh) {
        free(mesh->vertices);
        free(mesh);
    }
}

int mesh_get_num_faces(mesh_t *mesh) {
    return mesh ? mesh->num_faces : 0;
}

vertex_t *mesh_get_vertices(mesh_t *mesh) {
    return mesh ? mesh->vertices : nullptr;
}

Vec3<float> mesh_get_center(mesh_t *mesh) {
    return mesh ? mesh->center : Vec3<float>(0, 0, 0);
}
