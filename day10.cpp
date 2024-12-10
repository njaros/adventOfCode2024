#include "utils/helpers.hpp"

Coord dirs[] = {Coord(0, 1), Coord(0, -1), Coord(1, 0), Coord(-1, 0)};

ui hikingScore(Coord current, int value, const Grid<char>& grid, std::set<Coord>& arrivals) {
    ui partScore = 0;
    
    if (value == 9)
    {
        arrivals.insert(current);
        return 1;
    }
    else {
        for (Coord dir: dirs) {
            if (
                grid.inBounds(current + dir) &&
                grid.get(current + dir) - '0' == value + 1
            )
            partScore += hikingScore(current + dir, value + 1, grid, arrivals);
        }
    }
    return partScore;
}

int main() {
    ui part = 5;
    std::ifstream input;
    Grid<char> grid;
    std::string line;
    std::set<Coord> starts;
    std::set<Coord> arrivals;
    ui res = 0;
    ui resP2 = 0;;

    if (getFileAndPart(10, input, part))
        return errno;

    while (!input.eof()) {
        getline(input, line);
        grid.addBackLine(line);
    }

    starts = grid.findAll('0');

    for (Coord start: starts) {
        arrivals.clear();
        resP2 += hikingScore(start, 0, grid, arrivals);
        res += arrivals.size();
    }

    std::cout << "result P1 is " << res << '\n';
    std::cout << "result P2 is " << resP2 << '\n';


    return 0;
}