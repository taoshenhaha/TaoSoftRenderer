#pragma once
#include "Vec3.hpp"
#include "Mat4.hpp"

class Camera
{
public:
    Camera(Vec3<float> position, Vec3<float> target, Vec3<float> up);
    ~Camera();
    Mat4<float> getViewMatrix();
    Mat4<float> getProjectionMatrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar);
    Mat4<float> lookAt(Vec3<float> eye, Vec3<float> target, Vec3<float> up);
private:
    Vec3<float> mPosition{ 0.0f, 0.0f, 0.0f };
    Vec3<float> mTarget{ 0.0f, 0.0f, 1.0f };
    Vec3<float> mUp{ 0.0f, 1.0f, 0.0f };
};
