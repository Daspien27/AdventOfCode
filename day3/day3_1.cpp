#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <iostream>
#include <algorithm>
#include <map>
#include <functional>

struct Claim
{
    int x_coord {};
    int y_coord {};
    int width {};
    int height {};
};

enum class Area
{
    UNDEFINED,
    BEGIN,
    END
};

struct AreaPair
{
    int coord {};
    Area state {Area::UNDEFINED};
};

Claim GetClaimFromString(const std::string& StringClaim)
{
    auto BeginXCoord = std::find(StringClaim.begin(), StringClaim.end(), '@');
    auto BeginYCoord = std::find(StringClaim.begin(), StringClaim.end(), ',');
    auto BeginWidth = std::find(StringClaim.begin(), StringClaim.end(), ':');
    auto BeginHeight = std::find(StringClaim.begin(), StringClaim.end(), 'x');

    std::string xcoord_str (BeginXCoord+1, BeginYCoord);
    std::string ycoord_str (BeginYCoord+1, BeginWidth);
    std::string width_str (BeginWidth+1, BeginHeight);
    std::string height_str (BeginHeight+1, StringClaim.end());


    return {atoi(xcoord_str.c_str()), atoi(ycoord_str.c_str()), atoi(width_str.c_str()), atoi(height_str.c_str())};
}

int CountDoublyOccupiedSquares (const std::multimap<AreaPair, int, std::function<bool(const AreaPair&, const AreaPair&)>>& Sweepline)
{
    int CurrentOccupied = 0;
    int NumDoublyOccupied = 0;
    int yCoord = 0;
    for (const auto& AreaPair : Sweepline)
    {
        if (AreaPair.first.state == Area::BEGIN)
        {
            CurrentOccupied++;
            if (CurrentOccupied == 2) yCoord = AreaPair.first.coord;
        }
        else if (AreaPair.first.state == Area::END)
        {
            CurrentOccupied--;
            if (CurrentOccupied == 1)
            {
                NumDoublyOccupied += AreaPair.first.coord - yCoord;
            }
        }
    }

    return NumDoublyOccupied; 
}

void DecreaseLife (std::multimap<AreaPair, int, std::function<bool(const AreaPair&, const AreaPair&)>>& Sweepline)
{
    for (auto& Val : Sweepline)
    {
        Val.second--;
    }

    auto Iter = std::find_if(Sweepline.begin(), Sweepline.end(), [](const auto& A) {return A.second == 0;});

    while (Iter != Sweepline.end())
    {
        Iter = Sweepline.erase(Iter);
        Iter = std::find_if(Iter, Sweepline.end(), [](const auto& A) {return A.second == 0;});
    }
}

int main()
{
    std::filesystem::path input_file = "day3/day3_input.txt";


    std::ifstream fs (input_file.string(), std::ios::in);

    if (!fs.is_open())
    {
        std::runtime_error("Could not open input file!");
    }

    std::vector<Claim> Claims;
    std::string StringClaim;

    while (std::getline(fs, StringClaim))
    {
      Claims.push_back(GetClaimFromString(StringClaim));
    }

    std::sort(Claims.begin(), Claims.end(), [](const auto& A, const auto& B) { return A.x_coord < B.x_coord; });
    
    auto Sweepline = std::multimap<AreaPair, int, std::function<bool(const AreaPair&, const AreaPair&)>> {
        [](const AreaPair& A, const AreaPair& B)
        {
            return A.coord < B.coord;
        }};

    
    int TotalCommonSquares = 0;
    
    if (!Claims.empty())
    {
        int SweeplineXCoord = Claims.begin()->x_coord;
        for (const auto& ElfClaim : Claims)
        {
            if (ElfClaim.x_coord != SweeplineXCoord)
            {
                //Increment Sweepline
                int DistanceSwept = ElfClaim.x_coord - SweeplineXCoord;
                SweeplineXCoord = ElfClaim.x_coord;

                for (int i = 0; i < DistanceSwept; i++)
                {
                    TotalCommonSquares += CountDoublyOccupiedSquares (Sweepline);
                    DecreaseLife (Sweepline);
                }
            }

            Sweepline.emplace(AreaPair{ElfClaim.y_coord, Area::BEGIN}, ElfClaim.width);
            Sweepline.emplace(AreaPair{ElfClaim.y_coord + ElfClaim.height, Area::END}, ElfClaim.width);
        }

        while (!Sweepline.empty())
        {
            TotalCommonSquares += CountDoublyOccupiedSquares (Sweepline);
            DecreaseLife (Sweepline);
        }
    }

    std::cout << TotalCommonSquares << '\n';

    fs.close();


}