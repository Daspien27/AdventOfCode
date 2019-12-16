#include <iostream>
#include <fstream>
#include <string>
#include <set>

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
        std::set<std::pair<int, int>> houses_visited;
        
        houses_visited.emplace (0, 0);

        std::string santa_movement;
        std::getline(in, santa_movement);

        track_santa_movement (houses_visited, santa_movement);
      
        std::cout << "Santa traversed " << houses_visited.size () << " houses." << std::endl;

    }
}
