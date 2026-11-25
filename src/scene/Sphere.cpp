#include "headersrc/Sphere.h"
#include <cmath>

namespace Lumis{
    Sphere::Sphere(const Vector3& center, const float radius, const Material* material)
        : s_center(center), s_radius(radius), s_material(material) {}

    const Vector3& Sphere::getCenter() const{
        return s_center;
    }

    float Sphere::getRadius() const{
        return s_radius;
    }

    bool Sphere::hit(const Ray& ray, const float t_min, const float t_max, HitRecord& rec) const{
        const Vector3 oc = ray.origin - s_center;
        const float a = ray.direction.magnitudeSq();
        const float b = 2.0f * oc.dotProduct(ray.direction);
        const float c = oc.magnitudeSq() - s_radius * s_radius;

        const float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f){
            return false;
        }

        const float sqrtD = std::sqrt(discriminant);
        float root = (-b - sqrtD) / (2.0f * a);
        if (root <= t_min || root >= t_max){
            root = (-b + sqrtD) / (2.0f * a);
            if (root <= t_min || root >= t_max){
                return false;
            }
        }

        rec.t = root;
        rec.h_point = ray.findCord(rec.t);
        const Vector3 outwardNormal = (rec.h_point - s_center) / s_radius;
        rec.setFaceNormal(ray, outwardNormal);
        rec.m_material = s_material;
        return true;
    }
}
