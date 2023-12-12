#include "Day7.h"
#include <string>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace aoc
{

namespace
{

enum class HandType
{
    FiveOfAKind     = 7,
    FourOfAKind     = 6,
    FullHouse       = 5,
    ThreeOfAKind    = 4,
    TwoPair         = 3,
    OnePair         = 2,
    HighCard        = 1
};

HandType ComputeType(const std::string& i_str, bool i_useJokers)
{
    std::unordered_map<char, uint32_t> counter;
    for (char c : i_str)
    {
        const auto& [it, didEmplace] = counter.emplace(c, 1);
        if (!didEmplace)
        {
            it->second++;
        }
    }

    if (counter.size() == 1)
    {
        return HandType::FiveOfAKind;
    }
    uint32_t threeOfAKindQuantity = 0;
    uint32_t pairQuantity = 0;

    if (auto jokersIt = counter.find('J'); i_useJokers && jokersIt != std::end(counter))
    {
        const uint32_t jokersQuantity = jokersIt->second;
        counter.erase(jokersIt);
        auto maxElementIt = std::max_element(std::begin(counter), std::end(counter), [](const auto &i_lhs, const auto &i_rhs) { return i_lhs.second < i_rhs.second; });
        maxElementIt->second += jokersQuantity;
    }

    if (counter.size() == 1)
    {
        return HandType::FiveOfAKind;
    }

    for (const auto& [c, quantity]: counter)
    {
        if (quantity == 4)
        {
            return HandType::FourOfAKind;
        }
        else if (quantity == 3)
        {
            ++threeOfAKindQuantity;
        }
        else if (quantity == 2)
        {
            ++pairQuantity;
        }
    }
    if (threeOfAKindQuantity == 1 && pairQuantity == 1)
    {
        return HandType::FullHouse;
    }
    if (threeOfAKindQuantity == 1)
    {
        return HandType::ThreeOfAKind;
    }
    if (pairQuantity == 2)
    {
        return HandType::TwoPair;
    }
    if (pairQuantity == 1)
    {
        return HandType::OnePair;
    }
    return HandType::HighCard;
}

struct CamelCardHand
{
    std::string hand;
    uint32_t value;
    HandType type;

    CamelCardHand(const std::string& i_str, bool i_useJokers)
        : hand(i_str.substr(0,5))
        , value(std::stoi(i_str.substr(6)))
        , type(ComputeType(hand, i_useJokers))
    {
    }
};

std::vector<CamelCardHand> ParseInput(const std::string& i_path, bool i_useJokers)
{
    std::ifstream input(i_path);
    std::string line;
    std::vector<CamelCardHand> result;
    while(std::getline(input, line))
    {
        result.emplace_back(line, i_useJokers);
    }
    return result;
}

uint32_t GetCardValue(char c, bool i_useJokers)
{
    if ('1' <= c && c <= '9')
    {
        return c - '0';
    }
    if (c == 'T') { return 10; }
    if (c == 'J') { return i_useJokers ? 0 : 11; }
    if (c == 'Q') { return 12; }
    if (c == 'K') { return 13; }
    if (c == 'A') { return 14; }
    return -1;
}

bool CompareHands(const CamelCardHand& i_lhs, const CamelCardHand& i_rhs, bool i_useJokers)
{
    if(i_lhs.type != i_rhs.type)
    {
        return i_lhs.type < i_rhs.type;
    }
    for (size_t idx = 0; idx < i_lhs.hand.size(); ++idx)
    {
        if (i_lhs.hand[idx] != i_rhs.hand[idx])
        {
            return GetCardValue(i_lhs.hand[idx], i_useJokers) < GetCardValue(i_rhs.hand[idx], i_useJokers);
        }
    }
    return true;
}

}

void Day7Prob1()
{
    std::vector<CamelCardHand> hands = ParseInput("Inputs/Day7.txt", false);
    std::sort(std::begin(hands), std::end(hands), [](const auto& i_lhs, const auto& i_rhs){ return CompareHands(i_lhs, i_rhs, false);});
    uint64_t result = 0;
    for (size_t idx = 0; idx < hands.size(); ++idx)
    {
        result += (idx+1) * hands[idx].value;
    }
    std::cout << result << '\n';
}

void Day7Prob2()
{
    std::vector<CamelCardHand> hands = ParseInput("Inputs/Day7.txt", true);
    std::sort(std::begin(hands), std::end(hands),[](const auto& i_lhs, const auto& i_rhs){ return CompareHands(i_lhs, i_rhs, true);});
    uint64_t result = 0;
    for (size_t idx = 0; idx < hands.size(); ++idx)
    {
        result += (idx+1) * hands[idx].value;
    }
    std::cout << result << '\n';
}
}