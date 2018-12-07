#include "lib/telemetry-utility.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>

template<typename T, size_t N>
std::array<T, N> range() 
{
    std::array<T, N> a;
    T value{};
    for (size_t i = 0; i < N; ++i) a[i] = T{value++};
    return a;
}

int main(int argc, char const *argv[])
{
    fb::StartupTelemetry telemetry;

    int N = 10;
    for (int i = 0; i < N; ++i)
        telemetry.append({ "key-" + std::to_string(i), "value-" + std::to_string(i) });
    
    auto data = telemetry.collect();
    std::transform(data.begin(), data.end(), std::ostream_iterator<std::string>(std::cout, "\n"), [](const std::pair<std::string, std::string>& e) -> std::string {
        return e.first + " : " + e.second;
    });

    for (auto e : range<int, 10>())
        std::cout << e / 2 << ", ";

    std::cout << std::endl;

    return 0;
}