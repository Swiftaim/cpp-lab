#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct Claim {
    int id;
    std::pair<int, int> pos;
    std::pair<int, int> extent;
    bool stomped = false;
};

std::ostream& operator<<(std::ostream& o, const Claim& c)
{
    return o << "id: " << c.id << " pos: " << c.pos.first << "," << c.pos.second << " extent: " << c.extent.first << "," << c.extent.second; 
}

Claim parseClaimSpec(const std::string& spec)
{
    Claim claim;
    std::string input;
    for (auto c : spec)
    {   
        switch (c) {
            case ' ':
                break;
            case '#':
                input.clear();
                break;
            case '@':
                claim.id = std::stoi(input);
                input.clear();
                break;
            case ',':
                claim.pos.first = std::stoi(input);
                input.clear();
                break;
            case ':':
                claim.pos.second = std::stoi(input);
                input.clear();
                break;
            case 'x':
                claim.extent.first = std::stoi(input);
                input.clear();
                break;
            default:
                input += c;
        }
    }

    claim.extent.second = std::stoi(input);
    input.clear();

    return claim;
}

std::pair<int, int> getFabricDimensions(const std::vector<Claim>& claims)
{
    int width = 0, height = 0;
    std::for_each(claims.begin(), claims.end(), [&width, &height](const Claim& claim) {
        auto claim_width = claim.pos.first + claim.extent.first;
        auto claim_height = claim.pos.second + claim.extent.second;
        if (claim_width > width)
            width = claim_width;
        if (claim_height > height)
            height = claim_height;
    });
    return std::make_pair(width, height);
}

int main(int argc, char* argv[])
{
    const char* dataPath{argv[1]};
    std::ifstream file(dataPath);
    
    std::vector<Claim> claims; claims.reserve(1265);
    std::string line;
    while (std::getline(file, line))
    {
        claims.push_back(parseClaimSpec(line));
    }

    auto dimensions = getFabricDimensions(claims);

    // lay out all claims on the fabric
    std::vector<int> fabric(dimensions.first * dimensions.second, 0);

    std::for_each(claims.begin(), claims.end(), [&](Claim& claim) {
        size_t begin = claim.pos.first + claim.pos.second * dimensions.first;
        
        for (size_t y = 0; y < claim.extent.second; ++y)
        {
            for (size_t x = 0; x < claim.extent.first; ++x)
            {
                auto& pos = fabric[begin + y * dimensions.first + x];
                pos += 1;
                if (pos > 1)
                    claim.stomped = true;
            }
        }
    });

    // count layers > 1 on each square inch of the fabric
    size_t count = 0;
    std::for_each(fabric.begin(), fabric.end(), [&count](int val) {
        count += (val > 1 ? 1 : 0);
    });

    std::cout << "More than one claim: " << count << std::endl;

    // find claims not stomped by others
    std::vector<Claim*> potential; potential.reserve(claims.size() / 2);
    for (size_t i = 0; i < claims.size(); ++i)
        if (!claims[i].stomped)
            potential.push_back(&claims[i]);
    
    auto stomped = [&](const Claim& claim) -> bool {
        size_t begin = claim.pos.first + claim.pos.second * dimensions.first;
        for (size_t y = 0; y < claim.extent.second; ++y)
        {
            for (size_t x = 0; x < claim.extent.first; ++x)
            {
                if (fabric[begin + y * dimensions.first + x] > 1)
                    return true;
            }
        }
        return false;
    };

    std::for_each(potential.begin(), potential.end(), [&stomped](Claim* claim) {
        if (!stomped(*claim))
            std::cout << "Claim ID: " << claim->id << " is not stomped." << std::endl;
    });
    
    return 0;
}