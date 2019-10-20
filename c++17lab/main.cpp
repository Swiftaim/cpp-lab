#include <algorithm>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

void rand_func()
{
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> int_dist(0, 1000);
}

void structured_bindings()
{
    struct MyStruct {
        int i;
        std::string s;
    };
    
    MyStruct ms;
    auto [u, v] = ms;

    assert(u == ms.i);
    assert(v == ms.s);

    std::map<int, std::string> mymap{ {0, "a"}, {1, "b"}, {2, "c"} };
    for (const auto& [k, v] : mymap) {
        assert(mymap[k] == v);
    }
    std::cout << std::endl;
}

void range_test()
{
        using range_t = std::vector<int>;
    for (const auto& x : range_t{0, 1, 2, 3}) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

void test_filesystem()
{
    namespace fs = std::filesystem;
    fs::path path("Users/david/Dev/C++");
    std::cout << "exists: " << fs::exists(path);
}

void run(int argc, char* argv[])
{
    structured_bindings();
    range_test();
    test_filesystem();
}

int main(int argc, char* argv[])
{
    auto start = std::chrono::steady_clock::now();

    run(argc, argv);

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
    std::cout << "main duration: " << duration.count() << " usec" << std::endl;
    
    return 0;
}