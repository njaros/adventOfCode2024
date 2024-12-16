#include "utils/helpers.hpp"

#define RIGHT Coord(0, 1)
#define DOWN Coord(1, 0)
#define LEFT Coord(-1, 0)
#define UP Coord(0, -1)

const Coord dirs[] = {RIGHT, DOWN, LEFT, UP};

class DeerState {

public:

    Coord _pos; // <- posisition of the deer. Key element of the open set
    Coord _dir; // <- direction of the deer. Key element of the open set
    ull _score; // <- number of steps done
    ull _distance; // <- heuristic
    ull _potential; // <- primary key element for open set (this is score + heuristic)
    DeerState* _parent; // <- to recover the path (maybe I should use the close set for that ?)

    DeerState() : _pos(Coord(0, 0)), _dir(RIGHT), _score(0), _distance(0), _potential(0), _parent(nullptr)
    {}

    DeerState(Coord pos, Coord dir, Coord end, ull score, DeerState* parent = nullptr):
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
    // too dangerous to copy a pointer here. I should never use this constructor.

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
    bool operator()(const DeerState& a, const DeerState& b) {
        if (a._pos < b._pos)
            return true;
        if (a._pos > b._pos)
            return false;
        return a._dir < b._dir; // dir ordering is no big deal, I just need to differentiate them.
    }
};

typedef std::set<DeerState> OpenSet;
typedef std::set<DeerState, closeCompare> CloseSet; // Sort visited DeerState by coordinates and orientation

ull AStar(const Grid<char>& maze) {
    OpenSet open;
    CloseSet close;
    Coord goal = maze.findOne('E').value();
    OpenSet::const_iterator current;

    DeerState start(
        maze.findOne('S').value(),
        RIGHT,
        goal,
        0);
    
    open.insert(start);
    current = open.begin();
    while (!open.empty() && current->_dir != goal) {
        open.erase(current);
        if ()
    }
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