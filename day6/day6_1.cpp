#include <fstream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <stack>
#include <iostream>
#include <optional>
#include <limits>

struct Coord
{
    int x = 0;
    int y = 0;
    int Id;
};

struct Node
{
    int distance {std::numeric_limits<int>::max()};
    std::optional<int> CoordId {std::nullopt};
};

void FloodFillUp (const Coord& Zone, std::vector<std::vector<Node>>& Grid)
{
    if (Zone.y < 0) return;

    if (Grid[Zone.y][Zone.x].distance > Grid[Zone.y+1][Zone.x].distance + 1)
    {
        Grid[Zone.y][Zone.x].distance = Grid[Zone.y+1][Zone.x].distance + 1;
        Grid[Zone.y][Zone.x].CoordId = Grid[Zone.y+1][Zone.x].CoordId;
    } 
    else if (Grid[Zone.y][Zone.x].distance == Grid[Zone.y+1][Zone.x].distance + 1)
    {
        Grid[Zone.y][Zone.x].CoordId = std::nullopt;
    }

    FloodFillUp(Coord{Zone.x, Zone.y-1, Zone.Id}, Grid);
}

void FloodFillRight (const Coord& Zone, std::vector<std::vector<Node>>& Grid)
{
    if (Zone.x > Grid[Zone.y].size()) return;

    if (Grid[Zone.y][Zone.x].distance > Grid[Zone.y][Zone.x-1].distance + 1)
    {
        Grid[Zone.y][Zone.x].distance = Grid[Zone.y][Zone.x-1].distance + 1;
        Grid[Zone.y][Zone.x].CoordId = Grid[Zone.y][Zone.x-1].CoordId;
    } 
    else if (Grid[Zone.y][Zone.x].distance == Grid[Zone.y][Zone.x-1].distance + 1)
    {
        Grid[Zone.y][Zone.x].CoordId = std::nullopt;
    }

    FloodFillRight(Coord{Zone.x+1, Zone.y, Zone.Id}, Grid);
}

void FloodFillLeft (const Coord& Zone, std::vector<std::vector<Node>>& Grid)
{
    if (Zone.x < 0) return;

    if (Grid[Zone.y][Zone.x].distance > Grid[Zone.y][Zone.x+1].distance + 1)
    {
        Grid[Zone.y][Zone.x].distance = Grid[Zone.y][Zone.x+1].distance + 1;
        Grid[Zone.y][Zone.x].CoordId = Grid[Zone.y][Zone.x+1].CoordId;
    } 
    else if (Grid[Zone.y][Zone.x].distance == Grid[Zone.y][Zone.x+1].distance + 1)
    {
        Grid[Zone.y][Zone.x].CoordId = std::nullopt;
    }

    FloodFillLeft(Coord{Zone.x-1, Zone.y, Zone.Id}, Grid);
}

void FloodFillRightUp (const Coord& Zone, std::vector<std::vector<Node>>& Grid)
{
    if (Zone.y > 0) FloodFillRightUp(Coord{Zone.x, Zone.y-1, Zone.Id}, Grid);
    FloodFillRight(Coord{Zone.x+1, Zone.y, Zone.Id}, Grid);
}

void FloodFillLeftUp (const Coord& Zone, std::vector<std::vector<Node>>& Grid)
{
    if (Zone.y > 0) FloodFillLeftUp(Coord{Zone.x, Zone.y-1, Zone.Id}, Grid);
    FloodFillLeft(Coord{Zone.x-1, Zone.y, Zone.Id}, Grid);
}

void FloodFillUpandAcross (const Coord& Zone, std::vector<std::vector<Node>>& Grid)
{
    FloodFillLeftUp (Coord{Zone.x-1, Zone.y, Zone.Id}, Grid);
    FloodFillRightUp (Coord{Zone.x+1, Zone.y, Zone.Id}, Grid);
    FloodFillUp (Coord{Zone.x, Zone.y-1, Zone.Id}, Grid);
}

void PerformPseudoFortunesAlg (const std::vector<Coord>& Coords, std::vector<std::vector<Node>>& Grid)
{
    auto beachline = 0;
    for (const auto& Zone : Coords)
    {
        if (Zone.y == beachline)
        {
            //add zone
            Grid[Zone.y][Zone.x].distance = 0;
            Grid[Zone.y][Zone.x].CoordId= Zone.Id;

            FloodFillUpandAcross (Zone, Grid);
        }
        else
        {
            //copy zone info down
            std::transform(Grid[beachline-1].begin(), Grid[beachline-1].end(), Grid[beachline].begin(), 
            [](const auto& Cell)
            {
                return Node{Cell.distance+1, Cell.CoordId};
            });
        }
    }
}

int GetLargestEnclosedRegionSize (const std::vector<Coord>& Coords)
{
    const auto min_x_coord = std::min_element(Coords.begin(), Coords.end(), [](const auto& A, const auto& B) { return A.x < B.x; });
    const auto max_x_coord = std::max_element(Coords.begin(), Coords.end(), [](const auto& A, const auto& B) { return A.x < B.x; });
    const auto min_y_coord = std::min_element(Coords.begin(), Coords.end(), [](const auto& A, const auto& B) { return A.y < B.y; });
    const auto max_y_coord = std::max_element(Coords.begin(), Coords.end(), [](const auto& A, const auto& B) { return A.y < B.y; });

    const auto span_x = max_x_coord->x - max_y_coord->x;
    const auto span_y = max_y_coord->y - min_y_coord->y;

    std::vector<Coord> NormalizedCoords = Coords;

    for (auto& Elem : NormalizedCoords)
    {
        Elem.x -= span_x;
        Elem.y -= span_y;
    }

    auto Grid = std::vector<std::vector<Node>>(span_y, std::vector<Node> (span_x));

    PerformPseudoFortunesAlg (Coords, Grid);

    //FindSizeOfLargestEnclosedRegionInGrid(Grid);
}

int main()
{
    std::filesystem::path input_file = "day6/day6_input.txt";


    std::ifstream fs (input_file.string(), std::ios::in);

    if (!fs.is_open())
    {
        std::runtime_error("Could not open input file!");
    }

    std::vector<Coord> Coords;
    std::string CoordString;
    int Id = 0;

    while (std::getline(fs, CoordString))
    {
        auto BeginSecondCoord = std::find(CoordString.begin(), CoordString.end(), ',');

        std::string xcoord_str (CoordString.begin(), BeginSecondCoord);
        std::string ycoord_str (BeginSecondCoord+1, CoordString.end());

        Coords.push_back(Coord{atoi(xcoord_str.c_str()), atoi(ycoord_str.c_str()), ++Id});
    }
    fs.close();

    std::sort(Coords.begin(), Coords.end(), [](const auto& A, const auto& B)
    {
        return A.y < B.y || (A.y == B.y && A.x < B.x);
    });

    std::cout << GetLargestEnclosedRegionSize (Coords) <<  std::endl;
}