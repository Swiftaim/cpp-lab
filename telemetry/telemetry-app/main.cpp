#include "lib/telemetry-utility.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <exception>

template<size_t START, size_t STOP, size_t step=1>
std::array<size_t, (1>=(STOP-START)/step?1:(STOP-START)/step)> range() 
{
    std::array<size_t, (1>=(STOP-START)/step?1:(STOP-START)/step)> a;
    size_t index = 0;
    for (size_t i = START; i < STOP; i+=step) { a[index] = i; ++index; }
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

    for (auto e : range<0,5>())
        std::cout << e / 2 << ", ";

    std::cout << std::endl;

    return 0;
}