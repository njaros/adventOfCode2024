#include "utils/helpers.hpp"

Coord dirs[] = {Coord(1, 0), Coord(0, 1), Coord(-1, 0), Coord(0, -1)};

int main() {
    ui part = 0;
    std::ifstream input;
    Grid<int> grid;
    std::string line;
    Coord goal;
    Coord current;
    std::set<Coord> walls;
    std::vector<std::pair<Coord, int>> racePos;
    ui res = 0;

    if (getFileAndPart(20, input, part))
        return errno;

    while (!input.eof()) {
        grid.addEmptyLine();
        getline(input, line);
        for (char c: line) {
            switch (c)
            {
            case '.':
                grid.appendInLastLine(0);
                break;
            
            case '#':
                grid.appendInLastLine(-1);
                break;

            case 'E':
                grid.appendInLastLine(-2);
                break;
            
            case 'S':
                grid.appendInLastLine(1);
                break;

            default:
                break;
            }
        }
    }

    goal = grid.findOne(-2).value();
    grid.get(goal) = 0;
    current = grid.findOne(1).value();
    walls = grid.findAll(-1);

    while (current != goal) {
        for (Coord dir: dirs) {
            if (grid.inBounds(current + dir) && grid.get(current + dir) == 0) {
                grid.get(current + dir) = grid.get(current) + 1;
                current += dir;
                break;
            }
        }
    }

    if (part == 1) {
        for (Coord wall: walls) {
            Coord right = wall + dirs[0];
            Coord left = wall + dirs[2];
            Coord up = wall + dirs[3];
            Coord down = wall + dirs[1];
            if (
                grid.inBounds(up) && grid.inBounds(down) &&
                grid.get(up) != -1 && grid.get(down) != -1 &&
                abs(grid.get(up) - grid.get(down)) > 100
            ) {
                res += 1;
            }
            else if (
                grid.inBounds(right) && grid.inBounds(left) &&
                grid.get(right) != -1 && grid.get(left) != -1 &&
                abs(grid.get(right) - grid.get(left)) > 100 
            ) {
                res += 1;
            }
        }
    }

    else {
        Grid<int>::browser br = std::nullopt;
        br = grid.lineBrowse(br);
        while (br.has_value()) {
            if (*(std::get<2>(br.value())) != -1)
                racePos.push_back(std::make_pair(std::get<0>(br.value()), *(std::get<2>(br.value()))));
            br = grid.lineBrowse(br);
        }
        ui max = grid.get(goal);
        for (std::pair<Coord, int> elt : racePos) {
            for (std::pair<Coord, int> elt2 : racePos) {
                int dist = math::ManhattanDist(elt2.first, elt.first);
                if (dist <= 20 && elt2.second - elt.second - dist >= 100) {
                    res++;
                }
            }
        }
    }

    std::cout << "result is " << res << '\n';

    return 0;
}