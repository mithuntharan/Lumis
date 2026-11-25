#include "headersrc/Scene.h"

namespace Lumis{
    void Scene::add(Hittable* object){
        if (object != nullptr){
            sc_objects.push_back(object);
        }
    }

    void Scene::clear(){
        sc_objects.clear();
    }

    bool Scene::hit(const Ray& ray, const float t_min, const float t_max, HitRecord& rec) const{
        HitRecord tempRec;
        bool hitAnything = false;
        float closestSoFar = t_max;

        for (const Hittable* object : sc_objects){
            if (object->hit(ray, t_min, closestSoFar, tempRec)){
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }
}
