#include "utils/helpers.hpp"

#define RIGHT Coord(1, 0)
#define DOWN Coord(0, 1)
#define LEFT Coord(-1, 0)
#define UP Coord(0, -1)

typedef std::map<std::vector<char>, std::vector<char>> Cache;
typedef std::pair<Coord, Coord> Move;
Cache cache;

const Grid<char> doorPad({
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {'n', '0', 'A'}
});

const Grid<char> robotPad({
    {'n', '^', 'A'},
    {'<', 'v', '>'}
});


class Robot {
    
    ull _getCost(Coord a, Coord b, std::map<Move, ull>& costs) {
        ull res = 0;
        ull res2 = 0;
        Coord a2 = a;

        if (a.second == 0 && b.first == 0) {
            res += costs[Move(Coord(2, 0), Coord(1, 1))];
            a += DOWN;
            res += costs[Move(Coord(1, 1), Coord(0, 1))];
            a += LEFT;
            while (a.first != b.first) {
                res += costs[Move(Coord(0, 1), Coord(0, 1))];
                a += LEFT;
            }
            res += costs[Move(Coord(0, 1), Coord(2, 0))];
            return res;
        }
        if (a.first == 0 && b.second == 0) {
            res += costs[Move(Coord(2, 0), Coord(2, 1))];
            a += RIGHT;
            while (a.first != b.first) {
                res += costs[Move(Coord(2, 1), Coord(2, 1))];
                a += RIGHT;
            }
            res += costs[Move(Coord(2, 1), Coord(1, 0))];
            a += UP;
            res += costs[Move(Coord(1, 0), Coord(2, 0))];
            return res;
        }
        if (a.first < b.first) {
            res += costs[Move(Coord(2, 0), Coord(2, 1))];
            a += RIGHT;
            while (a.first != b.first) {
                res += costs[Move(Coord(2, 1), Coord(2, 1))];
                a += RIGHT;
            }
            if (a.second < b.second) {
                res += costs[Move(Coord(2, 1), Coord(1, 1))];
                res += costs[Move(Coord(1, 1), Coord(2, 0))];
            }
            else if (a.second > b.second) {
                res += costs[Move(Coord(2, 1), Coord(1, 0))];
                res += costs[Move(Coord(1, 0), Coord(2, 0))];
            }
            else {
                res += costs[Move(Coord(2, 1), Coord(2, 0))];
                return res;
            }
            if (a2.second < b.second) {
                res2 += costs[Move(Coord(2, 0), Coord(1, 1))];
                res2 += costs[Move(Coord(1, 1), Coord(2, 1))];
                a2 += RIGHT;
                while (a2.first != b.first) {
                    res2 += costs[Move(Coord(2, 1), Coord(2, 1))];
                    a2 += RIGHT;
                }
            }
            else if (a2.second > b.second) {
                res2 += costs[Move(Coord(2, 0), Coord(1, 0))];
                res2 += costs[Move(Coord(1, 0), Coord(2, 1))];
                a2 += RIGHT;
                while (a2.first != b.first) {
                    res2 += costs[Move(Coord(2, 1), Coord(2, 1))];
                    a2 += RIGHT;
                }
            }
        }
        else if (a.first > b.first) {
            res += costs[Move(Coord(2, 0), Coord(0, 1))];
            a += LEFT;
            while (a.first != b.first) {
                res += costs[Move(Coord(0, 1), Coord(0, 1))];
                a += LEFT;
            }
            if (a.second < b.second) {
                res += costs[Move(Coord(0, 1), Coord(1, 1))];
                res += costs[Move(Coord(1, 1), Coord(2, 0))];
            }
            else if (a.second > b.second) {
                res += costs[Move(Coord(0, 1), Coord(1, 0))];
                res += costs[Move(Coord(1, 0), Coord(2, 0))];
            }
            else {
                res += costs[Move(Coord(0, 1), Coord(2, 0))];
                return res;
            }
            if (a2.second < b.second) {
                res2 += costs[Move(Coord(2, 0), Coord(1, 1))];
                res2 += costs[Move(Coord(1, 1), Coord(0, 1))];
                a2 += LEFT;
                while (a2.first != b.first) {
                    res2 += costs[Move(Coord(0, 1), Coord(0, 1))];
                    a2 += LEFT;
                }
            }
            else if (a2.second > b.second) {
                res2 += costs[Move(Coord(2, 0), Coord(1, 0))];
                res2 += costs[Move(Coord(1, 0), Coord(0, 1))];
                a2 += LEFT;
                while (a2.first != b.first) {
                    res2 += costs[Move(Coord(0, 1), Coord(0, 1))];
                    a2 += LEFT;
                }
            }
        }
        else {
            if (a.second < b.second) {
                res += costs[Move(Coord(2, 0), Coord(1, 1))];
                res += costs[Move(Coord(1, 1), Coord(2, 0))];
            }
            else if (a.second > b.second) {
                res += costs[Move(Coord(2, 0), Coord(1, 0))];
                res += costs[Move(Coord(1, 0), Coord(2, 0))];
            }
            else
                res = costs[Move(Coord(2, 0), Coord(2, 0))];
            return res;
        }
        
        return std::min(res, res2);
    }

    public:

    Coord index;
    std::map<Move, ull> moveCosts;

    Robot() {
        index = robotPad.findOne('A').value();
        moveCosts[std::make_pair(Coord(1, 0), Coord(1, 0))] = 1;
        moveCosts[std::make_pair(Coord(1, 0), Coord(2, 0))] = 2;
        moveCosts[std::make_pair(Coord(1, 0), Coord(0, 1))] = 3;
        moveCosts[std::make_pair(Coord(1, 0), Coord(1, 1))] = 2;
        moveCosts[std::make_pair(Coord(1, 0), Coord(2, 1))] = 3;

        moveCosts[std::make_pair(Coord(2, 0), Coord(1, 0))] = 2;
        moveCosts[std::make_pair(Coord(2, 0), Coord(2, 0))] = 1;
        moveCosts[std::make_pair(Coord(2, 0), Coord(0, 1))] = 4;
        moveCosts[std::make_pair(Coord(2, 0), Coord(1, 1))] = 3;
        moveCosts[std::make_pair(Coord(2, 0), Coord(2, 1))] = 2;

        moveCosts[std::make_pair(Coord(0, 1), Coord(1, 0))] = 3;
        moveCosts[std::make_pair(Coord(0, 1), Coord(2, 0))] = 4;
        moveCosts[std::make_pair(Coord(0, 1), Coord(0, 1))] = 1;
        moveCosts[std::make_pair(Coord(0, 1), Coord(1, 1))] = 2;
        moveCosts[std::make_pair(Coord(0, 1), Coord(2, 1))] = 3;

        moveCosts[std::make_pair(Coord(1, 1), Coord(1, 0))] = 2;
        moveCosts[std::make_pair(Coord(1, 1), Coord(2, 0))] = 3;
        moveCosts[std::make_pair(Coord(1, 1), Coord(0, 1))] = 2;
        moveCosts[std::make_pair(Coord(1, 1), Coord(1, 1))] = 1;
        moveCosts[std::make_pair(Coord(1, 1), Coord(2, 1))] = 2;

        moveCosts[std::make_pair(Coord(2, 1), Coord(1, 0))] = 3;
        moveCosts[std::make_pair(Coord(2, 1), Coord(2, 0))] = 2;
        moveCosts[std::make_pair(Coord(2, 1), Coord(0, 1))] = 3;
        moveCosts[std::make_pair(Coord(2, 1), Coord(1, 1))] = 2;
        moveCosts[std::make_pair(Coord(2, 1), Coord(2, 1))] = 1;
    }

    Robot(Robot& o): index(o.index) {
        moveCosts[std::make_pair(Coord(1, 0), Coord(1, 0))] = _getCost(Coord(1, 0), Coord(1, 0), o.moveCosts);
        moveCosts[std::make_pair(Coord(1, 0), Coord(2, 0))] = _getCost(Coord(1, 0), Coord(2, 0), o.moveCosts);
        moveCosts[std::make_pair(Coord(1, 0), Coord(0, 1))] = _getCost(Coord(1, 0), Coord(0, 1), o.moveCosts);
        moveCosts[std::make_pair(Coord(1, 0), Coord(1, 1))] = _getCost(Coord(1, 0), Coord(1, 1), o.moveCosts);
        moveCosts[std::make_pair(Coord(1, 0), Coord(2, 1))] = _getCost(Coord(1, 0), Coord(2, 1), o.moveCosts);

        moveCosts[std::make_pair(Coord(2, 0), Coord(1, 0))] = _getCost(Coord(2, 0), Coord(1, 0), o.moveCosts);
        moveCosts[std::make_pair(Coord(2, 0), Coord(2, 0))] = _getCost(Coord(2, 0), Coord(2, 0), o.moveCosts);
        moveCosts[std::make_pair(Coord(2, 0), Coord(0, 1))] = _getCost(Coord(2, 0), Coord(0, 1), o.moveCosts);
        moveCosts[std::make_pair(Coord(2, 0), Coord(1, 1))] = _getCost(Coord(2, 0), Coord(1, 1), o.moveCosts);
        moveCosts[std::make_pair(Coord(2, 0), Coord(2, 1))] = _getCost(Coord(2, 0), Coord(2, 1), o.moveCosts);

        moveCosts[std::make_pair(Coord(0, 1), Coord(1, 0))] = _getCost(Coord(0, 1), Coord(1, 0), o.moveCosts);
        moveCosts[std::make_pair(Coord(0, 1), Coord(2, 0))] = _getCost(Coord(0, 1), Coord(2, 0), o.moveCosts);
        moveCosts[std::make_pair(Coord(0, 1), Coord(0, 1))] = _getCost(Coord(0, 1), Coord(0, 1), o.moveCosts);
        moveCosts[std::make_pair(Coord(0, 1), Coord(1, 1))] = _getCost(Coord(0, 1), Coord(1, 1), o.moveCosts);
        moveCosts[std::make_pair(Coord(0, 1), Coord(2, 1))] = _getCost(Coord(0, 1), Coord(2, 1), o.moveCosts);

        moveCosts[std::make_pair(Coord(1, 1), Coord(1, 0))] = _getCost(Coord(1, 1), Coord(1, 0), o.moveCosts);
        moveCosts[std::make_pair(Coord(1, 1), Coord(2, 0))] = _getCost(Coord(1, 1), Coord(2, 0), o.moveCosts);
        moveCosts[std::make_pair(Coord(1, 1), Coord(0, 1))] = _getCost(Coord(1, 1), Coord(0, 1), o.moveCosts);
        moveCosts[std::make_pair(Coord(1, 1), Coord(1, 1))] = _getCost(Coord(1, 1), Coord(1, 1), o.moveCosts);
        moveCosts[std::make_pair(Coord(1, 1), Coord(2, 1))] = _getCost(Coord(1, 1), Coord(2, 1), o.moveCosts);

        moveCosts[std::make_pair(Coord(2, 1), Coord(1, 0))] = _getCost(Coord(2, 1), Coord(1, 0), o.moveCosts);
        moveCosts[std::make_pair(Coord(2, 1), Coord(2, 0))] = _getCost(Coord(2, 1), Coord(2, 0), o.moveCosts);
        moveCosts[std::make_pair(Coord(2, 1), Coord(0, 1))] = _getCost(Coord(2, 1), Coord(0, 1), o.moveCosts);
        moveCosts[std::make_pair(Coord(2, 1), Coord(1, 1))] = _getCost(Coord(2, 1), Coord(1, 1), o.moveCosts);
        moveCosts[std::make_pair(Coord(2, 1), Coord(2, 1))] = _getCost(Coord(2, 1), Coord(2, 1), o.moveCosts);
    }

};

class Door {

    public:

    void initPad() {
        pad[std::make_pair('A', '0')] = {"<A"};
        pad[std::make_pair('A', '1')] = {"^<<A"};
        pad[std::make_pair('A', '2')] = {"<^A", "^<A"};
        pad[std::make_pair('A', '3')] = {"^A"};
        pad[std::make_pair('A', '4')] = {"^^<<A"};
        pad[std::make_pair('A', '5')] = {"<^^A", "^^<A"};
        pad[std::make_pair('A', '6')] = {"^^A"};
        pad[std::make_pair('A', '7')] = {"^^^<<A"};
        pad[std::make_pair('A', '8')] = {"<^^^A", "^^^<A"};
        pad[std::make_pair('A', '9')] = {"^^^A"};
        pad[std::make_pair('A', 'A')] = {"A"};

        pad[std::make_pair('0', '0')] = {"A"};
        pad[std::make_pair('0', '1')] = {"^<A"};
        pad[std::make_pair('0', '2')] = {"^A"};
        pad[std::make_pair('0', '3')] = {"^>A", ">^A"};
        pad[std::make_pair('0', '4')] = {"^^<A"};
        pad[std::make_pair('0', '5')] = {"^^A"};
        pad[std::make_pair('0', '6')] = {"^^>A", ">^^A"};
        pad[std::make_pair('0', '7')] = {"^^^<A"};
        pad[std::make_pair('0', '8')] = {"^^^A"};
        pad[std::make_pair('0', '9')] = {"^^^>A", ">^^^A"};
        pad[std::make_pair('0', 'A')] = {">A"};

        pad[std::make_pair('1', '0')] = {">vA"};
        pad[std::make_pair('1', '1')] = {"A"};
        pad[std::make_pair('1', '2')] = {">A"};
        pad[std::make_pair('1', '3')] = {">>A"};
        pad[std::make_pair('1', '4')] = {"^A"};
        pad[std::make_pair('1', '5')] = {"^>A", ">^A"};
        pad[std::make_pair('1', '6')] = {"^>>A", ">>^A"};
        pad[std::make_pair('1', '7')] = {"^^A"};
        pad[std::make_pair('1', '8')] = {"^^>A", ">^^A"};
        pad[std::make_pair('1', '9')] = {"^^>>A", ">>^^A"};
        pad[std::make_pair('1', 'A')] = {">>vA"};

        pad[std::make_pair('2', '0')] = {"vA"};
        pad[std::make_pair('2', '1')] = {"<A"};
        pad[std::make_pair('2', '2')] = {"A"};
        pad[std::make_pair('2', '3')] = {">A"};
        pad[std::make_pair('2', '4')] = {"^<A", "<^A"};
        pad[std::make_pair('2', '5')] = {"^A"};
        pad[std::make_pair('2', '6')] = {"^>A", ">^A"};
        pad[std::make_pair('2', '7')] = {"^^<A", "<^^A"};
        pad[std::make_pair('2', '8')] = {"^^A"};
        pad[std::make_pair('2', '9')] = {">>^A", "^>>A"};
        pad[std::make_pair('2', 'A')] = {">vA", "v>A"};

        pad[std::make_pair('3', '0')] = {"v<A", "<vA"};
        pad[std::make_pair('3', '1')] = {"<<A"};
        pad[std::make_pair('3', '2')] = {"<A"};
        pad[std::make_pair('3', '3')] = {"A"};
        pad[std::make_pair('3', '4')] = {"^<<A", "<<^A"};
        pad[std::make_pair('3', '5')] = {"<^A", "^<A"};
        pad[std::make_pair('3', '6')] = {"^A"};
        pad[std::make_pair('3', '7')] = {"^^<<A", "<<^^A"};
        pad[std::make_pair('3', '8')] = {"<^^A", "^^<A"};
        pad[std::make_pair('3', '9')] = {"^^A"};
        pad[std::make_pair('3', 'A')] = {"vA"};

        pad[std::make_pair('4', '0')] = {">vvA"};
        pad[std::make_pair('4', '1')] = {"vA"};
        pad[std::make_pair('4', '2')] = {"v>A", ">vA"};
        pad[std::make_pair('4', '3')] = {"v>>A", ">>vA"};
        pad[std::make_pair('4', '4')] = {"A"};
        pad[std::make_pair('4', '5')] = {">A"};
        pad[std::make_pair('4', '6')] = {">>A"};
        pad[std::make_pair('4', '7')] = {"^A"};
        pad[std::make_pair('4', '8')] = {"^>A", ">^A"};
        pad[std::make_pair('4', '9')] = {">>^A", "^>>A"};
        pad[std::make_pair('4', 'A')] = {">>vvA"};

        pad[std::make_pair('5', '0')] = {"vvA"};
        pad[std::make_pair('5', '1')] = {"v<A", "<vA"};
        pad[std::make_pair('5', '2')] = {"vA"};
        pad[std::make_pair('5', '3')] = {">vA", "v>A"};
        pad[std::make_pair('5', '4')] = {"<A"};
        pad[std::make_pair('5', '5')] = {"A"};
        pad[std::make_pair('5', '6')] = {">A"};
        pad[std::make_pair('5', '7')] = {"^<A", "<^A"};
        pad[std::make_pair('5', '8')] = {"^A"};
        pad[std::make_pair('5', '9')] = {"^>A", ">^A"};
        pad[std::make_pair('5', 'A')] = {">vvA", "vv>A"};

        pad[std::make_pair('6', '0')] = {"<vvA", "vv<A"};
        pad[std::make_pair('6', '1')] = {"<<vA", "v<<A"};
        pad[std::make_pair('6', '2')] = {"<vA", "v<A"};
        pad[std::make_pair('6', '3')] = {"vA"};
        pad[std::make_pair('6', '4')] = {"<<A"};
        pad[std::make_pair('6', '5')] = {"<A"};
        pad[std::make_pair('6', '6')] = {"A"};
        pad[std::make_pair('6', '7')] = {"^<<A", "<<^A"};
        pad[std::make_pair('6', '8')] = {"<^A", "^<A"};
        pad[std::make_pair('6', '9')] = {"^A"};
        pad[std::make_pair('6', 'A')] = {"vvA"};

        pad[std::make_pair('7', '0')] = {">vvvA"};
        pad[std::make_pair('7', '1')] = {"vvA"};
        pad[std::make_pair('7', '2')] = {"vv>A", ">vvA"};
        pad[std::make_pair('7', '3')] = {">>vvA", "vv>>A"};
        pad[std::make_pair('7', '4')] = {"vA"};
        pad[std::make_pair('7', '5')] = {"v>A", ">vA"};
        pad[std::make_pair('7', '6')] = {">>vA", "v>>A"};
        pad[std::make_pair('7', '7')] = {"A"};
        pad[std::make_pair('7', '8')] = {">A"};
        pad[std::make_pair('7', '9')] = {">>A"};
        pad[std::make_pair('7', 'A')] = {">>vvvA", "vvv>>A"};

        pad[std::make_pair('8', '0')] = {"vvvA"};
        pad[std::make_pair('8', '1')] = {"vv<A", "<vvA"};
        pad[std::make_pair('8', '2')] = {"vvA"};
        pad[std::make_pair('8', '3')] = {"vv>A", ">vvA"};
        pad[std::make_pair('8', '4')] = {"<vA","v<A"};
        pad[std::make_pair('8', '5')] = {"vA"};
        pad[std::make_pair('8', '6')] = {"v>A", ">vA"};
        pad[std::make_pair('8', '7')] = {"<A"};
        pad[std::make_pair('8', '8')] = {"A"};
        pad[std::make_pair('8', '9')] = {">A"};
        pad[std::make_pair('8', 'A')] = {">vvvA", "vvv>A"};

        pad[std::make_pair('9', '0')] = {"<vvvA", "vvv<A"};
        pad[std::make_pair('9', '1')] = {"<<vvA", "vv<<A"};
        pad[std::make_pair('9', '2')] = {"vv<A", "<vvA"};
        pad[std::make_pair('9', '3')] = {"vvA"};
        pad[std::make_pair('9', '4')] = {"v<<A", "<<vA"};
        pad[std::make_pair('9', '5')] = {"<vA", "v<A"};
        pad[std::make_pair('9', '6')] = {"vA"};
        pad[std::make_pair('9', '7')] = {"<<A"};
        pad[std::make_pair('9', '8')] = {"<A"};
        pad[std::make_pair('9', '9')] = {"A"};
        pad[std::make_pair('9', 'A')] = {"vvA"};
    }

    std::map<Move, ull> costs;

    std::map<std::pair<char, char>, std::vector<std::string>> pad;

    Door(const Robot& r) : costs(r.moveCosts) {
        initPad();
    }

    Coord getCoordFromChar(char c) {
        switch (c)
        {
        case '>':
            return Coord(2, 1);
        case '<':
            return Coord(0, 1);
        case 'v':
            return Coord(1, 1);
        case '^':
            return Coord(1, 0);
        case 'A':
            return Coord(2, 0);       
        default:
            return Coord(0, 0);
        }
    }

    ull minCost(const std::vector<std::string>& poss) {
        ull min = ~0;
        ull current;

        for (std::string elt : poss) {
            std::cout << elt << '\n';
            current = 0;
            for (int i = 0; i < elt.size(); ++i) {
                if (i == 0) {
                    std::cout << Move(getCoordFromChar('A'), getCoordFromChar(elt[i])) << " = " << costs[Move(getCoordFromChar('A'), getCoordFromChar(elt[i]))] << '\n';
                    current += costs[Move(getCoordFromChar('A'), getCoordFromChar(elt[i]))];
                }
                else 
                {
                    std::cout << Move(getCoordFromChar(elt[i - 1]), getCoordFromChar(elt[i])) << " = " << costs[Move(getCoordFromChar(elt[i - 1]), getCoordFromChar(elt[i]))] << '\n';
                    current += costs[Move(getCoordFromChar(elt[i - 1]), getCoordFromChar(elt[i]))];
                }
            }
            if (min > current)
                min = current;
        }
        return current;
    }

    ull calcCost(const std::string& code) {
        ull res = 0;
        char index = 'A';

        for (char c : code) {
            if (c != '\n' && c != '\r') {
                res += minCost(pad[std::make_pair(index, c)]);
                index = c;
            }
        }
        return res;
    }

};

ui getNumber(const std::string& str) {
    ui number = 0;
    
    for (int i = 0; i < 3; ++i) {
        number *= 10;
        number += str[i] - '0';
    }
    return number;
}

std::vector<char> getMin(const std::set<std::vector<char>>& poss)
{
    std::set<std::vector<char>>::const_iterator citMin;
    ui min = ~0;
    for (std::set<std::vector<char>>::const_iterator cit = poss.begin(); cit != poss.end(); ++cit) {
        if (cit->size() < min) {
            min = cit->size();
            citMin = cit;
        }
    }

    return *citMin;
}

int main() {
    ui part = 0;
    std::ifstream input;
    std::string line;
    std::vector<char> instructions;
    std::set<std::vector<char>> poss;
    ui res = 0;
    Robot r1;
    Robot r2(r1);
    Robot r3(r2);
    Robot r4(r3);
    Robot r5(r4);
    Robot r6(r5);
    Robot r7(r6);
    Robot r8(r7);
    Robot r9(r8);
    Robot r10(r9);
    Robot r11(r10);
    Robot r12(r11);
    Robot r13(r12);
    Robot r14(r13);
    Robot r15(r14);
    Robot r16(r15);
    Robot r17(r16);
    Robot r18(r17);
    Robot r19(r18);
    Robot r20(r19);
    Robot r21(r20);
    Robot r22(r21);
    Robot r23(r22);
    Robot r24(r23);

    if (getFileAndPart(100, input, part))
        return errno;

    if (part == 1) {

        Door rf(r2);
        while(!input.eof()) {
            getline(input, line);
            res += rf.calcCost(line) * getNumber(line);
            std::cout << rf.calcCost(line) << " | " << getNumber(line) << '\n';
        }
    }

    else {

        Door rf(r24);
        while(!input.eof()) {
            getline(input, line);
            res += rf.calcCost(line) * getNumber(line);
            std::cout << rf.calcCost(line) << " | " << getNumber(line) << '\n';
        }
    }

    std::cout << r1.moveCosts << '\n';
    std::cout << r2.moveCosts << '\n';
    std::cout << r3.moveCosts << '\n';
    std::cout << r4.moveCosts << '\n';

    std::cout << "result is " << res << '\n';

    return 0;
}