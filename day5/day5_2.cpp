#include <fstream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <stack>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

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

int LengthOfRemovedPolymer(const std::string& polymer_sequence, char polymer)
{
    std::string RemovedPolymer = polymer_sequence;
    auto RemoveStart = std::remove_if(RemovedPolymer.begin(), RemovedPolymer.end(), [polymer](auto A) { return A == toupper(polymer) || A == tolower(polymer);});
    RemovedPolymer.erase(RemoveStart, RemovedPolymer.end());

    return LengthOfPolymer(RemovedPolymer);
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

    std::string unique_polymers = polymer_sequence;
    std::transform(unique_polymers.begin(), unique_polymers.end(), unique_polymers.begin(), toupper);
    
    std::unordered_set<char> UniquePolymers (unique_polymers.begin(), unique_polymers.end());
    std::unordered_map<char, int> RemovedPolymersLength;

    for (const auto& polymer : UniquePolymers)
    {
        RemovedPolymersLength[polymer] = LengthOfRemovedPolymer(polymer_sequence, polymer);
    }

    auto Min = std::min_element(RemovedPolymersLength.begin(), RemovedPolymersLength.end(), [](const auto& A, const auto& B) { return A.second < B.second; });

    std::cout << Min->second <<  std::endl;
}