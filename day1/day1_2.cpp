#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <iostream>
#include <set>

int main()
{
    const std::filesystem::path input_file = "day1/day1_input.txt";

    auto Frequency = 0l;
    std::string Reading;

    std::set<long> VisitedFrequencies = {0};
    bool NoRepeatedFrequency = true;
    do
    {
        std::ifstream fs (input_file.string(), std::ios::in);

        if (!fs.is_open())
        {
            std::runtime_error("Could not open input file!");
        }

        while (std::getline(fs, Reading))
        {
            int Sign = (*Reading.begin() == '+') ? 1.0 : -1.0;

            auto Magnitude = atol (Reading.data()+1);

            Frequency += Sign * Magnitude;
            auto Res = VisitedFrequencies.insert(Frequency);

            if (!Res.second)
            {
                NoRepeatedFrequency = false;
                break;
            }

        }

        fs.close();
    } while (NoRepeatedFrequency);

    std::cout << Frequency << std::endl;
}