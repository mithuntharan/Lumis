#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "Hittable.h"
#include "Vector3.h"

namespace Lumis{
    class Material;

    class Sphere : public Hittable{
    private:
        Vector3 s_center;
        float s_radius;
        const Material* s_material;

    public:
        Sphere(const Vector3& center, float radius, const Material* material = nullptr);

        const Vector3& getCenter() const;
        float getRadius() const;

        bool hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const override;
    };
}

#endif //RAYTRACING_SPHERE_H
