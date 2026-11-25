#ifndef RAYTRACING_HITRECORD_H
#define RAYTRACING_HITRECORD_H

#include "Vector3.h"
#include "Ray.h"

namespace Lumis{
    class Material;

    struct HitRecord{
        Vector3 h_point;
        Vector3 n_normal;
        const Material* m_material = nullptr;
        float t = 0.0f;
        bool frontFace = true;

        HitRecord() = default;

        void setFaceNormal(const Ray& ray, const Vector3& outwardNormal){
            frontFace = ray.direction.dotProduct(outwardNormal) < 0.0f;
            n_normal = frontFace ? outwardNormal : -outwardNormal;
        }
    };
}

#endif //RAYTRACING_HITRECORD_H
