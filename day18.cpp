#include "utils/helpers.hpp"

Coord goal(70, 70);
Coord dirs[] = {Coord(1, 0), Coord(0, 1), Coord(-1, 0), Coord(0, -1)};

class Position {

    public:

    Coord _pos;
    int _potential;
    int _score;

    Position(): _pos(0, 0), _potential(math::ManhattanDist(_pos, goal)), _score(0) {}

    Position(Coord pos, int score): _pos(pos), _potential(score + math::ManhattanDist(_pos, goal)), _score(score) {}

    Position(const Position& o): _pos(o._pos), _potential(o._potential), _score(o._score) {}
    
};


bool operator<(const Position& p1, const Position& p2) {
    if (p1._potential < p2._potential)
        return true;
    if (p1._potential > p2._potential)
        return false;
    if (p1._score < p2._score)
        return true;
    if (p1._score > p2._score)
        return false;
    return (p1._pos < p2._pos);
}

typedef std::map<Coord, Position> CloseSet;
typedef std::set<Position> OpenSet;

void insertSafe(CloseSet& close, OpenSet& open, Position pos) {
    CloseSet::iterator found = close.find(pos._pos);
    
    if (found != close.end()) {
        if (found->second._score > pos._score) {
            found->second = pos;
            open.insert(pos);
        }
    }
    else {
        close[pos._pos] = pos;
        open.insert(pos);
    }
}

bool AStar(Grid<char>& grid, ui part) {
    Position start;
    OpenSet open;
    CloseSet close;
    OpenSet::iterator current;

    open.insert(start);
    current = open.begin();

    while (current->_pos != goal && !open.empty()) {

        for (Coord dir : dirs) {
            if (grid.inBounds(current->_pos + dir) && grid.get(current->_pos + dir) == '.')
                insertSafe(close, open, Position(current->_pos + dir, current->_score + 1));
        }

        open.erase(current);
        current = open.begin();

    }

    if (part == 1) {
        std::cout << "result is " << current->_score << '\n';
        return true;
    }
    else
        return current->_pos == goal;
}

int countNeighbour(const Grid<char>& g, Coord c) {
    int count = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i != j) {
                if (!g.inBounds(c + Coord(i, j)))
                    ++count;
                else if (g.get(c + Coord(i, j)) == '#')
                    ++count;
            }
        }
    }
    return count;
}

int main() {
    ui part = 0;
    std::ifstream input;
    Grid<char> grid;
    Coord read;

    for (int i = 0; i < 71; i++)
        grid.addBackLine(std::vector<char>(71, '.'));


    if (getFileAndPart(18, input, part))
        return errno;

    for (int i = 0; i < 1024; i++) {
        read.first = inputLib::extractNextNumber(input).first.value();
        read.second = inputLib::extractNextNumber(input).first.value();
        grid.get(read) = '#';
    }

    // std::cout << grid;

    if (part == 1)
        AStar(grid, part);
    
    else {
        while (AStar(grid, part)) {
            read.first = inputLib::extractNextNumber(input).first.value();
            read.second = inputLib::extractNextNumber(input).first.value();
            grid.get(read) = '#';
            while (countNeighbour(grid, read) < 1) {
                read.first = inputLib::extractNextNumber(input).first.value();
                read.second = inputLib::extractNextNumber(input).first.value();
                grid.get(read) = '#';
            }
            // std::cout << grid;
        }
        std::cout << "result is " << read << '\n';
    }

    return 0;
}