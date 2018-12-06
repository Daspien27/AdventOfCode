#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <iostream>
#include <algorithm>
#include <unordered_map>

std::string Day2Puzzle2 (std::vector<std::string>& BoxIds)
{
    for (auto Iter = BoxIds.begin(); Iter != BoxIds.end(); ++Iter)
    {
        for (auto JIter =Iter+1; JIter != BoxIds.end(); ++JIter)
        {
            auto FirstMismatch = std::mismatch(Iter->begin(), Iter->end(), JIter->begin());

            if (FirstMismatch.first == Iter->end()) continue; //BoxIds are the same

            auto SecondMismatch = std::mismatch(FirstMismatch.first + 1, Iter->end(), FirstMismatch.second + 1);

            if (SecondMismatch.first != Iter->end()) continue; //Boxes have more than two points of mismatch

            std::string CommonLetters;

            std::copy (Iter->begin(), FirstMismatch.first, std::back_inserter(CommonLetters));
            std::copy (FirstMismatch.first + 1, Iter->end(), std::back_inserter(CommonLetters));

            return CommonLetters;
        }
    }

    throw std::runtime_error("No 2 Ids had just one letter mismatched.");
}

int main()
{
    std::filesystem::path input_file = "day2/day2_input.txt";


    std::ifstream fs (input_file.string(), std::ios::in);

    if (!fs.is_open())
    {
        std::runtime_error("Could not open input file!");
    }

    std::string BoxId;

    std::vector<std::string> BoxIds;

    while (std::getline(fs, BoxId))
    {
      BoxIds.push_back(BoxId);
    }
    fs.close();

    auto CommonIds = Day2Puzzle2(BoxIds);

    std::cout <<  CommonIds << std::endl;
}