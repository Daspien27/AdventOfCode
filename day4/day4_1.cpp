#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <iostream>
#include <algorithm>
#include <map>
#include <functional>
#include <array>
#include <optional>
#include <numeric>


int main()
{
    std::filesystem::path input_file = "day4/day4_input.txt";


    std::ifstream fs (input_file.string(), std::ios::in);

    if (!fs.is_open())
    {
        std::runtime_error("Could not open input file!");
    }

    std::vector<std::string> Logs;
    
    {
        std::string inLine;

        while (std::getline(fs, inLine))
        {
            Logs.push_back(inLine);
        }
    }
    fs.close();

    std::sort(Logs.begin(), Logs.end());

    std::unordered_map<int, std::array<int, 60>> GuardData;

    int CurrentGuard = 0;
    int StartSleep = 0;
    int EndSleep = 0;
    for (const auto& Log : Logs)
    { 
        if (auto GuardBegin = Log.find('#'); GuardBegin != std::string::npos)
        {
            auto GuardEnd = Log.find(' ', GuardBegin);
            std::string GuardId_str = Log.substr(GuardBegin+1, GuardEnd - GuardBegin - 1);
            CurrentGuard = atoi(GuardId_str.c_str());
        }
        else if(auto GuardStartsSleeping = Log.find("falls"); GuardStartsSleeping != std::string::npos)
        {
            auto MinuteStart = Log.find(':');
            auto MinuteEnd = Log.find(']');
            std::string StartSleep_str = Log.substr(MinuteStart + 1, MinuteEnd - MinuteStart-1);
            StartSleep = atoi(StartSleep_str.c_str());
        }
        else
        {
            auto MinuteStart = Log.find(':');
            auto MinuteEnd = Log.find(']');
            std::string EndSleep_str = Log.substr(MinuteStart + 1, MinuteEnd - MinuteStart-1);
            EndSleep = atoi(EndSleep_str.c_str());

            auto& CurrentGuardHistogram = GuardData[CurrentGuard];
            auto BeginMinuteInHist = CurrentGuardHistogram.begin() + StartSleep;
            auto EndMinuteInHist = CurrentGuardHistogram.begin() + EndSleep;
            std::transform(BeginMinuteInHist,EndMinuteInHist, BeginMinuteInHist, [](auto Minute) { return Minute + 1; } );

            StartSleep = 0;
            EndSleep = 0;
        }
    }

    std::unordered_map<int, int> GuardMinutesAccumulated;

    for (const auto& Guard : GuardData)
    {
        GuardMinutesAccumulated[Guard.first] = std::accumulate(Guard.second.begin(), Guard.second.end(), 0);
    }

    auto SleepiestGuard = std::max_element(GuardMinutesAccumulated.begin(), GuardMinutesAccumulated.end(), 
                                [](const auto& A, const auto& B){return A.second < B.second;});

    const auto& SleepiestGuardHistogram = GuardData[SleepiestGuard->first];

    auto SleepiestMinute = std::distance(SleepiestGuardHistogram.begin(), 
                                         std::max_element(SleepiestGuardHistogram.begin(), SleepiestGuardHistogram.end()));

    std::cout << SleepiestGuard->first << " was sleepiest, sleeping " << SleepiestGuard->second 
              << ", most frequently on " << SleepiestMinute << " hashing to " << (SleepiestGuard->first * SleepiestMinute) <<  std::endl;


}