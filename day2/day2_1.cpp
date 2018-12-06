#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <iostream>
#include <algorithm>
#include <unordered_map>

int main()
{
    std::filesystem::path input_file = "day2/day2_input.txt";


    std::ifstream fs (input_file.string(), std::ios::in);

    if (!fs.is_open())
    {
        std::runtime_error("Could not open input file!");
    }

    auto Twos = 0l;   //Exactly 2 of one letter
    auto Threes = 0l; //Exactly 3 of one letter
    std::string BoxId;

    while (std::getline(fs, BoxId))
    {
      std::unordered_map<char, long> SortedBoxId;
      
      for (const auto& c : BoxId) SortedBoxId[c]++;

      auto IterTwo = std::find_if (SortedBoxId.begin(), SortedBoxId.end(), [](const auto& P) { return P.second == 2; });
      auto IterThree = std::find_if (SortedBoxId.begin(), SortedBoxId.end(), [](const auto& P) { return P.second == 3; });

      if (IterTwo != SortedBoxId.end()) Twos++;
      if (IterThree != SortedBoxId.end()) Threes++;
    }
    fs.close();

    std::cout << Twos * Threes << std::endl;
}