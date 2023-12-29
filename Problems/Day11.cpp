#include "Day11.h"
#include <string>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <iostream>
#include <vector>
#include <set>
#include "Core.h"

namespace aoc
{

struct Universe
{
    std::vector<Vec2> galaxies;
    std::set<size_t> freeRows;
    std::set<size_t> freeCols;
};

Universe ParseUniverse(const std::string& i_path)
{
    Universe result;

    std::ifstream input(i_path);
    std::string line;

    std::getline(input, line);
    for (size_t idx = 0; idx < line.size(); ++idx)
    {
        result.freeCols.emplace(idx);
    }
    uint32_t row = 0;
    do
    {
        bool isRowFree = true;
        for (uint32_t col = 0; col < line.size(); ++col)
        {
            if (line[col] == '#')
            {
                isRowFree = false;
                result.galaxies.emplace_back(row, col);
                result.freeCols.erase(col);
            }
        }
        if (isRowFree)
        {
            result.freeRows.emplace(row);
        }
        ++row;
    } while(std::getline(input, line));

    return result;
}

size_t CountElementsInRange(const std::set<size_t>& i_numbers, size_t i_start, size_t i_end)
{
    auto minElementIt = i_numbers.upper_bound(i_start);
    size_t result = 0;
    while (*minElementIt < i_end && minElementIt != std::end(i_numbers))
    {
        ++result;
        ++minElementIt;
    }
    return result;
}

size_t ComputeDistances(const Universe& i_universe, size_t i_emptyRowSize)
{
    size_t result = 0;
    i_emptyRowSize -= 1;
    for (size_t i = 0; i < i_universe.galaxies.size(); ++i)
    {
        for (size_t j = i+1; j < i_universe.galaxies.size(); ++j)
        {
            const Vec2& galaxy1 = i_universe.galaxies[i];
            const Vec2& galaxy2 = i_universe.galaxies[j];
            const size_t minx = std::min(galaxy1.x, galaxy2.x);
            const size_t maxx = std::max(galaxy1.x, galaxy2.x);
            const size_t miny = std::min(galaxy1.y, galaxy2.y);
            const size_t maxy = std::max(galaxy1.y, galaxy2.y);

            result += (maxx - minx) + (maxy - miny) + 
                      (CountElementsInRange(i_universe.freeRows, minx, maxx)*i_emptyRowSize) + 
                      (CountElementsInRange(i_universe.freeCols, miny, maxy)*i_emptyRowSize);
        }
    }
    return result;
}

void Day11Prob1()
{
    Universe universe = ParseUniverse("Inputs/Day11.txt"); 
    std::cout << ComputeDistances(universe, 1) << '\n';
}

void Day11Prob2()
{
    Universe universe = ParseUniverse("Inputs/Day11.txt"); 
    std::cout << ComputeDistances(universe, 1000000) << '\n';
}
}