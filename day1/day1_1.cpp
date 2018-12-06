#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <iostream>

int main()
{
    std::filesystem::path input_file = "day1/day1_input.txt";


    std::ifstream fs (input_file.string(), std::ios::in);

    if (!fs.is_open())
    {
        std::runtime_error("Could not open input file!");
    }

    auto Frequency = 0l;
    std::string Reading;

    while (std::getline(fs, Reading))
    {
        int Sign = (*Reading.begin() == '+') ? 1.0 : -1.0;
        
        auto Magnitude = atol (Reading.data()+1);

        Frequency += Sign * Magnitude;
    }
    fs.close();

    std::cout << Frequency << std::endl;
}