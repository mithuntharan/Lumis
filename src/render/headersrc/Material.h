#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "Vector3.h"
#include "Ray.h"
#include "HitRecord.h"

namespace Lumis{
    class Material{
    public:
        Material() = default;
        virtual ~Material() = default;

        virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const = 0;
    };

    class Lambertian : public Material{
    private:
        Vector3 m_albedo;

    public:
        explicit Lambertian(const Vector3& albedo);

        bool scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override;
    };

    class Metal : public Material{
    private:
        Vector3 m_albedo;
        float m_fuzz;

    public:
        Metal(const Vector3& albedo, float fuzz = 0.0f);

        bool scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override;
    };

    class Plastic : public Material{
    private:
        Vector3 m_albedo;
        float m_fuzz;
        float m_ior;

    public:
        Plastic(const Vector3& albedo, float fuzz = 0.05f, float ior = 1.5f);

        bool scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override;
    };

    class Dielectric : public Material{
    private:
        float m_ir;

    public:
        explicit Dielectric(float indexOfRefraction);

        bool scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override;
    };
}

#endif //RAYTRACING_MATERIAL_H
