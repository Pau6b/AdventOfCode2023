#include "Day6.h"
#include <string>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

namespace aoc
{

namespace
{
std::vector<uint32_t> ParseVector(const std::string& i_str)
{
    std::vector<uint32_t> result;

    std::stringstream lineParser(i_str);
    lineParser.ignore(10);

    uint32_t num;
    while(lineParser >> num)
    {
        result.emplace_back(num);
    }

    return result;
}

uint64_t ParseNumber(const std::string& i_str)
{
    std::string numbers = i_str.substr(10);
    numbers.erase(std::remove(std::begin(numbers), std::end(numbers), ' '), numbers.end());
    return std::stoll(numbers);
}

struct EquationSolutions
{
    double first;
    double second;
};

EquationSolutions ComputeSolution(uint64_t i_distanceToBeat, uint64_t i_maxTime)
{
    const double b = static_cast<double>(i_maxTime);
    const double c = -static_cast<double>(i_distanceToBeat);
    const double sqPart = std::sqrt((b*b)-(4.f*-1.f*c));
    return EquationSolutions{(-b+sqPart)/-2.f, (-b-sqPart)/-2.f};
}
}

void Day6Prob1()
{
    std::ifstream input("Inputs/Day6.txt");
    std::string line;
    std::getline(input, line);
    const std::vector<uint32_t> times = ParseVector(line);
    std::getline(input, line);
    const std::vector<uint32_t> distances = ParseVector(line);
    //assuming times and distances have the same size
    uint32_t result = 1;
    for (size_t idx = 0; idx < times.size(); ++idx)
    {
        const EquationSolutions solutions = ComputeSolution(distances[idx], times[idx]);
        const uint32_t lowestSolution = static_cast<uint32_t>(std::min(solutions.first, solutions.second)+1.f);
        const uint32_t highestSolution = static_cast<uint32_t>(std::ceil(std::max(solutions.first, solutions.second)-1.f));
        std::cout << highestSolution-lowestSolution+1 << "\n";
        result *= highestSolution-lowestSolution+1;
    }
    std::cout << result << "\n";
}

void Day6Prob2()
{
    std::ifstream input("Inputs/Day6.txt");
    std::string line;
    std::getline(input, line);
    const uint64_t time = ParseNumber(line);
    std::getline(input, line);
    const uint64_t distance = ParseNumber(line);
    //assuming times and distances have the same size
    const EquationSolutions solutions = ComputeSolution(distance, time);
    const uint64_t lowestSolution = static_cast<uint64_t>(std::min(solutions.first, solutions.second)+1.f);
    const uint64_t highestSolution = static_cast<uint64_t>(std::ceil(std::max(solutions.first, solutions.second)-1.f));
    std::cout << highestSolution-lowestSolution+1 << "\n";
}
}