#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <optional>
#include <array>
#include <numeric>
#include <sstream>
#include <variant>

enum class LightCommand
{
    ON,
    OFF,
    TOGGLE,
};

class Rect
{
friend std::vector<Rect> get_sub_rects (const Rect& A, const Rect& B);

    int x_min;
    int y_min;
    int x_max; //exclusive max
    int y_max;

public:
    Rect(int x_min_use, int y_min_use, int x_max_use, int y_max_use) : x_min(x_min_use),
                                                                       y_min(y_min_use),
                                                                       x_max(x_max_use),
                                                                       y_max(y_max_use)
    {
        if (x_max < x_min || y_max < y_min)
        {
            throw std::runtime_error("A rectangle's upper bound must not be smaller than its lower bound.");
        }
    }

    Rect (const Rect&) = default;
    Rect (Rect &&) = default;
    Rect& operator= (const Rect&) = default;
    Rect& operator= (Rect&&) = default;

    int size () const
    {
        return (x_max - x_min) * (y_max - y_min);
    }


    bool empty () const
    {
        return size () == 0;
    }

    bool contains_point(int x, int y) const
    {
        return (x_min <= x && x < x_max) && (y_min <= y && y < y_max);
    }

    bool contains_rect(const Rect& r) const
    {
        return (x_min <= r.x_min && r.x_max <= x_max) && (y_min <= r.y_min && r.y_max <= y_max);
    }

    bool overlaps_with(const Rect& r) const
    {
        return ((x_min <= r.x_min && r.x_min < x_max) || (r.x_min <= x_min && x_min < r.x_max))
            && ((y_min <= r.y_min && r.y_min < y_max) || (r.y_min <= y_min && y_min < r.y_max));              
    }
};

struct Command
{
    LightCommand command;
    Rect area;
};

template <LightCommand CMD>
struct CommandMatch;

template <>
struct CommandMatch<LightCommand::ON>
{
    constexpr static auto regex = R"(^turn on (\d+),(\d+) through (\d+),(\d+))";
};

template <>
struct CommandMatch<LightCommand::OFF>
{
    constexpr static auto regex = R"(^turn off (\d+),(\d+) through (\d+),(\d+))";
};

template <>
struct CommandMatch<LightCommand::TOGGLE>
{
    constexpr static auto regex = R"(^toggle (\d+),(\d+) through (\d+),(\d+))";
};

template <LightCommand CMD>
std::optional<Command> retrieve_command(const std::string &input)
{
    const auto regex = std::regex(CommandMatch<CMD>::regex);
    const auto match_end = std::sregex_iterator();
    
    auto match = std::sregex_iterator(input.begin(), input.end(), regex);

    if (match != match_end)
    {
        std::smatch m = *match;

        return Command{CMD, Rect{std::stoi(m[1]),
                                 std::stoi(m[2]), 
                                 std::stoi(m[3]) + 1, //exclusive maxes
                                 std::stoi(m[4]) + 1}};
    }
    else
    {
        return std::nullopt;
    }
}


auto get_commands_from_file (std::string_view file)
{
    std::fstream in = std::fstream(file.data (), std::fstream::in);

    if (!in.is_open())
    {
        throw std::runtime_error ("Unable to open input.txt");
    }
    else
    {
        std::vector<Command> commands;

        while (!in.eof())
        {
            std::string input;
            std::getline(in, input);

            if (auto OnMatch = retrieve_command<LightCommand::ON>(input))
            {
                commands.push_back(OnMatch.value());
            }
            else if (auto OffMatch = retrieve_command<LightCommand::OFF>(input))
            {
                commands.push_back(OffMatch.value());
            }
            else if (auto ToggleMatch = retrieve_command<LightCommand::TOGGLE>(input))
            {
                commands.push_back(ToggleMatch.value());
            }
        }

        return commands;
    }
}


std::vector<Rect> get_sub_rects (const Rect& A, const Rect& B)
{
    std::array<int, 4> xs = {A.x_min, A.x_max, B.x_min, B.x_max};
    std::sort (xs.begin (), xs.end ());
    std::array<int, 4> ys = {A.y_min, A.y_max, B.y_min, B.y_max};
    std::sort (ys.begin (), ys.end ());

    std::array<Rect, 9> sub_rects = {
        Rect {xs[0], ys[0], xs[1], ys[1]}, Rect {xs[1], ys[0], xs[2], ys[1]}, Rect {xs[2], ys[0], xs[3], ys[1]},
        Rect {xs[0], ys[1], xs[1], ys[2]}, Rect {xs[1], ys[1], xs[2], ys[2]}, Rect {xs[2], ys[1], xs[3], ys[2]},
        Rect {xs[0], ys[2], xs[1], ys[3]}, Rect {xs[1], ys[2], xs[2], ys[3]}, Rect {xs[2], ys[2], xs[3], ys[3]}
    };

    std::vector<Rect> result;

    std::copy_if (sub_rects.begin (), sub_rects.end (), std::back_inserter(result), [&A, &B] (const auto& sub_rect)
    {
        return !sub_rect.empty () && (A.contains_rect(sub_rect) || B.contains_rect (sub_rect));
    });

    return result;
}


void perform_command (Command cmd, std::vector<Rect>& lights)
{  
    std::vector<Rect> overlapping_lights;
    std::vector<Rect> nonoverlapping_lights;

    const auto command_contains = [cmd](const auto& light)
    {
        return light.overlaps_with (cmd.area);
    };

    std::copy_if(lights.cbegin (), lights.cend (), std::back_inserter(overlapping_lights), command_contains);
    std::remove_copy_if(lights.cbegin (), lights.cend (), std::back_inserter(nonoverlapping_lights), command_contains);

    lights = nonoverlapping_lights;

    auto commands = std::vector<Command> {cmd};
    
    while (!overlapping_lights.empty () || !commands.empty ())
    {
        if (!commands.empty ())
        {
            auto command = commands.back ();
            commands.pop_back ();
            
            auto first_overlapping_light = std::find_if(overlapping_lights.begin (), overlapping_lights.end (), [command] (const auto& light)
            {
                return light.overlaps_with (command.area);
            });

            if (first_overlapping_light != overlapping_lights.end ())
            {
                auto light = *first_overlapping_light;

                overlapping_lights.erase (first_overlapping_light); //remove the lights from the list

                auto sub_rects = get_sub_rects(command.area, light);

                for (auto& sub_rect : sub_rects)
                {
                    auto is_in_command = command.area.contains_rect(sub_rect);
                    auto is_in_light = light.contains_rect(sub_rect);

                    if (is_in_command && is_in_light)
                    {
                        // we can process the command as there can be no other light that will intersect with the area
                        switch (command.command)
                        {
                            case LightCommand::ON: lights.push_back (sub_rect); break;
                            case LightCommand::OFF:                             break; //Don't reinsert lights
                            case LightCommand::TOGGLE:                          break;
                        }
                    }
                    else if (is_in_light)
                    {
                        lights.push_back(sub_rect); //no other light has this light
                    }
                    else if (is_in_command)
                    {
                        commands.push_back (Command{command.command, sub_rect});
                    }
                    else
                    {
                        throw std::runtime_error ("get_sub_rect returned a faulty result.");
                    }
                }
            }
            else //This command is for sections of lights that are not on
            {
                std::vector<Rect> new_lights;

                switch (command.command)
                {
                    case LightCommand::ON:     new_lights.push_back (command.area); break; // Add lights
                    case LightCommand::OFF:                                         break; // Don't reinsert lights
                    case LightCommand::TOGGLE: new_lights.push_back (command.area); break; // Lights are now on
                }

                while (!new_lights.empty ())
                {
                    auto new_light = new_lights.back ();
                    new_lights.pop_back ();

                    const auto overlaps_with = [new_light] (const auto& light)
                    {
                        return light.overlaps_with(new_light);
                    };

                    auto iter = std::find_if(lights.begin (), lights.end (), overlaps_with);
                    if (iter == lights.end ())
                    {
                        lights.push_back (new_light);
                    }
                    else
                    {
                        auto old_light = *iter;
                        lights.erase(iter);
                        auto sub_lights = get_sub_rects (old_light, new_light);
                        new_lights.insert (new_lights.end (), sub_lights.begin (), sub_lights.end ());
                    }
                }
            }
            
        }
        else
        {
            lights.insert (lights.end (), overlapping_lights.begin (), overlapping_lights.end ());
            overlapping_lights.clear ();
        }
    }

    //Merge all lights together

}


struct LightRectTree
{
    struct Leaf;
    struct Node;
    using VarNode = std::variant<std::unique_ptr<Node>, Leaf>;

    struct Leaf
    {
        Rect leaf;
    };
    
    struct Node
    {
        std::vector<VarNode> branches;
    };

    VarNode root;
};


int main(int, char **)
{
    auto commands = get_commands_from_file ("input.txt"); //959255 is wrong
    //auto commands = get_commands_from_file ("input.1.txt"); //569999 is right


    std::cout << "Processing " << commands.size () << " commands." << std::endl;

    auto lights = std::vector<Rect> ();

    std::vector<std::string> log;

    for (auto cmd : commands)
    {
        perform_command (cmd, lights);

        auto active_lights = std::accumulate (lights.begin (), lights.end (), 0, [](const auto sum, const auto& light)
        {
            return sum + light.size ();
        });

        std::stringstream ss;

        ss << "There are " << active_lights << " lights lit";

        log.push_back(ss.str ());
    }

    auto log_iter = log.begin ();

    while (log_iter != log.end ())
    {
        auto log_iter_next = std::adjacent_find (log_iter, log.end (), [](const auto& a, const auto& b)
        {
            return a != b;
        });

        auto dist = std::distance (log_iter, (log_iter_next != log.end ()) ? log_iter_next+1 : log.end ());

        if (dist > 1)
        {
            std::cout << "(" << dist << ") ";
        }
          
        std::cout << *log_iter << "\n";

        if (log_iter_next == log.end ()) break;

        log_iter = std::next (log_iter_next);
    }

    auto active_lights = std::accumulate (lights.begin (), lights.end (), 0, [](const auto sum, const auto& light)
    {
        return sum + light.size ();
    });

    std::cout << "There are " << active_lights << " lights turned on." << std::endl;
}
