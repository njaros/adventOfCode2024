#include "utils/helpers.hpp"

#define RIGHT Coord(1, 0)
#define DOWN Coord(0, 1)
#define LEFT Coord(-1, 0)
#define UP Coord(0, -1)

typedef std::map<std::vector<char>, std::vector<char>> Cache;
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

class Door {

    public:

    Coord index;

    Door() {
        index = doorPad.findOne('A').value();
    }

    std::vector<char> goTo(char n) {
        Coord goal = doorPad.findOne(n).value();
        std::vector<char> instructions;

        while (index != goal) {
            if (index.first < goal.first) {
                instructions.push_back('>');
                index += RIGHT;
            }
            else if (index.second > goal.second) {
                instructions.push_back('^');
                index += UP;
            }
            else if (index.first > goal.first) {
                instructions.push_back('<');
                index += LEFT;
            }
            else if (index.second < goal.second) {
                instructions.push_back('v');
                index += DOWN;
            }
        }
        instructions.push_back('A');
        return instructions;
    }

    std::set<std::vector<char>> givePossibilities(char n) {
        const Coord goal = doorPad.findOne(n).value();
        const Coord tmp = index;
        std::set<std::vector<char>> poss;
        std::vector<char> current;
        std::vector<char> current2;

        if (index.second < goal.second) {
            if (goal.second == 3 && index.first == 0) {
                while (index.first != goal.first)
                {
                    current.push_back('>');
                    index += RIGHT;
                }
                while (index.second != goal.second)
                {
                    current.push_back('v');
                    index += DOWN;
                }
                current.push_back('A');
                poss.insert(current);
            }
            else if (index.first < goal.first) {
                while (index.first != goal.first)
                {
                    current.push_back('>');
                    index += RIGHT;
                }
                while (index.second != goal.second)
                {
                    current.push_back('v');
                    index += DOWN;
                }
                current.push_back('A');
                poss.insert(current);
                index = tmp;
                while (index.second != goal.second)
                {
                    current2.push_back('v');
                    index += DOWN;
                }
                while (index.first != goal.first)
                {
                    current2.push_back('>');
                    index += RIGHT;
                }
                current2.push_back('A');
                poss.insert(current2);
            }
            else {
                while (index.second != goal.second)
                {
                    current.push_back('v');
                    index += DOWN;
                }
                while (index.first != goal.first)
                {
                    current.push_back('<');
                    index += LEFT;
                }
                current.push_back('A');
                poss.insert(current);
                index = tmp;
                while (index.first != goal.first)
                {
                    current2.push_back('<');
                    index += LEFT;
                }
                while (index.second != goal.second)
                {
                    current2.push_back('v');
                    index += DOWN;
                }
                current2.push_back('A');
                poss.insert(current2);
            }
        }
        else {
            if (index.second == 3 && goal.first == 0) {
                while (index.second != goal.second)
                {
                    current.push_back('^');
                    index += UP;
                }
                while (index.first != goal.first)
                {
                    current.push_back('<');
                    index += LEFT;
                }
                current.push_back('A');
                poss.insert(current);
            }
            else if (index.first < goal.first) {
                while (index.first != goal.first)
                {
                    current.push_back('>');
                    index += RIGHT;
                }
                while (index.second != goal.second)
                {
                    current.push_back('^');
                    index += UP;
                }
                current.push_back('A');
                poss.insert(current);
                index = tmp;
                while (index.second != goal.second)
                {
                    current2.push_back('^');
                    index += UP;
                }
                while (index.first != goal.first)
                {
                    current2.push_back('>');
                    index += RIGHT;
                }
                current2.push_back('A');
                poss.insert(current2);
            }
            else {
                while (index.second != goal.second)
                {
                    current.push_back('^');
                    index += UP;
                }
                while (index.first != goal.first)
                {
                    current.push_back('<');
                    index += LEFT;
                }
                current.push_back('A');
                poss.insert(current);
                index = tmp;
                while (index.first != goal.first)
                {
                    current2.push_back('<');
                    index += LEFT;
                }
                while (index.second != goal.second)
                {
                    current2.push_back('^');
                    index += UP;
                }
                current2.push_back('A');
                poss.insert(current2);
            }
        }
        return poss;
    }

    std::vector<char> getCode(const std::string& code) {
        std::vector<char> instructions;
        std::vector<char> moves;

        for (std::string::const_iterator cit = code.begin(); cit != code.end(); ++cit) {
            if (*cit != '\n' && *cit != '\r') {
                moves = goTo(*cit);
                instructions.insert(instructions.end(), moves.begin(), moves.end());
            }
        }
        return instructions;
    }

    std::set<std::vector<char>> getPossibilities(const std::string& code) {
        std::set<std::vector<char>> moves;
        std::vector<char> tmp;
        std::vector<std::vector<char>> poss;
        std::set<std::vector<char>> toReturn;
        int end;

        for (std::string::const_iterator cit = code.begin(); cit != code.end(); ++cit) {
            if (*cit != '\n' && *cit != '\r') {
                moves = givePossibilities(*cit);
                if (poss.empty()) {
                    for (const std::vector<char>& elt : moves) {
                        poss.push_back(elt);
                    }
                }
                else {
                    end = poss.size();
                    for (int it = 0; it != end; ++it) {
                        for (std::set<std::vector<char>>::const_iterator it2 = moves.begin(); it2 != moves.end(); ++ it2) {
                            if (it2 == moves.begin()) {
                                tmp = poss[it];
                                poss[it].insert(poss[it].end(), it2->begin(), it2->end());
                            }
                            else
                            {
                                std::vector<char> inFlight(tmp);
                                inFlight.insert(inFlight.end(), it2->begin(), it2->end());
                                poss.push_back(inFlight);
                            }
                        }
                    }
                }
            }
        }
        toReturn.insert(poss.begin(), poss.end());
        return toReturn;
    }

};

class Robot {
    
    public:

    Coord index;

    Robot() {
        index = robotPad.findOne('A').value();
    }

    std::set<std::vector<char>> givePossibilities(char n) {
        const Coord goal = robotPad.findOne(n).value();
        const Coord tmp = index;
        std::set<std::vector<char>> poss;
        std::vector<char> current;
        std::vector<char> current2;

        if (index.second < goal.second) {
            if (goal.first == 0) {
                while (index.second != goal.second)
                {
                    current.push_back('v');
                    index += DOWN;
                }
                while (index.first != goal.first)
                {
                    current.push_back('<');
                    index += LEFT;
                }
                current.push_back('A');
                poss.insert(current);
            }
            else if (index.first < goal.first) {
                while (index.first != goal.first)
                {
                    current.push_back('>');
                    index += RIGHT;
                }
                while (index.second != goal.second)
                {
                    current.push_back('v');
                    index += DOWN;
                }
                current.push_back('A');
                poss.insert(current);
                index = tmp;
                while (index.second != goal.second)
                {
                    current2.push_back('v');
                    index += DOWN;
                }
                while (index.first != goal.first)
                {
                    current2.push_back('>');
                    index += RIGHT;
                }
                current2.push_back('A');
                poss.insert(current2);
            }
            else {
                while (index.second != goal.second)
                {
                    current.push_back('v');
                    index += DOWN;
                }
                while (index.first != goal.first)
                {
                    current.push_back('<');
                    index += LEFT;
                }
                current.push_back('A');
                poss.insert(current);
                index = tmp;
                while (index.first != goal.first)
                {
                    current2.push_back('<');
                    index += LEFT;
                }
                while (index.second != goal.second)
                {
                    current2.push_back('v');
                    index += DOWN;
                }
                current2.push_back('A');
                poss.insert(current2);
            }
        }
        else {
            if (index.first == 0) {
                while (index.first != goal.first)
                {
                    current.push_back('>');
                    index += RIGHT;
                }
                while (index.second != goal.second)
                {
                    current.push_back('^');
                    index += UP;
                }
                current.push_back('A');
                poss.insert(current);
            }
            else if (index.first < goal.first) {
                while (index.first != goal.first)
                {
                    current.push_back('>');
                    index += RIGHT;
                }
                while (index.second != goal.second)
                {
                    current.push_back('^');
                    index += UP;
                }
                current.push_back('A');
                poss.insert(current);
                index = tmp;
                while (index.second != goal.second)
                {
                    current2.push_back('^');
                    index += UP;
                }
                while (index.first != goal.first)
                {
                    current2.push_back('>');
                    index += RIGHT;
                }
                current2.push_back('A');
                poss.insert(current2);
            }
            else {
                while (index.second != goal.second)
                {
                    current.push_back('^');
                    index += UP;
                }
                while (index.first != goal.first)
                {
                    current.push_back('<');
                    index += LEFT;
                }
                current.push_back('A');
                poss.insert(current);
                index = tmp;
                while (index.first != goal.first)
                {
                    current2.push_back('<');
                    index += LEFT;
                }
                while (index.second != goal.second)
                {
                    current2.push_back('^');
                    index += UP;
                }
                current2.push_back('A');
                poss.insert(current2);
            }
        }
        return poss;
    }

    std::vector<char> goTo(char n) {
        Coord goal = robotPad.findOne(n).value();
        std::vector<char> instructions;

        while (index != goal) {
            if (index.second < goal.second) {
                instructions.push_back('v');
                index += DOWN;
            }
            else if (index.first < goal.first) {
                instructions.push_back('>');
                index += RIGHT;
            }
            else if (index.first > goal.first) {
                instructions.push_back('<');
                index += LEFT;
            }
            else if (index.second > goal.second) {
                instructions.push_back('^');
                index += UP;
            }
        }
        instructions.push_back('A');
        return instructions;
    }

    std::vector<char> getInstructions(const std::vector<char>& instr) {
        std::vector<char> instructions;
        std::vector<char> moves;

        // std::cout << instr << '\n';

        for (char c: instr) {
            moves = goTo(c);
            instructions.insert(instructions.end(), moves.begin(), moves.end());
        }

        return instructions;
    }

    std::set<std::vector<char>> getPossibilities(const std::set<std::vector<char>>& code) {
        std::set<std::vector<char>> moves;
        std::vector<char> tmp;
        std::vector<std::vector<char>> poss;
        std::set<std::vector<char>> toReturn;
        int end;

        for (std::set<std::vector<char>>::const_iterator citSet = code.begin(); citSet != code.end(); ++citSet) {
            poss.clear();
            for (std::vector<char>::const_iterator cit = citSet->begin(); cit != citSet->end(); ++cit) {
                moves = givePossibilities(*cit);
                if (poss.empty()) {
                    for (const std::vector<char>& elt : moves) {
                        poss.push_back(elt);
                    }
                }
                else {
                    end = poss.size();
                    for (int it = 0; it != end; ++it) {
                        for (std::set<std::vector<char>>::const_iterator it2 = moves.begin(); it2 != moves.end(); ++it2) {
                            if (it2 == moves.begin()) {
                                tmp = poss[it];
                                poss[it].insert(poss[it].end(), it2->begin(), it2->end());
                            }
                            else
                            {
                                std::vector<char> inFlight(tmp);
                                inFlight.insert(inFlight.end(), it2->begin(), it2->end());
                                poss.push_back(inFlight);
                            }
                        }
                    }
                }
            }
            toReturn.insert(poss.begin(), poss.end());
        }
        return toReturn;
    }

    std::set<std::vector<char>> getPossibilitiesP2(const std::vector<char>& code) {
        std::set<std::vector<char>> moves;
        std::vector<char> tmp;
        std::vector<std::vector<char>> poss;
        std::set<std::vector<char>> toReturn;
        int end;

        for (std::set<std::vector<char>>::const_iterator citSet = code.begin(); citSet != code.end(); ++citSet) {
            poss.clear();
            for (std::vector<char>::const_iterator cit = citSet->begin(); cit != citSet->end(); ++cit) {
                moves = givePossibilities(*cit);
                if (poss.empty()) {
                    for (const std::vector<char>& elt : moves) {
                        poss.push_back(elt);
                    }
                }
                else {
                    end = poss.size();
                    for (int it = 0; it != end; ++it) {
                        for (std::set<std::vector<char>>::const_iterator it2 = moves.begin(); it2 != moves.end(); ++it2) {
                            if (it2 == moves.begin()) {
                                tmp = poss[it];
                                poss[it].insert(poss[it].end(), it2->begin(), it2->end());
                            }
                            else
                            {
                                std::vector<char> inFlight(tmp);
                                inFlight.insert(inFlight.end(), it2->begin(), it2->end());
                                poss.push_back(inFlight);
                            }
                        }
                    }
                }
            }
            toReturn.insert(poss.begin(), poss.end());
        }
        return toReturn;
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
    Door door;
    Robot r1;
    Robot r2;

    if (getFileAndPart(21, input, part))
        return errno;

    while(!input.eof()) {
        getline(input, line);
        poss = door.getPossibilities(line);
        poss = r1.getPossibilities(poss);
        poss = r1.getPossibilities(poss);
        
        if (part == 1)
            res += getMin(poss).size() * getNumber(line);
        else {
            std::vector<char> minPoss;
            minPoss = getMin(poss);
            minPoss = r1.getPossibilitiesP2(poss);
            std::cout << poss << '\n';
            res += getMin(poss).size() * getNumber(line);
        }
    }

    std::cout << "result is " << res << '\n';

    return 0;
}