#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int part1(std::ifstream& file)
{
    file.clear();
    file.seekg(0);

    std::string line;
    std::vector<std::string> input;
    while (getline(file, line)) {
        sort(line.begin(), line.end());
        input.push_back(line);
    }

    int twoCount = 0;
    int threeCount = 0;

    for (auto id : input) {
        bool twoCounted = false, threeCounted = false;
        int count = 0;
        char prev = id[0];
        for (auto c : id) {
            if (c == prev) {
                count++;
                if (count > 3)
                    break;
            } else {
                if (count == 2) twoCounted = true;
                else if (count == 3) threeCounted = true;
                if (twoCounted && threeCounted)
                    break;
                count = 1;
            }
            prev = c;
        }
        if (count == 2) twoCounted = true;
        else if (count == 3) threeCounted = true;
    
        twoCount += (twoCounted ? 1 : 0);
        threeCount += (threeCounted ? 1 : 0);
    }
    return twoCount * threeCount;
}

void part2(std::ifstream& file)
{
    file.clear();
    file.seekg(0);

    std::string line;
    std::vector<std::string> input;
    while (getline(file, line))
    {
        input.push_back(line);
    }

    for (const auto& x : input)
    {
        for (const auto& y : input)
        {
            int diff = 0;
            int diffIndex = -1;

            for (int i = 0, end = x.size(); i != end; ++i)
            {
                if (x[i] != y[i])
                {
                    diff++;
                    diffIndex = i;
                }
            }

            if (diff == 1)
            {
                std::cout << "ID: " << x << ", " << y << std::endl;
                std::cout << "Diff index: " << diffIndex << "; x, y: " << x[diffIndex] << ", " << y[diffIndex] << " ";
                std::string output = x.substr(0, diffIndex) + x.substr(diffIndex + 1, x.size() - 1);
                
                std::cout << "Day 2, part 2: " << output << std::endl;
                return;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    std::ifstream file("/Users/david/Dev/C++/bazel_projects/advent/day2/input.txt");
    std::cout << "Day 2, part 1: " << part1(file) << std::endl;
    part2(file);
}
