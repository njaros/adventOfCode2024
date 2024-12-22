#include "utils/helpers.hpp"

#define RIGHT Coord(1, 0)
#define DOWN Coord(0, 1)
#define LEFT Coord(-1, 0)
#define UP Coord(0, -1)

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
            if (index.second < goal.second) {
                instructions.push_back('v');
                index += DOWN;
            }
            else if (index.first > goal.first) {
                instructions.push_back('<');
                index += LEFT;
            }
            else if (index.second > goal.second) {
                instructions.push_back('^');
                index += UP;
            }
            else {
                instructions.push_back('>');
                index += RIGHT;
            }
        }
        return instructions;
    }

};

class Pad {
    
    public:

    Coord index;

    Pad() {
        index = robotPad.findOne('A').value();
    }

    std::vector<char> goTo(char n) {
        Coord goal = doorPad.findOne(n).value();
        std::vector<char> instructions;

        while (index != goal) {
            if (index.second > goal.second) {
                instructions.push_back('^');
                index += UP;
            }
            else if (index.first < goal.first) {
                instructions.push_back('>');
                index += RIGHT;
            }
            else if (index.second < goal.second) {
                instructions.push_back('v');
                index += DOWN;
            }
            else {
                instructions.push_back('<');
                index += LEFT;
            }
        }
        return instructions;
    }


};

int main() {
    ui part = 0;
    std::ifstream input;
    std::string line;
    ui res = 0;

    if (getFileAndPart(100, input, part))
        return errno;

    while(!input.eof()) {
        getline(input, line);
    }

    return 0;
}