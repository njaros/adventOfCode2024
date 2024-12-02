#include "utils/helpers.hpp"

bool sameSign(int a, int b) {
    return (a < 0 && b < 0 || a > 0 && b > 0);
}

bool diffInRange(int a) {
    return (abs(a) <= 3 && a != 0);
}

std::vector<int> removeElt(const std::vector<int>& src, std::vector<int>::const_iterator toRemove) {
    std::vector<int> cpyRemoved;

    for (std::vector<int>::const_iterator cpyIt = src.begin(); cpyIt != src.end(); ++cpyIt) {
        if (cpyIt != toRemove) {
            cpyRemoved.push_back(*cpyIt);
        }
    }
    return cpyRemoved;
}

bool filter(const std::vector<int>& nbrs, bool tolerate) {
    int mem;
    bool started = false;
    int memDif = 0;
    int dif;

    for (std::vector<int>::const_iterator cit = nbrs.begin(); cit != nbrs.end(); ++cit) {
        if (!started) {
            started = true;
            mem = *cit;
        }
        else {
            dif = mem - *cit;
            if (!diffInRange(dif) || (memDif != 0 && !sameSign(dif, memDif)))
            {
                if (tolerate) {
                    if (cit - 1 != nbrs.begin())
                        return (filter(removeElt(nbrs, cit), false) | filter(removeElt(nbrs, cit - 1), false) | filter(removeElt(nbrs, cit - 2), false));
                    return (filter(removeElt(nbrs, cit), false) | filter(removeElt(nbrs, cit - 1), false));
                }
                return false;
            }
            memDif = dif;
            mem = *cit;
        }
    }
    return true;
}

int main() {
    ui part;
    std::ifstream input;
    std::vector<int> nbrs;
    inputLib::extracted extracted;
    int result = 0;

    if (getFileAndPart(2, input, part))
        return errno;
    
    while (extracted.second != EOF) {
        extracted = inputLib::extractNextNumber(input);
        nbrs.push_back(extracted.first.value());
        if (extracted.second == '\n' || extracted.second == EOF) {
            result += filter(nbrs, part != 1);
            nbrs.clear();
        }
    }

    std::cout << "result is " << result << '\n';
    
    return 0;
}