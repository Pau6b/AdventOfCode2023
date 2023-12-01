#include "Day1.h"
#include <fstream>
#include <string>
#include <iostream>
#include <inttypes.h>
#include <algorithm>
#include <array>

namespace aoc
{

namespace
{
uint32_t ParseNumber(const std::string &i_line)
{
    const auto isNumber = [](char i_c)
    { return '0' <= i_c && i_c <= '9'; };
    const auto firstDigitIt = std::find_if(std::begin(i_line), std::end(i_line), isNumber);
    const auto secondDigitIt = std::find_if(std::rbegin(i_line), std::rend(i_line), isNumber);
    return ((*firstDigitIt - '0') * 10) + (*secondDigitIt - '0');
}

const std::array<std::string, 9> numbersInWords{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

uint32_t ParseNumberWithWords(const std::string &i_line)
{
    size_t firstWordDigitPos = i_line.size();
    uint32_t firstDigit = 0;
    size_t secondWordDigitPos = 0;
    uint32_t secondDigit = 0;
    for (size_t i = 0; i < numbersInWords.size(); ++i)
    {
        auto tempFirstWordDigitPos = i_line.find(numbersInWords[i]);
        if (tempFirstWordDigitPos < firstWordDigitPos)
        {
            firstWordDigitPos = tempFirstWordDigitPos;
            firstDigit = i + 1;
        }
        auto tempSecondWordDigitPos = i_line.rfind(numbersInWords[i]);
        if (tempSecondWordDigitPos > secondWordDigitPos && tempFirstWordDigitPos != i_line.npos)
        {
            secondWordDigitPos = tempSecondWordDigitPos;
            secondDigit = i + 1;
        }
    }
    const auto isNumber = [](char i_c) { return '0' <= i_c && i_c <= '9'; };

    const auto firstDigitIt = std::find_if(std::begin(i_line), std::end(i_line), isNumber);
    if (std::distance(std::begin(i_line), firstDigitIt) < firstWordDigitPos)
    {
        firstDigit = *firstDigitIt - '0';
    }

    const auto secondDigitIt = std::find_if(std::rbegin(i_line), std::rend(i_line), isNumber);
    if (std::distance(std::begin(i_line), secondDigitIt.base()) > secondWordDigitPos)
    {
        secondDigit = *secondDigitIt - '0';
    }
    return (firstDigit * 10) + secondDigit;
}
}

void Day1Prob1()
{
    std::ifstream input("Inputs/Day1.txt");
    std::string line;
    uint32_t result = 0;
    while (std::getline(input, line))
    {
        result += ParseNumber(line);
    }
    std::cout << "Prob1: " << result << "\n";
}

void Day1Prob2()
{
    std::ifstream input("Inputs/Day1.txt");
    std::string line;
    uint32_t result = 0;
    while (std::getline(input, line))
    {
        result += ParseNumberWithWords(line);
    }
    std::cout << "Prob1: " << result << "\n";
}

}