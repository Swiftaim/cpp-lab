#include "include/Vec3.h"

#include <iostream>

using namespace std;

void test_vec3()
{
    Vec3f v1(3, 4, 5);
    Vec3f v2(v1);
    Vec3f unit(1,1,1);
    
    normalize(v2);
    assert(v2.length() == v1.normalize().length() && v1.length() == 1);
    assert(dot(v1, v2) == 1);
    
    Vec3f u(1,0,0);
    Vec3f v(0,1,0);
    Vec3f w = cross(u, v);
    assert(dot(u, w) == 0);

    assert(w + v1 == v1 + w);
}

int main(int argc, char* argv[])
{
    test_vec3();

    return 0;
}