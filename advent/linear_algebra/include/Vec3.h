#pragma once

#include <cassert>
#include <cmath>
#include <ostream>

template<typename T>
class Vec3
{
public:
    T x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) {}

    T lengthSqr() const { return x*x + y*y + z*z; }
    T length() const { return sqrt(x*x + y*y + z*z); }
    Vec3<T>& normalize() {
        auto len2 = lengthSqr();
        if (len2 > 0)
        {
            auto inv = 1 / sqrt(len2);
            x *= inv; y *= inv; z *= inv;
        }
        return *this;
    }

    Vec3<T> operator+(const Vec3<T>& v)
    {
        return Vec3<T>{ x + v.x, y + v.y, z + v.z };
    }

    Vec3<T> operator-(const Vec3<T>& v)
    {
        return Vec3<T>{ x - v.x, y - v.y, z - v.z };
    }

    Vec3<T> operator*(const T& r)
    {
        return Vec3<T>{ x * r, y * r, z * r };
    }

    bool operator==(const Vec3<T>& v)
    {
        return (x == v.x) && (y == v.y) && (z == v.z);
    }
};

template<typename T>
void normalize(Vec3<T>& v)
{
    auto len2 = v.lengthSqr();
    if (len2 > 0)
    {
        auto inv = 1 / sqrt(len2);
        v.x *= inv; v.y *= inv; v.z *= inv;
    }
}

template<typename T>
T dot(const Vec3<T>& v1, const Vec3<T>& v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template<typename T>
Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b)
{
    return Vec3<T>{
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

template<typename T>
std::ostream& operator<<(std::ostream& o, const Vec3<T>& v)
{
    return o << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
using Vec3i = Vec3<int>;
