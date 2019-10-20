#include <chrono>
#include <iostream>
#include <random>

#include "include/geometry.h"

int main(int argc, char* argv[])
{
    auto start = std::chrono::steady_clock::now();

    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> int_dist(0, 1000);

    Vec3f v(0, 1, 2); 
    std::cerr << v << std::endl; 
    Matrix44f a, b, c; 
    c = a * b; 
 
    Matrix44f d(0.707107, 0, -0.707107, 0, -0.331295, 0.883452, -0.331295, 0, 0.624695, 0.468521, 0.624695, 0, 4.000574, 3.00043, 4.000574, 1); 
    std::cerr << d << std::endl; 
    d.invert(); 
    std::cerr << d << std::endl; 

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    
    std::cout << "Duration: " << duration.count() << " msec" << std::endl;
    
    return 0;
}