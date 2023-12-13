#include "Day10.h"
#include <string>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <iostream>
#include <vector>
#include <unordered_map>

namespace aoc
{

namespace
{

struct Vec2
{
    int32_t x;
    int32_t y;

    Vec2& operator+=(const Vec2& i_other)
    {
        x += i_other.x;
        y += i_other.y;
        return *this;
    }

    Vec2 operator+(const Vec2& i_other)
    {
        Vec2 result = *this;
        result += i_other;
        return result;
    }

    bool operator==(const Vec2& i_other) const
    {
        return x == i_other.x && y == i_other.y;
    }
};

struct Input
{
    std::vector<std::string> map;
    Vec2 startingPosition;
};

Input ParseInput(const std::string& i_path)
{
    std::ifstream input(i_path);
    Input result;
    std::string line;
    std::getline(input,line);

    
    result.map.reserve(line.size());
    auto processLine = [&result](const std::string &i_line) {
        result.map.emplace_back(i_line);
        if (size_t pos = i_line.find('S'); pos != std::string::npos)
        {
            result.startingPosition = Vec2{static_cast<int32_t>(pos), static_cast<int32_t>(result.map.size()) - 1};
        }
    };

    processLine(line);
    while(std::getline(input, line))
    {
        processLine(line);
    }
    return result;
}

bool IsPositionValid(const std::vector<std::string>& i_map, const Vec2& i_newPosition)
{
    //Assuming the map is a square
    return i_newPosition.x >= 0 && i_newPosition.y >= 0 && i_newPosition.x < i_map.size() && i_newPosition.y < i_map.size();
}

static const Vec2 k_up    = { 0,-1};
static const Vec2 k_down  = { 0, 1};
static const Vec2 k_left  = {-1, 0};
static const Vec2 k_right = { 1, 0};

Vec2 FindInitialPipeDirection(const Input& i_input)
{
    Vec2 position = i_input.startingPosition;

    const Vec2 top = position + k_up;
    if (IsPositionValid(i_input.map, top) && (i_input.map[top.y][top.x] == '|'
                                             || i_input.map[top.y][top.x] == 'F' 
                                             || i_input.map[top.y][top.x] == '7'))
    {
        return Vec2{0, -1};
    }
    const Vec2 bottom = position + k_down;
    if (IsPositionValid(i_input.map, bottom) && (i_input.map[bottom.y][bottom.x] == '|'
                                             || i_input.map[bottom.y][bottom.x] == 'L' 
                                             || i_input.map[bottom.y][bottom.x] == 'J'))
    {
        return Vec2{0, 1};
    }
    const Vec2 k_leftSide = position + k_left;
    if (IsPositionValid(i_input.map, k_leftSide) && (i_input.map[k_leftSide.y][k_leftSide.x] == '-'
                                             || i_input.map[k_leftSide.y][k_leftSide.x] == 'L' 
                                             || i_input.map[k_leftSide.y][k_leftSide.x] == 'F'))
    {
        return Vec2{-1, 0};
    }
    const Vec2 k_rightSide = position + k_right;
    if (IsPositionValid(i_input.map, k_rightSide) && (i_input.map[k_rightSide.y][k_rightSide.x] == '-'
                                             || i_input.map[k_rightSide.y][k_rightSide.x] == 'J' 
                                             || i_input.map[k_rightSide.y][k_rightSide.x] == '7'))
    {
        return Vec2{-1, 0};
    }
    std::cout << "error\n";
    return Vec2{0, 0};
}

Vec2 GetNextDirection(std::vector<std::string> i_map, Vec2 i_pos, Vec2 i_previousDirection)
{
    const char pipe = i_map[i_pos.y][i_pos.x];
    if (pipe == '|')
    {
        if (i_previousDirection == k_down){ return k_down; }
        if (i_previousDirection == k_up){ return k_up; }
    }
    if (pipe == '-')
    {
        if (i_previousDirection == k_left){ return k_left; }
        if (i_previousDirection == k_right){ return k_right; }
    }
    if (pipe == 'L')
    {
        if (i_previousDirection == k_down){ return k_right; }
        if (i_previousDirection == k_left){ return k_up; }
    }
    if (pipe == 'J')
    {
        if (i_previousDirection == k_right){ return k_up; }
        if (i_previousDirection == k_down){ return k_left; }
    }
    if (pipe == '7')
    {
        if (i_previousDirection == k_right){ return k_down; }
        if (i_previousDirection == k_up){ return k_left; }
    }
    if (pipe == 'F')
    {
        if (i_previousDirection == k_up){ return k_right; }
        if (i_previousDirection == k_left){ return k_down; }
    }
    std::cout << "error at char [" << pipe << "] and previous dir [" << i_previousDirection.x << ", " << i_previousDirection.y << "]\n";
    return Vec2{0, 0};
}

uint32_t GetTotalLoopSize(const Input& i_input)
{
    Vec2 position = i_input.startingPosition;
    Vec2 dir = FindInitialPipeDirection(i_input);
    position += dir;
    size_t steps = 1;
    while (i_input.map[position.y][position.x] != 'S')
    {
        dir = GetNextDirection(i_input.map, position, dir);
        position += dir;
        ++steps;
    }
    return steps;
}

}

void Day10Prob1()
{
    const Input input = ParseInput("Inputs/Day10.txt");
    std::cout << GetTotalLoopSize(input) << '\n';
}

void Day10Prob2()
{
}
}