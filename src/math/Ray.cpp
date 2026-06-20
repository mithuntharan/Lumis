
#include "Ray.h"

namespace Lumis{
    Vector3 Ray::findCord(const float t) const{
        return {origin + direction * t};
    }
}
