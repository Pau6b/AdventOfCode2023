#include "Day2.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <inttypes.h>

namespace aoc
{

namespace
{
bool IsGamePossible(const std::string& i_round, uint32_t i_maxRed, uint32_t i_maxGreen, uint32_t i_maxBlue)
{
    std::stringstream lineStream(i_round);
    std::string color;
    //Ignore Game XX:
    std::getline(lineStream, color, ':');
    uint32_t quantity;
    while (lineStream >> quantity >> color)
    {
        if (color[color.size()-1] == ',' || color[color.size()-1] == ';')
        {
            color.pop_back();
        }
        if ((color == "red" && quantity > i_maxRed) ||
            (color == "blue" && quantity > i_maxBlue) ||
            (color == "green" && quantity > i_maxGreen))
        {
            return false;
        }
    }
    return true;
}

uint32_t GetPowerOfMinimumCubes(const std::string i_game)
{
    std::stringstream lineStream(i_game);
    std::string color;
    //Ignore Game XX:
    std::getline(lineStream, color, ':');
    uint32_t quantity;
    uint32_t minRed = 0;
    uint32_t minBlue = 0;
    uint32_t minGreen = 0;
    while (lineStream >> quantity >> color)
    {
        if (color[color.size()-1] == ',' || color[color.size()-1] == ';')
        {
            color.pop_back();
        }
        if (color == "red")
        {
            if (minRed < quantity)
            {
                minRed = quantity;
            }
        }
        else if (color == "blue")
        {
            if (minBlue < quantity)
            {
                minBlue = quantity;
            }
        }
        else if (color == "green")
        {
            if (minGreen < quantity)
            {
                minGreen = quantity;
            }
        }
    }
    return minRed * minBlue * minGreen;
}
}

void Day2Prob1()
{
    std::ifstream input("Inputs/Day2.txt");
    std::string game;
    uint32_t round = 1;
    uint32_t result = 0;
    while (std::getline(input, game))
    {
        if (IsGamePossible(game, 12, 13, 14))
        {
            result += round;
        }
        ++round;
    }
    std::cout << result << "\n";
}

void Day2Prob2()
{
    std::ifstream input("Inputs/Day2.txt");
    std::string game;
    uint32_t result = 0;
    while (std::getline(input, game))
    {
        result += GetPowerOfMinimumCubes(game);
    }
    std::cout << result << "\n";
}
}