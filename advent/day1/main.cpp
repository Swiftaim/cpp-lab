#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    std::ifstream file("E:/dev/c++/bazel_projects/lab/advent/day1/input.txt");
    std::string line;
    int sum{0};
    int rows{0};
    while (std::getline(file, line)) {
        auto num = std::stoi(line);
        sum += num;
        rows++;
    }
    std::cout << "rows: " << rows << ", sum: " << sum << std::endl;
    return 0;
}