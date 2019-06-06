#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "include/Gene.h"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

int main(int argc, char* argv[])
{
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> int_dist(0, 1000);
    
    auto gene_generator = [&](const Chromosome_t& c) -> int {
        return int_dist(generator) % 3 - 1;
    };

    auto mutation = [&](Gene& gene) {
        if (int_dist(generator) < 70)
        {
            gene.data = int_dist(generator) % 3 - 1;
        }
    };

    auto fitness = [](const Chromosome_t& chromosome) -> int {
        int velocity = 0;
        int goal = 234;
        int pos = 0;
        
        for (const auto& gene : chromosome)
        {
            velocity += gene.data;
            pos += velocity;
        }

        return (pos - goal) * (pos - goal) + velocity*velocity;
    };

    auto next_gen = [&](Genome_t& genome) -> Genome_t {
        Genome_t next;
        std::sort(begin(genome), end(genome), [&](const Chromosome_t& a, const Chromosome_t& b) {
            if (fitness(a) < fitness(b))
                return true;
            return false;
        });

        auto& breeder = genome.front();
        auto len = genome.size() / 2 + 1;
        for (size_t i = 1; i < len; ++i)
        {
            auto spawn = GA::crossover(breeder, genome[i]);
            next.push_back(GA::mutate(spawn.first, mutation));
            next.push_back(GA::mutate(spawn.second, mutation));
        }
        return next;
    };

    Genome_t genome;
    for (int i = 0; i < 128; ++i)
    {
        genome.push_back(GA::create_chromosome(32, gene_generator));
    }

    size_t generation = 0;
    bool found = false;
    auto start = std::chrono::steady_clock::now();
    while (!found)
    {
        ++generation;
        for (const auto& chromosome : genome)
        {
            auto fit = fitness(chromosome);
            if (fit == 0) {
                found = true;
                std::cout << "Gen: " << generation << " " << GREEN << chromosome << " >> " << fit << RESET << std::endl;
            }
        }
        if (!found)
            genome = next_gen(genome);
    }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    std::cout << "Duration: " << duration.count() << " msec; Generation/sec: " << generation * 1000 / duration.count() 
        << "; Processed chromosomes/sec: " << generation*genome.size() * 1000 / duration.count() << std::endl;
    return 0;
}