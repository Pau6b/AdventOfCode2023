#include "Day4.h"
#include <string>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <iostream>
#include <unordered_set>
#include <array>
#include <numeric>
#include <iterator>

namespace aoc
{

namespace
{

const size_t k_winningNumbersQuantity = 10;

uint64_t ComputeCardMatches(const std::string& i_str)
{
    std::stringstream lineStream(i_str);
    std::string s;
    //ignore card number
    std::getline(lineStream, s, ':');

    //parse winning numbers
    std::unordered_set<uint32_t> winningNumbers;
    winningNumbers.reserve(k_winningNumbersQuantity);
    for (size_t i = 0; i < k_winningNumbersQuantity; ++i)
    {
        uint32_t winningNumber;
        lineStream >> winningNumber;
        winningNumbers.emplace(winningNumber);
    }

    //ignore |
    std::string remaining;
    const int32_t currentPos = lineStream.tellg();
    lineStream.seekg((currentPos + 3));

    uint32_t winningNumbersQuantity = 0;
    uint32_t number;
    while (lineStream >> number)
    {
        if (winningNumbers.contains(number))
        {
            ++winningNumbersQuantity;
        }
    }
    return winningNumbersQuantity;
}

uint64_t ComputeCardPoints(const std::string& i_str)
{
    uint64_t winningNumbersQuantity = ComputeCardMatches(i_str);
    if (winningNumbersQuantity == 0)
    {
        return 0;
    }
    return 1 << (winningNumbersQuantity-1);
}
}

void Day4Prob1()
{
    std::fstream input("Inputs/Day4.txt");
    std::string cardLine;
    uint64_t result = 0;
    while (std::getline(input, cardLine))
    {
        result += ComputeCardPoints(cardLine);
    }
    std::cout << result << '\n';
}

void Day4Prob2()
{
    std::fstream input("Inputs/Day4.txt");
    std::string cardLine;
    std::array<uint32_t, 193> accumulatedCards;
    accumulatedCards.fill(1);
    size_t currentCard = 0;
    while (std::getline(input, cardLine))
    {
        const uint32_t cardsWon = ComputeCardMatches(cardLine);
        for (size_t i = 1; i <= cardsWon; ++i)
        {
            accumulatedCards[currentCard + i] += accumulatedCards[currentCard];
        }
        ++currentCard;
    }
    auto endIt = std::begin(accumulatedCards);
    std::advance(endIt, currentCard);
    const uint64_t result = std::accumulate(std::begin(accumulatedCards), endIt, 0);
    std::cout << result << '\n';
}
}