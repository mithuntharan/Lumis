#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include "Vector3.h"
#include "Ray.h"

namespace Lumis{
    class Camera{
    private:
        Vector3 c_position;
        Vector3 c_up = {0, 1, 0};
        Vector3 c_direction;

        float c_fov = 0; float c_aspectRatio = 0;

        Vector3 c_horizontal, c_vertical, c_bottomLeft;

    public:
        Camera(const Vector3& pos, const Vector3& dir, float fov, float asp, const Vector3& c_up = {0, 1, 0});

        Ray generateRay(float x, float y, int width, int height) const;

    };
} // Lumis

#endif //RAYTRACING_CAMERA_H
