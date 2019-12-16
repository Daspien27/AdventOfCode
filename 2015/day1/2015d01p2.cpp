#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

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
     
        auto first_basement = std::find_if(input.begin(), input.end(), [floor = 0](auto c) mutable {
            (c == '(') ? ++floor : --floor;
            return floor == -1;            
        });

        auto distance = std::distance(input.begin (), first_basement) + 1;
        std::cout << "Santa took " << distance << " traversals to reach floor -1." << std::endl;
    }
}
