#include "utils/helpers.hpp"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

typedef std::map<char, std::set<Coord>> LetterGroups;
typedef std::vector<std::set<Coord>> Plots;

Coord dirs[] = {Coord(0, 1), Coord(0, -1), Coord(1, 0), Coord(-1, 0)};

ui calcCost(const std::set<Coord>& plot, ui part) {
    ui cost = 0;
    std::set<std::pair<Coord, int>> fences;    
    int voisin;
    ui sides = 0;

    for (Coord c: plot) {
        for (Coord dir: dirs) {
            if (plot.find(c + dir) == plot.end())
            {
                ++cost;
                if (dir == Coord(0, 1))
                    fences.insert(std::make_pair((c + dir), DOWN));
                else if (dir == Coord (0, -1))
                    fences.insert(std::make_pair((c + dir), UP));
                else if (dir == Coord (1, 0))
                    fences.insert(std::make_pair((c + dir), RIGHT));
                else
                    fences.insert(std::make_pair((c + dir), LEFT));
            }
        }
    }
    if (part == 1)
        return cost * plot.size();
    else {
        for (std::pair<Coord, int> cf : fences) {
            voisin = 0;
            for (Coord dir: dirs)
            {
                if (fences.find(std::make_pair(cf.first + dir, cf.second)) != fences.end())
                    ++voisin;
            }
            switch (voisin) {
                case 0:
                    sides += 2;
                    break;
                case 1:
                    ++sides;
                    break;
            }
        }
        return (sides / 2) * plot.size();
    }
}

void lookForPlots(std::set<Coord>& coords, Plots& plots) {
    std::set<Coord>::iterator found;
    std::deque<Coord> tasks;
    std::set<Coord> build;
    std::set<Coord> fixLoopInf;
    Coord current;

    while (!coords.empty()) {
        build.clear();
        tasks.push_back(*(coords.begin()));
        while (!tasks.empty()) {
            current = tasks.front();
            coords.erase(current);
            tasks.pop_front();
            build.insert(current);
            for (Coord dir: dirs) {
                found = coords.find(current + dir);
                if (fixLoopInf.find(current + dir) == fixLoopInf.end() && found != coords.end()) {
                    tasks.push_back(current + dir);
                }
                fixLoopInf.insert(current + dir);
            }
        }
        plots.push_back(build);
    }
}

int main() {
    ui part = 0;
    std::ifstream input;
    LetterGroups allLetters;
    Plots plots;
    ui costCount = 0;
    std::pair<char, Coord> read = std::make_pair(0, Coord(0, 0));

    if (getFileAndPart(12, input, part))
        return errno;
    
    read.first = input.get();
    while (!input.eof()) {
        if (read.first == '\r')
            read.first = input.get();
        if (read.first == '\n')
            read = std::make_pair(input.get(), Coord(0, read.second.second + 1));
        allLetters[read.first].insert(read.second);
        read.second += Coord(1, 0);
        read.first = input.get();
    }

    for (LetterGroups::iterator it = allLetters.begin(); it != allLetters.end(); ++it) {
        lookForPlots(it->second, plots);
    }
    for (const std::set<Coord>& plot: plots)
        costCount += calcCost(plot, part);

    std::cout << "642214 too low\n834546 too low\nresult is " << costCount << '\n';
}