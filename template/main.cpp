#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    auto start = std::chrono::steady_clock::now();

    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> int_dist(0, 1000);

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    
    std::cout << "Duration: " << duration.count() << " msec" << std::endl;
    
    return 0;
}