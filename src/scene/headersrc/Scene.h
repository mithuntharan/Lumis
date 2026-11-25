#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <vector>
#include "Hittable.h"

namespace Lumis{
    class Scene : public Hittable{
    private:
        std::vector<Hittable*> sc_objects;

    public:
        Scene() = default;

        void add(Hittable* object);
        void clear();

        bool hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const override;
    };
}

#endif //RAYTRACING_SCENE_H
