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

    std::array<std::pair<int, int>, 60> SleepiestMinuteAmongAllGuards;

    for (int Minute = 0; Minute < 60; ++Minute)
    {
        auto SleepiestGuardThisMinuteComp = [Minute](const auto& A, const auto& B)
        {
            return A.second[Minute] < B.second[Minute];
        };

        auto SleepiestGuardThisMinute = std::max_element(GuardData.begin(), GuardData.end(), SleepiestGuardThisMinuteComp);
        SleepiestMinuteAmongAllGuards[Minute] = std::make_pair(SleepiestGuardThisMinute->first, 
                                                               SleepiestGuardThisMinute->second[Minute]);
    }

    auto SleepiestMinute = std::max_element(SleepiestMinuteAmongAllGuards.begin(), SleepiestMinuteAmongAllGuards.end(),
                            [](const auto& A, const auto& B)
                            {
                                return A.second < B.second;
                            });

    auto SleepiestMinuteNum = std::distance(SleepiestMinuteAmongAllGuards.begin(), SleepiestMinute);

    std::cout << SleepiestMinute->first << " was sleepiest, sleeping most frequently " << SleepiestMinute->second 
              << ", on " << SleepiestMinuteNum << " hashing to " << (SleepiestMinute->first * SleepiestMinuteNum) <<  std::endl;


}