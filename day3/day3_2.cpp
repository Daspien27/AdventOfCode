#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <iostream>
#include <algorithm>
#include <map>
#include <functional>
#include <set>
#include <optional>

struct Claim
{
    int ID;
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
    int ID {};
};

Claim GetClaimFromString(const std::string& StringClaim)
{
    auto BeginId = std::find(StringClaim.begin(), StringClaim.end(), '#');
    auto BeginXCoord = std::find(StringClaim.begin(), StringClaim.end(), '@');
    auto BeginYCoord = std::find(StringClaim.begin(), StringClaim.end(), ',');
    auto BeginWidth = std::find(StringClaim.begin(), StringClaim.end(), ':');
    auto BeginHeight = std::find(StringClaim.begin(), StringClaim.end(), 'x');


    std::string id_str (BeginId+1, BeginXCoord);
    std::string xcoord_str (BeginXCoord+1, BeginYCoord);
    std::string ycoord_str (BeginYCoord+1, BeginWidth);
    std::string width_str (BeginWidth+1, BeginHeight);
    std::string height_str (BeginHeight+1, StringClaim.end());


    return {atoi(id_str.c_str()), atoi(xcoord_str.c_str()), atoi(ycoord_str.c_str()), atoi(width_str.c_str()), atoi(height_str.c_str())};
}

void FindCoincidingIDs (const std::multimap<AreaPair, int, std::function<bool(const AreaPair&, const AreaPair&)>>& Sweepline, std::set<int>& CoincidingIDs)
{
    std::set<int> CurrentIDs;

    for (const auto& AreaPair : Sweepline)
    {
        if (AreaPair.first.state == Area::BEGIN)
        {
            CurrentIDs.insert(AreaPair.first.ID);
        }
        else if (AreaPair.first.state == Area::END)
        {
            CurrentIDs.erase(AreaPair.first.ID);
        }

        if (CurrentIDs.size() > 1)
        {
            CoincidingIDs.insert(CurrentIDs.begin(), CurrentIDs.end());
        }
    }
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
 
    std::set<int> CoincidingIDs {};

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
                    FindCoincidingIDs (Sweepline, CoincidingIDs);
                    DecreaseLife (Sweepline);
                }
            }

            Sweepline.emplace(AreaPair{ElfClaim.y_coord, Area::BEGIN, ElfClaim.ID}, ElfClaim.width);
            Sweepline.emplace(AreaPair{ElfClaim.y_coord + ElfClaim.height, Area::END, ElfClaim.ID}, ElfClaim.width);
        }

        while (!Sweepline.empty())
        {
            FindCoincidingIDs (Sweepline, CoincidingIDs);
            DecreaseLife (Sweepline);
        }
    }

    auto FirstNonConsecutive = std::adjacent_find (CoincidingIDs.begin(), CoincidingIDs.end(), [](const auto& A, const auto& B) { return B - A != 1; });

    std::cout << CoincidingIDs.size() << " out of " << Claims.size() << ": " << (*FirstNonConsecutive) + 1 << '\n';

    fs.close();


}