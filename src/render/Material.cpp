#include "headersrc/Material.h"
#include <algorithm>
#include <cmath>

namespace Lumis{
    namespace{
        float schlickReflectance(const float cosine, const float refIdx){
            float r0 = (1.0f - refIdx) / (1.0f + refIdx);
            r0 = r0 * r0;
            return r0 + (1.0f - r0) * std::pow(1.0f - cosine, 5.0f);
        }
    }

    Lambertian::Lambertian(const Vector3& albedo)
        : m_albedo(albedo) {}

    bool Lambertian::scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const{
        (void)rayIn;
        Vector3 scatterDir = rec.n_normal + Vector3::randomUnitVector();
        if (scatterDir.near_zero()){
            scatterDir = rec.n_normal;
        }
        scattered = Ray(rec.h_point, scatterDir.normalized());
        attenuation = m_albedo;
        return true;
    }

    Metal::Metal(const Vector3& albedo, const float fuzz)
        : m_albedo(albedo), m_fuzz(std::clamp(fuzz, 0.0f, 1.0f)) {}

    bool Metal::scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const{
        const Vector3 reflected = Vector3::reflectRay(rayIn.direction.normalized(), rec.n_normal);
        const Vector3 fuzzed = reflected + m_fuzz * Vector3::randomInUnitSphere();
        scattered = Ray(rec.h_point, fuzzed.normalized());
        attenuation = m_albedo;
        return scattered.direction.dotProduct(rec.n_normal) > 0.0f;
    }

    Plastic::Plastic(const Vector3& albedo, const float fuzz, const float ior)
        : m_albedo(albedo), m_fuzz(std::clamp(fuzz, 0.0f, 1.0f)), m_ior(ior) {}

    bool Plastic::scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const{
        const Vector3 unitDir = rayIn.direction.normalized();
        const float cosTheta = std::fmin((-unitDir).dotProduct(rec.n_normal), 1.0f);

        if (schlickReflectance(cosTheta, 1.0f / m_ior) > randomFloat()){
            const Vector3 reflected = Vector3::reflectRay(unitDir, rec.n_normal) + m_fuzz * Vector3::randomInUnitSphere();
            scattered = Ray(rec.h_point, reflected.normalized());
            attenuation = Vector3(1.0f, 1.0f, 1.0f);
            return scattered.direction.dotProduct(rec.n_normal) > 0.0f;
        }

        Vector3 scatterDir = rec.n_normal + Vector3::randomUnitVector();
        if (scatterDir.near_zero()){
            scatterDir = rec.n_normal;
        }
        scattered = Ray(rec.h_point, scatterDir.normalized());
        attenuation = m_albedo;
        return true;
    }

    Dielectric::Dielectric(const float indexOfRefraction)
        : m_ir(indexOfRefraction) {}

    bool Dielectric::scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const{
        attenuation = Vector3(1.0f, 1.0f, 1.0f);
        const float refractionRatio = rec.frontFace ? (1.0f / m_ir) : m_ir;

        const Vector3 unitDirection = rayIn.direction.normalized();
        const float cosTheta = std::fmin((-unitDirection).dotProduct(rec.n_normal), 1.0f);
        const float sinTheta = std::sqrt(std::fmax(0.0f, 1.0f - cosTheta * cosTheta));

        const bool cannotRefract = refractionRatio * sinTheta > 1.0f;
        Vector3 direction;

        if (cannotRefract || schlickReflectance(cosTheta, refractionRatio) > randomFloat()){
            direction = Vector3::reflectRay(unitDirection, rec.n_normal);
        } else{
            direction = Vector3::refract(unitDirection, rec.n_normal, refractionRatio);
        }

        scattered = Ray(rec.h_point, direction.normalized());
        return true;
    }
}
