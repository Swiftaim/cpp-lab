#pragma once

#include <functional>
#include <ostream>
#include <random>
#include <vector>

struct Gene
{
    int data;
    Gene(int d) : data(d) {}
};

using Chromosome_t = std::vector<Gene>;
using Genome_t = std::vector<Chromosome_t>;

class GA
{
public:
    static Chromosome_t create_chromosome(size_t gene_count, std::function<int(const Chromosome_t& c)> creator)
    {
        Chromosome_t chromo; chromo.reserve(gene_count);
        for (size_t i = 0; i < gene_count; ++i)
        {
            chromo.push_back({ creator(chromo) });
        }
        return chromo;
    }

    static std::pair<Chromosome_t, Chromosome_t>
    crossover(const Chromosome_t& a, const Chromosome_t& b)
    {
        auto len = a.size();
        std::random_device device;
        std::mt19937 generator(device());
        std::uniform_int_distribution<int> cut_dist(len/4, len*3/4);

        auto cut = cut_dist(generator);
        Chromosome_t x; x.reserve(len);
        Chromosome_t y; y.reserve(len);
        for (size_t i = 0; i < cut; ++i)
        {
            x.push_back(a[i]);
        }
        for (size_t i = cut; i < len; ++i)
        {
            x.push_back(b[i]);
        }

        cut = cut_dist(generator);
        for (size_t i = 0; i < cut; ++i)
        {
            y.push_back(b[i]);
        }
        for (size_t i = cut; i < len; ++i)
        {
            y.push_back(a[i]);
        }
        return std::make_pair(x, y);
    }

    static Chromosome_t mutate(Chromosome_t& chromosome, std::function<void(Gene&)> mutation)
    {
        for (auto& gene : chromosome)
        {
            mutation(gene);
        }
        return chromosome;
    }
};

std::ostream& operator<<(std::ostream& o, const Chromosome_t& c)
{
    o << "[ ";

    for (auto gene : c)
    {
        o << gene.data << " ";
    }
    return o << "]";
}

std::ostream& operator<<(std::ostream& o, const Genome_t& g)
{
    o << "<------------------------------------ GENOME BEGIN -----------------------------------\n";
    for (const auto& chromo : g)
    {
        o << chromo << std::endl;
    }
    return o << "------------------------------------ GENOME END ----------------------------------/>\n";
}