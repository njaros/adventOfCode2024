#include "utils/helpers.hpp"

typedef std::pair<std::optional<int>, bool> nbInput;

bool lookForMul(std::ifstream& input, ui part) {
    char c = input.get();
    int mul = 0;
    bool buildingMul = true;
    while (c != EOF) {
        switch (mul)
        {
            case 0:
                if (c == 'm')
                    ++mul;
                else if (part == 2 & c == 'd')
                {
                    ++mul;
                    buildingMul = false;
                }
                break;

            case 1:
                if (buildingMul & (c == 'u'))
                    ++mul;
                else if ((part == 2) & !buildingMul & (c == 'o'))
                    ++mul;
                else {
                    mul = 0;
                    buildingMul = true;
                }
                break;

            case 2:
                if (buildingMul & (c == 'l'))
                    ++mul;
                else if ((part == 2) & !buildingMul & (c == 'n'))
                    ++mul;
                else {
                    mul = 0;
                    buildingMul = true;
                }
                break;

            case 3:
                if (buildingMul & (c == '('))
                    return true;
                if ((part == 2) & !buildingMul & (c == '\''))
                    ++mul;
                else {
                    mul = 0;
                    buildingMul = true;
                }
                break;
            
            case 4:
                if (c == 't')
                    ++mul;
                else {
                    mul = 0;
                    buildingMul = true;
                }
                break;

            case 5:
                if (c == '(')
                    ++mul;
                else {
                    mul = 0;
                    buildingMul = true;
                }
                break;

            default:
                if (c == ')')
                    return false;
                mul = 0;
                buildingMul = true;
        }
        c = input.get();
    }
    return false;
}

bool lookForDo(std::ifstream& input) {
    int doo = 0;
    char c = input.get();

    while (c != EOF) {
        switch (doo) {
            case 0:
                if (c == 'd')
                    ++doo;
                break;

            case 1:
                if (c == 'o')
                    ++doo;
                else
                    doo = 0;
                break;

            case 2:
                if (c == '(')
                    ++doo;
                else
                    doo = 0;
                break;

            default:
                if (c == ')')
                    return true;
                doo = 0;
        }
        c = input.get();
    }
    return false;
}

nbInput lookNumberComa(std::ifstream& input) {
    nbInput res(std::nullopt, false);

    char c = input.get();
    while (c != EOF && isdigit(c)) {
        if (!res.first.has_value())
            res.first = 0;
        *res.first *= 10;
        *res.first += c - '0';
        c = input.get();
    }
    if (c == EOF)
        res.second = true;
    else if (res.first.has_value() && c != ',')
        res.first.reset();
    return res;
}

nbInput lookNumberEnd(std::ifstream& input) {
    nbInput res(std::nullopt, false);

    char c = input.get();
    while (c != EOF && isdigit(c)) {
        if (!res.first.has_value())
            res.first = 0;
        *res.first *= 10;
        *res.first += c - '0';
        c = input.get();
    }
    if (res.first.has_value() && c != ')')
        res.first.reset();
    if (c == EOF)
        res.second = true;
    return res;
}

int main() {
    std::ifstream input;
    ui part;
    int nbRight;
    int nbLeft;
    nbInput read;
    bool doo = true;
    long res = 0;

    if (getFileAndPart(3, input, part))
        return errno;
    while (!input.eof()) {
        if (doo) {
            if (lookForMul(input, part)) {
                read = lookNumberComa(input);
                if (!read.second & read.first.has_value()) {
                    nbLeft = read.first.value();
                    read = lookNumberEnd(input);
                    if (read.first.has_value()) {
                        nbRight = read.first.value();
                        res += nbLeft * nbRight;
                    }
                }
            }
            else
                doo = false;
        }
        else {
            if (lookForDo(input))
                doo = true;
        }
    }

    std::cout << "result is " << res << '\n';

    return 0;
}