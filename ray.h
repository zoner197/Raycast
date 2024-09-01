#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
        // Constructors
        ray() {}
        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}
        
        // Functions to return values of origin and direction to protect data integrity
        const point3& origin() const {return orig;}
        const vec3& direction() const {return dir;}

        // Function for linear interpolation (function that can find points on a ray based on it's origin and direction)
        point3 at(double t) const {return orig + t*dir;}
    private:
        point3 orig;
        vec3 dir;
};

#endif