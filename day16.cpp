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
typedef std::pair<Coord, Coord> Node;
typedef std::pair<ull, std::set<Node>> Candidate;
typedef std::map<Node, Candidate> CloseSet;

void countSteps(const CloseSet& close, Node current, std::set<Coord>& steps) {
    if (current.first != Coord(0, 0))
        steps.insert(current.first);
    CloseSet::const_iterator found = close.find(current);
    if (found != close.end()) {
        for (Node n : found->second.second) {
            countSteps(close, n, steps);
        }
    }
}

void printMap(Grid<char> grid, const std::set<Coord>& steps) {
    for (Coord c : steps) {
        grid.get(c) = 'O';
    }
    std::cout << grid << '\n';
}

void safeInsert(OpenSet& open, CloseSet& close, DeerState state) {
    Node newNode(state._pos, state._dir);
    CloseSet::iterator found = close.find(newNode);
    if (found != close.end()) {
        if (found->second.first > state._score) {
            close.erase(found);
            close[newNode] = Candidate(state._score, std::set<Node>({state._parent}));
            open.insert(state);
        }
        else if (found->second.first == state._score) {
            found->second.second.insert(state._parent);
            open.insert(state);
        }
    }
    else {
        close[newNode] = Candidate(state._score, std::set<Node>({state._parent}));
        open.insert(state);
    }
}

bool stopLoop(Coord goal, DeerState current, ull& resP1, std::set<Coord>& save) {
    static bool endReached = false;
    if (goal == current._pos) {
        // std::cout << current._score << '\n';
        if (!endReached) {
            resP1 = current._score;
            save.insert(current._dir);
            endReached = true;
            return false;
        }
        if (current._score > resP1)
            return true;
        else
            save.insert(current._dir);
    }
    return false;
}

ull aStar(const Grid<char>& maze, ui part) {
    OpenSet open;
    CloseSet close;
    Coord goal = maze.findOne('E').value();
    OpenSet::iterator current;
    std::set<Coord> saveGoodDir;
    ull resP1;

    DeerState start(
        maze.findOne('S').value(),
        RIGHT,
        goal,
        0);
    
    open.insert(start);
    close[Node(start._pos, start._dir)] = Candidate(start._score, std::set<Node>({Node(Coord(0, 0), Coord(0, 0))}));
    current = open.begin();
    while (!open.empty() && !stopLoop(goal, *current, resP1, saveGoodDir)) {
        if (current->_pos == goal)
        {
            open.erase(current);
            current = open.begin();
            continue;
        }
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

    if (part == 1)
        return resP1;
    else {
        std::set<Coord> steps;
        for (Coord dir : saveGoodDir)
            countSteps(close, Node(goal, dir), steps);
        // printMap(maze, steps);
        return steps.size();
    }
}

int main() {
    ui part = 0;
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

    std::cout << "result is " << aStar(maze, part) << '\n';
    return 0;
}