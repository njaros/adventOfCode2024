#include "utils/helpers.hpp"

bool checkXmasDir(Coord elt, Coord dir, const Grid<char>& grid) {
    int combo = 0;
    char xmas[] = {'X', 'M', 'A', 'S'};

    while (grid.inBounds(elt)) {
        if (grid.get(elt) == xmas[combo])
            ++combo;
        else
            return false;
        if (combo == 4)
            return true;
        elt += dir;
    }
    return false;
}

ui checkXmas(Coord elt, const Grid<char>& grid) {
    ui res = 0;
    
    Coord directions[] = {
        Coord(0, 1),
        Coord(0, -1),
        Coord(1, 0),
        Coord(-1, 0),
        Coord(1, 1),
        Coord(1, -1),
        Coord(-1, 1),
        Coord(-1, -1)
        };
    
    for (Coord dir: directions) {
        res += checkXmasDir(elt, dir, grid);
    }

    return res;
}

bool checkX_Mas(Coord elt, const Grid<char>& grid) {
    std::set<char> correcSet = {'M', 'S'};
    std::set<char> read;
    Coord d1[] = {Coord(-1, -1), Coord(1, 1)};
    Coord d2[] = {Coord(-1, 1), Coord(1, -1)};
    for (Coord d: d1) {
        if (grid.inBounds(d + elt))
            read.insert(grid.get(d + elt));
        else
            return false;
    }
    if (read != correcSet)
        return false;
    read.clear();
    for (Coord d: d2) {
        if (grid.inBounds(d + elt))
            read.insert(grid.get(d + elt));
        else
            return false;
    }
    if (read != correcSet)
        return false;
    return true;
}

int main() {
    Grid<char> grid;
    std::ifstream input;
    ui part = 0;
    ui res = 0;
    std::string line;
    std::set<Coord> found;

    if (getFileAndPart(4, input, part))
        return errno;

    while (!input.eof()) {
        getline(input, line);
        grid.addBackLine(line);
    }

    if (part == 1) {
        found = grid.findAll('X');
        for (Coord elt: found) {
            res += checkXmas(elt, grid);
        }
    }

    else {
        found = grid.findAll('A');
        for (Coord elt: found) {
            res += checkX_Mas(elt, grid);
        }
    }

    std::cout << "result is " << res << '\n';

    return 0;
}