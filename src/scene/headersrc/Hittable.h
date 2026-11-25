#ifndef RAYTRACING_HITTABLE_H
#define RAYTRACING_HITTABLE_H

#include "Ray.h"
#include "HitRecord.h"

namespace Lumis{
    class Hittable{
    public:
        Hittable() = default;
        virtual ~Hittable() = default;

        virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const = 0;
    };
}

#endif //RAYTRACING_HITTABLE_H
