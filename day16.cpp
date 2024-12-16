#include "utils/helpers.hpp"
#include <unistd.h>

#define RIGHT Coord(1, 0)
#define DOWN Coord(0, 1)
#define LEFT Coord(-1, 0)
#define UP Coord(0, -1)

Coord turnLeft(Coord dir) {
    if (dir == LEFT)
        return DOWN;
    if (dir == DOWN)
        return RIGHT;
    if (dir == RIGHT)
        return UP;
    return LEFT;
}

Coord turnRight(Coord dir) {
   if (dir == LEFT)
        return UP;
    if (dir == DOWN)
        return LEFT;
    if (dir == RIGHT)
        return DOWN;
    return RIGHT;
}

class DeerState {

public:

    Coord _pos; // <- posisition of the deer. Key element of the open set
    Coord _dir; // <- direction of the deer. Key element of the open set
    ull _score; // <- number of steps done
    ull _distance; // <- heuristic
    ull _potential; // <- primary key element for open set (this is score + heuristic)
    std::pair<Coord, Coord> _parent; // <- to recover the path (maybe I should use the close set for that ?)

    DeerState() : _pos(Coord(0, 0)), _dir(RIGHT), _score(0), _distance(0), _potential(0), _parent(Coord(0, 0), Coord(0, 0))
    {}

    DeerState(Coord pos, Coord dir, Coord end, ull score, std::pair<Coord, Coord> parent = std::make_pair(Coord(0, 0), Coord(0, 0))):
    _pos(pos), _dir(dir), _score(score), _parent(parent)
    {
        _distance = math::ManhattanDist(_pos, end);
        if (end.first == _pos.first) {
            if (_dir == UP || _dir == DOWN)
                _distance += 1000;
            else if (_dir == LEFT)
                _distance += 2000;
        }
        else if (end.second == _pos.second) {
            if (_dir == RIGHT || _dir == LEFT)
                _distance += 1000;
            else if (_dir == DOWN)
                _distance += 2000;
        }
        else {
            _distance += 2000;
            if (dir == LEFT || dir == DOWN)
                _distance += 1000;
        }
        _potential = _score + _distance;
    }

    DeerState(const DeerState& other) : _pos(other._pos), _dir(other._dir), _score(other._score),
    _distance(other._distance), _potential(other._potential), _parent(other._parent) {}

    DeerState& operator=(const DeerState& o) {
        _pos = o._pos;
        _dir = o._dir;
        _score = o._score;
        _distance = o._distance;
        _potential = o._potential;
        _parent = o._parent;
        return *this;
    }

};

bool operator<(const DeerState& a, const DeerState& b) {
    if (a._potential < b._potential)
        return true;
    if (a._potential > a._potential)
        return false;
    if (a._pos < b._pos)
        return true;
    if (a._pos > b._pos)
        return false;
    return a._dir < b._dir;
}

struct closeCompare {
    bool operator()(const DeerState& a, const DeerState& b) const {
        if (a._pos < b._pos)
            return true;
        if (a._pos > b._pos)
            return false;
        return a._dir < b._dir; // dir ordering is no big deal, I just need to differentiate them.
    }
};

typedef std::set<DeerState> OpenSet;
typedef std::set<DeerState, closeCompare> CloseSet; // Sort visited DeerState by coordinates and orientation

void drawPath(const CloseSet& close, OpenSet::iterator it, Grid<char> grid) {
    DeerState deer = *it;
    std::vector<DeerState> path;
    path.push_back(deer);
    while (deer._parent.second != Coord(0, 0)) {
        deer = *(close.find(DeerState(deer._parent.first, deer._parent.second, Coord(0, 0), 0)));
        path.push_back(deer);
    }

    for (std::vector<DeerState>::const_reverse_iterator rcit = path.rbegin(); rcit != path.rend(); ++rcit) {
        grid.get(rcit->_pos) = 'X';
        std::cout << rcit->_score << '\n';
        std::cout << grid << '\n';
        usleep(100000);
    }
}

void safeInsert(OpenSet& open, CloseSet& close, DeerState state) {
    CloseSet::iterator found = close.find(state);
    if (found != close.end()) {
        if (found->_score > state._score) {
            close.erase(found);
            close.insert(state);
            open.insert(state);
        }
    }
    else {
        close.insert(state);
        open.insert(state);
    }
}

ull aStar(const Grid<char>& maze) {
    OpenSet open;
    CloseSet close;
    Coord goal = maze.findOne('E').value();
    OpenSet::iterator current;

    DeerState start(
        maze.findOne('S').value(),
        RIGHT,
        goal,
        0);
    
    open.insert(start);
    close.insert(start);
    current = open.begin();
    while (!open.empty() && current->_pos != goal) {
        if (maze.get(current->_pos + current->_dir) != '#') {
            safeInsert(
                open,
                close,
                DeerState(
                    current->_pos + current->_dir,
                    current->_dir,
                    goal,
                    current->_score + 1,
                    std::make_pair(current->_pos, current->_dir)
                    ));
        }
        if (maze.get(current->_pos + turnLeft(current->_dir)) != '#') {
            safeInsert(
                open,
                close,
                DeerState(
                    current->_pos,
                    turnLeft(current->_dir),
                    goal,
                    current->_score + 1000,
                    std::make_pair(current->_pos, current->_dir)
                    ));
        }
        if (maze.get(current->_pos + turnRight(current->_dir)) != '#') {
            safeInsert(
                open,
                close,
                DeerState(
                    current->_pos,
                    turnRight(current->_dir),
                    goal,
                    current->_score + 1000,
                    std::make_pair(current->_pos, current->_dir)
                    ));
        }
        open.erase(current);
        current = open.begin();
    }

    // drawPath(close, current, maze);

    if (current->_pos == goal)
        return current->_score;
    else
        return 0;
}

int main() {
    ui part = 5;
    std::ifstream input;
    Grid<char> maze;
    std::string line;

    if (getFileAndPart(16, input, part))
        return errno;

    while (!input.eof())
    {
        getline(input, line);
        maze.addBackLine(line);
    }

    std::cout << "result is " << aStar(maze) << '\n';
    return 0;
}