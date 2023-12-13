#include "Day8.h"
#include <string>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>

namespace aoc
{

namespace
{

struct Destination
{
    std::string left;
    std::string right;
};

struct Input
{
    std::string directions;
    std::unordered_map<std::string, Destination> graph;
};

size_t ComputeSteps(const std::string& i_directions, std::unordered_map<std::string, Destination> i_graph, std::string i_startingNode)
{
    const std::string* node = &i_startingNode;
    size_t idx = 0;
    size_t steps = 0;
    while ((*node)[2] != 'Z')
    {
        auto nodeIt = i_graph.find(*node);
        if (nodeIt == std::end(i_graph))
        {
            std::cout << "Error, node not found";
        }
        if (i_directions[idx] == 'L')
        {
            node = &nodeIt->second.left;
        }
        else
        {
            node = &nodeIt->second.right;
        }
        ++idx;
        idx = idx%i_directions.size();
        steps++;
    }
    return steps;
}

Input ParseInput(const std::string& i_path)
{
    std::ifstream input(i_path);
    Input result;
    std::getline(input, result.directions);
    std::string from;
    while (input >> from)
    {
        std::string left(3, '\0');
        std::string right(3, '\0');
        input.ignore(4);
        input.read(&left[0], 3);
        input.ignore(2);
        input.read(&right[0], 3);
        input.ignore(1);
        result.graph.emplace(from, Destination{left, right});
    }
    return result;
}
}

void Day8Prob1()
{
    const Input input = ParseInput("Inputs/Day8.txt");
    std::cout << ComputeSteps(input.directions, input.graph, "AAA") << "\n";
}

void Day8Prob2()
{
    Input input = ParseInput("Inputs/Day8.txt");
    std::vector<const std::string*> nodes;
    for (const auto& [node, _ph_] : input.graph)
    {
        if (node[2] != 'A')
        {
            continue;
        }
        nodes.emplace_back(&node);
    }
    std::vector<size_t> steps;
    steps.reserve(nodes.size());
    std::transform(std::begin(nodes),
                   std::end(nodes),
                   std::back_inserter(steps),
                   [&input](const std::string *i_node) { return ComputeSteps(input.directions, input.graph, *i_node); });
    size_t result = 1;
    for (size_t step : steps)
    {
        result = std::lcm(result, step);
    }
    std::cout << result;
}
}