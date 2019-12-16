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

        auto up_counts = std::count (input.begin (), input.end (), '(');
        auto down_counts = std::count (input.begin (), input.end (), ')');
      
        std::cout << "Santa is now on floor " << up_counts - down_counts << "." << std::endl;

    }
}
