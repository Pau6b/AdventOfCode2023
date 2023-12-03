#include "Day3.h"
#include <fstream>
#include <vector>
#include <string>
#include <inttypes.h>
#include <iostream>
#include <functional>
#include <unordered_map>

namespace
{
struct Position
{
    int32_t row;
    int32_t col;

    bool operator==(const Position& i_other) const { return row == i_other.row && col == i_other.col; }
};

struct Gear
{
    uint32_t adjacentNumbersQuantity = 0;
    uint32_t gearRatio = 1;
};
}

template <>
struct std::hash<Position>
{
    std::size_t operator()( const Position& k ) const
    {
        // Compute individual hash values for first, second and third
        // http://stackoverflow.com/a/1646913/126995
        std::size_t res = 17;
        res = res * 31 + hash<int32_t>()( k.row );
        res = res * 31 + hash<int32_t>()( k.col );
        return res;
    }
};

namespace aoc
{

namespace
{

inline bool IsNumber(char i_c)
{
    return '0' <= i_c && i_c <= '9';
}

inline bool IsIndexValid(const std::vector<std::string>& i_engineSchematic, int32_t i_row, int32_t i_col)
{
    return i_row >= 0 && i_col >= 0 && i_row < i_engineSchematic.size() && i_col < i_engineSchematic[i_row].size();
}

inline bool IsSymbol(const std::vector<std::string>& i_engineSchematic, int32_t i_row, int32_t i_col)
{
    return IsIndexValid(i_engineSchematic, i_row, i_col) && !IsNumber(i_engineSchematic[i_row][i_col]) && i_engineSchematic[i_row][i_col] != '.';
}

inline bool IsGear(const std::vector<std::string>& i_engineSchematic, int32_t i_row, int32_t i_col)
{
    return IsIndexValid(i_engineSchematic, i_row, i_col) && i_engineSchematic[i_row][i_col] == '*';
}

inline void AddPositionIfIsGear(const std::vector<std::string>& i_engineSchematic, int32_t i_row, int32_t i_col, std::vector<Position>& o_positions)
{
    if (IsGear(i_engineSchematic, i_row, i_col))
    {
        o_positions.emplace_back(Position{i_row, i_col});
    }
}

}

void Day3Prob1()
{
    std::ifstream input("Inputs/Day3.txt");
    std::string line;
    std::vector<std::string> engineSchematic;
    
    while(std::getline(input, line))
    {
        engineSchematic.emplace_back(line);
    }

    uint32_t result = 0;
    for (int32_t row = 0; row < static_cast<int32_t>(engineSchematic.size()); ++row)
    {
        for (int32_t col = 0; col < static_cast<int32_t>(engineSchematic[row].size());)
        {
            //We are looking for the numbers
            if (!IsNumber(engineSchematic[row][col]))
            {
                ++col;
                continue;
            }

            //Check if previous column has a symbol
            bool isNumberSurroundedBySymbol = IsSymbol(engineSchematic, row,     col - 1) ||
                                              IsSymbol(engineSchematic, row - 1, col - 1) ||
                                              IsSymbol(engineSchematic, row + 1, col - 1);
            uint32_t parsedNumber = 0;
            while (col < engineSchematic[row].size() && IsNumber(engineSchematic[row][col]))
            {
                //Check if char above or below are a symbol
                isNumberSurroundedBySymbol = isNumberSurroundedBySymbol ||
                                             IsSymbol(engineSchematic, row - 1, col) ||
                                             IsSymbol(engineSchematic, row + 1, col);
                parsedNumber *= 10;
                parsedNumber += engineSchematic[row][col] - '0';
                ++col;
            }
            //Check if row aftrewards are a symbol
            isNumberSurroundedBySymbol = isNumberSurroundedBySymbol ||
                                         IsSymbol(engineSchematic, row,     col) ||
                                         IsSymbol(engineSchematic, row - 1, col) ||
                                         IsSymbol(engineSchematic, row + 1, col);
            if (isNumberSurroundedBySymbol)
            {
                result += parsedNumber;
            }
        }
    }
    std::cout << result;
}

void Day3Prob2()
{
    std::ifstream input("Inputs/Day3.txt");
    std::string line;
    std::vector<std::string> engineSchematic;
    
    while(std::getline(input, line))
    {
        engineSchematic.emplace_back(line);
    }

    std::unordered_map<Position, Gear> gears;
    for (int32_t row = 0; row < static_cast<int32_t>(engineSchematic.size()); ++row)
    {
        for (int32_t col = 0; col < static_cast<int32_t>(engineSchematic[row].size());)
        {
            //We are looking for the numbers
            if (!IsNumber(engineSchematic[row][col]))
            {
                ++col;
                continue;
            }

            std::vector<Position> gearPositions;
            AddPositionIfIsGear(engineSchematic, row,     col - 1, gearPositions);
            AddPositionIfIsGear(engineSchematic, row - 1, col - 1, gearPositions);
            AddPositionIfIsGear(engineSchematic, row + 1, col - 1, gearPositions);
            uint32_t parsedNumber = 0;
            while (col < engineSchematic[row].size() && IsNumber(engineSchematic[row][col]))
            {
                AddPositionIfIsGear(engineSchematic, row - 1, col, gearPositions);
                AddPositionIfIsGear(engineSchematic, row + 1, col, gearPositions);
                parsedNumber *= 10;
                parsedNumber += engineSchematic[row][col] - '0';
                ++col;
            }
            AddPositionIfIsGear(engineSchematic, row, col, gearPositions);
            AddPositionIfIsGear(engineSchematic, row - 1, col, gearPositions);
            AddPositionIfIsGear(engineSchematic, row + 1, col, gearPositions);

            for (const Position& gearPosition : gearPositions)
            {
                Gear &gear = gears[gearPosition];
                gear.adjacentNumbersQuantity++;
                gear.gearRatio *= parsedNumber;
            }
        }
    }

    uint64_t result = 0;
    for (const auto &[_p, gear] : gears)
    {
        if (gear.adjacentNumbersQuantity == 2)
        {
            result += gear.gearRatio;
        }
    }
    std::cout << result;
}
}