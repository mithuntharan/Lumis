
#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H
#include <Vector3.h>

namespace Lumis{
    struct Ray{
        Vector3 origin;
        Vector3 direction;

        Ray() = default;
        Ray(const Vector3& ori, const Vector3& dir) : origin(ori), direction(dir){};

        Vector3 findCord(float t) const;
    };
}


#endif //RAYTRACING_RAY_H
