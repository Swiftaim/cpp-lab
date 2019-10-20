#pragma once

#include "Vec3.h"

struct Ray
{
    Vec3f orig;
    Vec3f dir;
    float t_min, t_max;

    Ray() : orig(0, 0, 0), dir(0,0,-1), t_min(0.1), t_max(1000.0) {} 
    Ray(const Vec3f &o, const Vec3f &d) : orig(o), dir(d), t_min(0.1), t_max(1000.0) {}
};