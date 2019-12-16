#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <numeric>

void track_santa_movement (std::set<std::pair<int, int>>& houses_visited, const std::string& santa_movement)
{
    std::pair current_loc = {0, 0};
    for (auto c : santa_movement)
    {
        switch (c)
        {
            case '>': ++current_loc.first; break;
            case '<': --current_loc.first; break;
            case 'v': ++current_loc.second; break;
            case '^': --current_loc.second; break;
        }

        houses_visited.insert (current_loc);
    }
}

int main(int, char**) {

    std::fstream in = std::fstream("input.txt", std::fstream::in);

    if (!in.is_open())
    {
        std::cerr << "Unable to open input.txt" << std::endl;
        return -1; 
    }
    else
    {
        std::string input;
        std::getline(in, input);

        struct
        {
            std::string santa;
            std::string robo_santa;
        } inputs;

        auto robo_santas_turn = false;
        for (auto c : input)
        {
            if (robo_santas_turn)
            {
                inputs.robo_santa += c;
            }
            else
            {
                inputs.santa +=c;
            }

            robo_santas_turn = !robo_santas_turn;
        }

        std::set<std::pair<int, int>> houses_visited;
        
        houses_visited.emplace (0, 0);

        track_santa_movement (houses_visited, inputs.santa);
        track_santa_movement (houses_visited, inputs.robo_santa);

        std::cout << "Santa and Robo-Santa traversed " << houses_visited.size () << " houses." << std::endl;

    }
}
