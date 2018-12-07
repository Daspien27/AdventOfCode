#include <fstream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <stack>
#include <iostream>

bool polymers_react (char A, char B)
{
    return (A != B && (toupper(A) == B || tolower(A) == B));
}


int LengthOfPolymer (const std::string& polymer_sequence)
{
    std::stack<char> Reactions;


    for (const auto polymer : polymer_sequence)
    {
        if (Reactions.empty())
        {
            Reactions.push(polymer);
        }
        else if (polymers_react(polymer, Reactions.top()))
        {
            Reactions.pop();
        }
        else
        {
            Reactions.push(polymer);
        }
    }

    return Reactions.size();
}

int main()
{
    std::filesystem::path input_file = "day5/day5_input.txt";


    std::ifstream fs (input_file.string(), std::ios::in);

    if (!fs.is_open())
    {
        std::runtime_error("Could not open input file!");
    }

    std::string polymer_sequence;
    
    std::getline(fs, polymer_sequence);

    fs.close();

    std::cout << LengthOfPolymer(polymer_sequence) <<  std::endl;
}