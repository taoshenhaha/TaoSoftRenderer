#include "Camera.h"
#include "utils.h"
#include "Mat4.hpp"
Camera::Camera(Vec3<float> position, Vec3<float> target, Vec3<float> up)
{
    mPosition = position;
    mTarget = target;
    mUp = up;
}

Camera::~Camera()
{
}

Mat4<float> Camera::getViewMatrix()
{ 
    return lookAt(mPosition, mTarget, mUp);
}

/*
 * fovy: the field of view angle in the y direction, in radians
 * aspect: the aspect ratio, defined as width divided by height
 * near, far: the distances to the near and far depth clipping planes
 *
 * 1/(aspect*tan(fovy/2))              0             0           0
 *                      0  1/tan(fovy/2)             0           0
 *                      0              0  -(f+n)/(f-n)  -2fn/(f-n)
 *                      0              0            -1           0
 *
 * this is the same as
 *     float half_h = near * (float)tan(fovy / 2);
 *     float half_w = half_h * aspect;
 *     mat4_frustum(-half_w, half_w, -half_h, half_h, near, far);
 *
 * see http://www.songho.ca/opengl/gl_projectionmatrix.html
 */
Mat4<float> Camera::getProjectionMatrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Mat4<float> projection;

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.

    float eye_fov_radian = eye_fov * MY_PI / 180;

    float t = std::tan(eye_fov_radian / 2) * std::abs(zNear);
    float r = t * aspect_ratio;
    float l = -r;
    float b = -t;

    //相机空间是右手坐标系，但是NDC空间是左手坐标系，推导链接
    https://zhuanlan.zhihu.com/p/711297214，但是要注意这里的 最前面的缩放矩阵第三行第三列应该改成2 /(f - n)才能是正数
    projection.m[0][0] = 2 * zNear / (r - l);
    projection.m[0][1] = 0;
    projection.m[0][2] = (l + r) / (l - r);
    projection.m[0][3] = 0;

    projection.m[1][0] = 0;
    projection.m[1][1] = 2 * zNear / (t - b);
    projection.m[1][2] = (b + t) / (b - t);
    projection.m[1][3] = 0;

    projection.m[2][0] = 0;
    projection.m[2][1] = 0;
    projection.m[2][2] = -(zNear + zFar) / (zFar - zNear);
    projection.m[2][3] = -2 * zNear * zFar / (zFar - zNear);

    projection.m[3][0] = 0;
    projection.m[3][1] = 0;
    projection.m[3][2] = -1;
    projection.m[3][3] = 0;

    return projection;
}

Mat4<float> Camera::lookAt(Vec3<float> eye, Vec3<float> target, Vec3<float> up) 
{
    // 正确的lookAt矩阵计算
    // z轴：从目标指向相机的方向（相机看向-z方向）
    Vec3<float> z_axis = (eye - target).normalize();
    
    // x轴：up向量与z轴的叉积（右手坐标系）
    Vec3<float> x_axis = up.cross(z_axis).normalize();
    
    // y轴：z轴与x轴的叉积，确保正交
    Vec3<float> y_axis = z_axis.cross(x_axis);
    
    Mat4<float> m;

    // 旋转部分：将相机坐标系对齐到世界坐标系
    m.m[0][0] = x_axis.x;
    m.m[0][1] = y_axis.x;  // 注意：这里是转置矩阵
    m.m[0][2] = z_axis.x;
    m.m[0][3] = 0;

    m.m[1][0] = x_axis.y;
    m.m[1][1] = y_axis.y;
    m.m[1][2] = z_axis.y;
    m.m[1][3] = 0;

    m.m[2][0] = x_axis.z;
    m.m[2][1] = y_axis.z;
    m.m[2][2] = z_axis.z;
    m.m[2][3] = 0;

    // 平移部分：将相机移动到原点
    m.m[0][3] = -x_axis.dot(eye);
    m.m[1][3] = -y_axis.dot(eye);
    m.m[2][3] = -z_axis.dot(eye);
    m.m[3][3] = 1;

    return m;
}