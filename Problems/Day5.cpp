#include "Day5.h"
#include <string>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

namespace aoc
{

namespace
{
std::vector<uint64_t> ParseSeeds(const std::string& i_line)
{
    std::stringstream lineStream(i_line);
    lineStream.ignore(7);

    std::vector<uint64_t> seeds;
    uint32_t seed;
    while (lineStream >> seed)
    {
        seeds.emplace_back(seed);
    }
    return seeds;
}

struct Range
{
    uint64_t start = 0;
    uint64_t length = 0;
};

std::vector<Range> ParseSeedsAsRanges(const std::string& i_line)
{
    std::stringstream lineStream(i_line);
    lineStream.ignore(7);

    std::vector<Range> seeds;
    uint64_t seedStart;
    uint64_t length;
    while (lineStream >> seedStart >> length)
    {
        seeds.emplace_back(Range{seedStart, length});
    }
    return seeds;
}

struct Conversion
{
    uint64_t destinationStart;
    uint64_t delta;
};

std::vector<std::map<uint64_t, Conversion>> ParseConverters(std::ifstream& i_stream)
{
    std::string line;
    std::vector<std::map<uint64_t, Conversion>> result;
    while (std::getline(i_stream, line))
    {
        std::map<uint64_t, Conversion>& newConversion = result.emplace_back();
        while (std::getline(i_stream, line) && line != "")
        {
            //This would be prettier by splitting the string
            std::stringstream numberParser(line);
            uint32_t sourceStart;
            uint32_t destinationStart;
            uint32_t delta;
            numberParser >> destinationStart >> sourceStart >> delta;
            newConversion.emplace(sourceStart, Conversion{destinationStart, delta});
        }
    }
    return result;
}

uint64_t ConvertSeedToLocation(const std::vector<std::map<uint64_t, Conversion>>& i_converters, uint64_t i_seed)
{
    uint64_t seed = i_seed;
    for (const std::map<uint64_t, Conversion>& converter : i_converters)
    {
        auto it = converter.upper_bound(seed);
        //if there is no converter available, we stay with the same number
        if (it == std::begin(converter))
        {
            continue;
        }
        it--;
        const uint32_t seedDelta = seed - it->first;
        if (seedDelta >=  it->second.delta)
        {
            continue;
        }
        seed = it->second.destinationStart + seedDelta;
    }
    return seed;
}

void ConvertSeedRange(const Range& i_range, const std::map<uint64_t, Conversion>& i_converter, std::vector<Range>& o_ranges)
{
    auto startIt = i_converter.upper_bound(i_range.start);
    auto endIt = i_converter.upper_bound(i_range.start+i_range.length);
    if (startIt == std::begin(i_converter))
    {
        //range is before the start of the converter
        if (startIt == endIt)
        {
            o_ranges.emplace_back(i_range);
            return;
        }
        //add the range before the first
        o_ranges.emplace_back(i_range.start, startIt->first-i_range.start);
    }
    else
    {
        startIt--;
    }
    const uint64_t rangeEnd = i_range.start + i_range.length;
    while (startIt != endIt && rangeEnd > startIt->first)
    {
        const uint64_t conversionRangeStart = startIt->first;
        const uint64_t conversionRangeEnd = conversionRangeStart + startIt->second.delta;
        if (i_range.start < conversionRangeEnd)
        {
            uint64_t startDelta = 0;
            if (i_range.start > startIt->first)
            {
                startDelta = i_range.start - startIt->first;
            }
            uint64_t previousDelta = 0;
            if (i_range.start < startIt->first)
            {
                previousDelta = startIt->first - i_range.start;
            }
            const uint64_t convertedStart = startIt->second.destinationStart + startDelta;
            const uint64_t remainingRange = i_range.length - previousDelta;
            const uint64_t conversionRangeRemainingRange = startIt->second.delta - startDelta;
            o_ranges.emplace_back(convertedStart, std::min(remainingRange, conversionRangeRemainingRange));
        }
        ++startIt;
        if (rangeEnd <= conversionRangeEnd)
        {
            //we're done
            continue;
        }
        const uint64_t remainingRangeDelta = rangeEnd - conversionRangeEnd;
        if (startIt == std::end(i_converter))
        {
            const uint64_t conversionStart = std::max(conversionRangeEnd, i_range.start);
            o_ranges.emplace_back(conversionStart, rangeEnd - conversionStart);
            continue;
        }
        const uint64_t totalRangeInBetween = startIt->first - conversionRangeEnd;
        o_ranges.emplace_back(conversionRangeEnd, std::min(remainingRangeDelta, totalRangeInBetween));
    }
}

}

void Day5Prob1()
{
    std::ifstream input("Inputs/Day5.txt");
    std::string line;
    std::getline(input, line);
    const std::vector<uint64_t> seeds = ParseSeeds(line);
    std::getline(input, line);
    const std::vector<std::map<uint64_t, Conversion>> converters = ParseConverters(input);
    uint64_t minSeed = ConvertSeedToLocation(converters, seeds[0]);
    for (size_t i = 1; i < seeds.size(); ++i)
    {
        minSeed = std::min(minSeed, ConvertSeedToLocation(converters, seeds[i]));
    }
    std::cout << minSeed << "\n";
}

void Day5Prob2()
{
    std::ifstream input("Inputs/Day5.txt");
    std::string line;
    std::getline(input, line);
    std::vector<Range> seeds = ParseSeedsAsRanges(line);
    std::getline(input, line);
    const std::vector<std::map<uint64_t, Conversion>> converters = ParseConverters(input);
    for (const std::map<uint64_t, Conversion>& converter : converters)
    {
        std::vector<Range> nextStepSeeds;
        for (const Range& seedRange : seeds)
        {
            ConvertSeedRange(seedRange, converter, nextStepSeeds);
        }
        seeds = std::move(nextStepSeeds);
    }
    const auto minElement = std::ranges::min_element(seeds, [](const Range &i_lhs, const Range &i_rhs) { return i_lhs.start < i_rhs.start; });
    std::cout << minElement->start;
}
}