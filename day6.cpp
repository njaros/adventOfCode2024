#include "utils/helpers.hpp"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

typedef std::pair<Coord, int> Guard;

Guard turn(Guard g) {
    return std::make_pair(g.first, ((g.second + 1) % 4));
}

Guard next(Guard guard, const Grid<char>& grid) {
    Coord moves[] = {
        Coord(0, -1),
        Coord(1, 0),
        Coord(0, 1),
        Coord(-1, 0)
        };
    
    Guard nextGuard = std::make_pair(guard.first + moves[guard.second], guard.second);
    if (grid.inBounds(nextGuard.first) && grid.get(nextGuard.first) == '#') {
        return turn(guard);
    }
    return nextGuard;
}

bool addObstruction(Coord obs, Guard guard, Grid<char>& grid) {
    std::set<Guard> visited;
    
    grid.get(obs) = '#';
    while (grid.inBounds(guard.first)) {
        guard = next(guard, grid);
        if (!visited.insert(guard).second)
        {
            grid.get(obs) = '.';
            return true;
        }
    }
    grid.get(obs) = '.';
    return false;
}

int main () {
    ui part = 0;
    std::ifstream input;
    Grid<char> grid;
    std::string line;
    std::set<Coord> visited;
    Guard guard;
    ui resPart2 = 0;

    if (getFileAndPart(6, input, part))
        return errno;

    while (!input.eof()) {
        getline(input, line);
        grid.addBackLine(line);
    }

    guard = std::make_pair(grid.findOne('^').value(), UP);
    while (grid.inBounds(guard.first)) {
        visited.insert(guard.first);
        guard = next(guard, grid);
    }

    if (part == 1)
        std::cout << "result is " << visited.size() << '\n';
    else {
        guard = std::make_pair(grid.findOne('^').value(), UP);
        visited.erase(guard.first);
        for (Coord c: visited) {
            resPart2 += addObstruction(c, guard, grid);
        }

        std::cout << "result is " << resPart2 << '\n';
    }

    return 0;
}