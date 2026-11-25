#include "headersrc/Camera.h"
#include <cmath>

namespace Lumis{
    constexpr float PI = 3.14159265358979323846f;

    Camera::Camera(const Vector3& pos, const Vector3& dir, const float fov, const float asp, const Vector3& c_up)
        :c_position(pos), c_direction(dir), c_fov(fov), c_aspectRatio(asp){
        Vector3 b = c_direction.normalized() * -1;
        Vector3 r = c_up.crossProduct(b).normalized();
        Vector3 u = b.crossProduct(r);

        // View port Calculations
        float halfHeight = std::tan((c_fov * PI / 180.0f) / 2.0f);
        float halfWidth = halfHeight * c_aspectRatio;

        c_horizontal = r * halfWidth * 2;
        c_vertical = u * halfHeight * 2;

        c_bottomLeft = c_position - c_horizontal / 2 - c_vertical / 2 - b;
    }

    Ray Camera::generateRay(const float x, const float y, const int width, const int height) const{
        const float s = x / static_cast<float>(width - 1);
        const float t = 1.0f - y / static_cast<float>(height - 1);

        const Vector3 pos = c_bottomLeft + c_horizontal * s + c_vertical * t;
        const Vector3 dir = (pos - c_position).normalized();

        return {c_position, dir};
    }
}
