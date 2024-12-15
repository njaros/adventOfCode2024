#include "utils/helpers.hpp"
#include <unistd.h>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define EMPTY 4
#define CRATE 5
#define WALL 6
#define BOUND 7

typedef std::map<Coord, std::pair<char, bool>> Tasks;
typedef std::map<Coord, char> Replacement;

const Coord dirs[] = {Coord(0, -1), Coord(1, 0), Coord(0, 1), Coord(-1, 0)};

Coord doMoove(Grid<char>& g, Coord current, int d)
{
    Coord tmp = current;
    Coord dir = dirs[d];
    current += dir;
    while (g.inBounds(current)) {
        if (g.get(current) == '.') {
            while (current != tmp) {
                g.swapCells(current, current - dir);
                current -= dir;
            }
            return tmp + dir;
        }
        if (g.get(current) == '#')
            return tmp;
        current += dir;
    }
    return tmp;
}

Replacement ctrlMoovable(Grid<char>& g, Coord current, Coord dir) {
    Tasks tasks;
    Replacement res;
    Tasks::iterator it;
    char c;
    char next;
    Coord loc;
    bool generated;

    tasks[current] = std::make_pair('@', false);
    while (!tasks.empty()) {
        it = tasks.begin();
        loc = it->first;
        c = it->second.first;
        generated = it->second.second;
        
        if (!g.inBounds(loc) || c == '#')
            return {};
        
        tasks.erase(it);
        if (c == '@')
            res[loc] = '.';
        else if (!generated)
        {
            if (g.get(loc - dir) != '#')
                res[loc] = g.get(loc - dir);
        }
        else if (g.get(loc - dir) != c && res.find(loc) == res.end())
            res[loc] = '.';
        else if (g.get(loc - dir) != '#') // <-- weird fix for a bug I did't found...
            res[loc] = g.get(loc - dir);
        else
            res[loc] = '.';

        if (g.inBounds(loc + dir))
            next = g.get(loc + dir);
        else
            next = '!';

        switch (c) {
            case '@':
                tasks[loc + dir] = std::make_pair(next, false);
                break;
            case '[':
                if (!generated)
                {
                    if (tasks.find(loc + dirs[RIGHT]) == tasks.end())
                        tasks[loc + dirs[RIGHT]] = std::make_pair(']', true);
                    tasks[loc + dir] = std::make_pair(next, false);
                }
                else if (next != '[')
                    tasks[loc + dir] = std::make_pair(next, false);
                break;
            case ']':
                if (!generated) {
                    if (tasks.find(loc + dirs[LEFT]) == tasks.end())
                        tasks[loc + dirs[LEFT]] = std::make_pair('[', true);
                    tasks[loc + dir] = std::make_pair(next, false);
                }
                else if (next != ']') 
                    tasks[loc + dir] = std::make_pair(next, false);
                break;
        }
    }
    return res;
}

Coord doMooveP2(Grid<char>& g, Coord current, int d) {
    Coord dir = dirs[d];
    char tmp;
    Replacement repl;

    if (d == UP || d == DOWN) {
        repl = ctrlMoovable(g, current, dir);
        if (repl.empty())
            return current;
        // std::cout << repl << '\n';
        for (std::pair<Coord, char> elt: repl) {
            g.get(elt.first) = elt.second;
            // if (elt.second == '[')
            //     g.get(elt.first + dirs[RIGHT]) = ']';
            // if (elt.second == ']')
            //     g.get(elt.first + dirs[LEFT]) = '[';
        }
        return current + dir;
    }
    else {
        return doMoove(g, current, d);
    }
}

ull calcScoreP2(const Grid<char>& g) {
    ull res = 0;
    std::set<Coord> found(g.findAll(']'));

    for (Coord d : found)
        res += (100 * (d.second + 1) + d.first + 1);
    return res;
}

bool noGood(const Grid<char>& g) {
    
    for (const std::vector<char>& v:g) {
        for (int i = 0; i < v.size() - 1; ++i)
        {
            if (v[i] == ']' && v[i + 1] == ']')
                return true;
        }
    }
    
    return false;
}

int part2(std::ifstream &input, Grid<char>& grid) {
    
    std::string line("init");
    std::vector<int> mooves;
    int pouet;

    while (!line.empty()) {
        getline(input, line);
        if (!line.empty()) {
            grid.addEmptyLine();
            line = line.substr(1, line.size() - 2);
            for (char c: line) {
                switch (c) {
                    case '.':
                        grid.appendInLastLine('.');
                        grid.appendInLastLine('.');
                        break;
                    case '#':
                        grid.appendInLastLine('#');
                        grid.appendInLastLine('#');
                        break;
                    case 'O':
                        grid.appendInLastLine('[');
                        grid.appendInLastLine(']');
                        break;
                    case '@':
                        grid.appendInLastLine('@');
                        grid.appendInLastLine('.');
                        break;
                }
            }
        }
    }
    
    grid.erase(grid.begin());
    grid.erase(grid.end() - 1);

    while (!input.eof())
    {
        getline(input, line);
        for (char c : line) {
            switch (c) {
                case '<':
                    mooves.push_back(LEFT);
                    break;
                case 'v':
                    mooves.push_back(DOWN);
                    break;
                case '^':
                    mooves.push_back(UP);
                    break;
                case '>':
                    mooves.push_back(RIGHT);
                    break;
            }
        }
    }

    // Grid<char> save;

    Coord current = grid.findOne('@').value();
    for (int i : mooves) {
        // usleep(20000);
        current = doMooveP2(grid, current, i);
        // if (noGood(grid))
        //     break;
        // save = grid;
    }

    // std::cout << save << '\n';

    std::cout << grid;

    // while (1) {
    //     std::cin >> pouet;
    //     current = doMooveP2(grid, current, pouet);
    //     std::cout << grid;
    // }

    std::cout << "result is " << calcScoreP2(grid) << '\n';

    return 0;
}

ull calcScore(const Grid<char>& g) {
    ull res = 0;
    std::set<Coord> found(g.findAll('O'));

    for (Coord d : found)
        res += (100 * (d.second + 1) + d.first + 1);
    return res;
}

int main() {
    ui part = 0;
    std::ifstream input;
    Grid<char> grid;
    std::string line = "notempty";
    std::vector<int> mooves;

    if (getFileAndPart(15, input, part))
        return errno;

    if (part == 2) {
        return part2(input, grid);
    }

    while (!line.empty()) {
        getline(input, line);
        if (!line.empty()) {
            line = line.substr(1, line.size() - 2);
            grid.addBackLine(line);
        }
    }

    grid.erase(grid.begin());
    grid.erase(grid.end() - 1);

    while (!input.eof())
    {
        getline(input, line);
        for (char c : line) {
            switch (c) {
                case '<':
                    mooves.push_back(LEFT);
                    break;
                case 'v':
                    mooves.push_back(DOWN);
                    break;
                case '^':
                    mooves.push_back(UP);
                    break;
                case '>':
                    mooves.push_back(RIGHT);
                    break;
            }
        }
    }

    Coord current = grid.findOne('@').value();

    for (int m: mooves)
        current = doMoove(grid, current, m);
    std::cout << grid << '\n';

    std::cout << "result is " << calcScore(grid) << '\n';

    return 0;
}