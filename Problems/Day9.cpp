#include "Day9.h"
#include <string>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <iostream>
#include <vector>

namespace aoc
{

std::vector<std::vector<int32_t>> ParseSequences(const std::string& i_path)
{
    std::ifstream input(i_path);
    std::string line;
    std::vector<std::vector<int32_t>> result;
    while (std::getline(input, line))
    {
        std::vector<int32_t>& sequence = result.emplace_back();

        std::stringstream lineStream(line);
        int32_t nextNumber;
        while (lineStream >> nextNumber)
        {
            sequence.emplace_back(nextNumber);
        }
    }
    return result;
}

std::vector<std::vector<int32_t>> ComputeDeltas(const std::vector<int32_t>& i_sequence)
{
    std::vector<std::vector<int32_t>> deltas;
    deltas.emplace_back(i_sequence);
    bool isAllZeros = false;
    while(!isAllZeros)
    {
        std::vector<int32_t>& newDeltas = deltas.emplace_back();
        const std::vector<int32_t>& lastDeltas = deltas[deltas.size()-2];
        newDeltas.reserve(lastDeltas.size()-1);
        isAllZeros = true;
        for (size_t idx = 0; idx < lastDeltas.size() - 1; ++idx)
        {
            const int32_t newDelta = lastDeltas[idx + 1] - lastDeltas[idx];
            isAllZeros = isAllZeros && newDelta == 0;
            newDeltas.emplace_back(newDelta);
        }
    }
    return deltas;
}

int32_t PredictNextSequenceNumber(const std::vector<int32_t>& i_sequence)
{
    const std::vector<std::vector<int32_t>> deltas = ComputeDeltas(i_sequence);

    std::vector<int32_t> predictions(deltas.size(), 0);
    for (size_t idx = deltas.size()-1; idx > 0; --idx)
    {
        predictions[idx-1] = deltas[idx-1][deltas[idx-1].size()-1] + predictions[idx];
    }

    return predictions[0];
}

int32_t PredictPreviousSequenceNumber(const std::vector<int32_t>& i_sequence)
{
    const std::vector<std::vector<int32_t>> deltas = ComputeDeltas(i_sequence);

    std::vector<int32_t> predictions(deltas.size(), 0);
    for (size_t idx = deltas.size()-1; idx > 0; --idx)
    {
        predictions[idx-1] = deltas[idx-1][0] - predictions[idx];
    }

    return predictions[0];
}

void Day9Prob1()
{
    const std::vector<std::vector<int32_t>> sequences = ParseSequences("Inputs/Day9.txt");
    int64_t result = 0;
    for (const std::vector<int32_t>& sequence : sequences)
    {
        result += PredictNextSequenceNumber(sequence);
    }
    std::cout << result;
}

void Day9Prob2()
{
    const std::vector<std::vector<int32_t>> sequences = ParseSequences("Inputs/Day9.txt");
    int64_t result = 0;
    for (const std::vector<int32_t>& sequence : sequences)
    {
        result += PredictPreviousSequenceNumber(sequence);
    }
    std::cout << result;
}
}