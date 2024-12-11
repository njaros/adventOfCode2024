#include "utils/helpers.hpp"

typedef std::map<ull, ull> Stones;

ui numberSize(ull nb) {
    ui div = 1;
    while (nb / 10) {
        ++div;
        nb /= 10;
    }

    return div;
}

std::pair<ull, ull> splitNumbers(ull nb, ui size) {
    ull right = 0;
    std::list<ull> rightElts;
    
    while (size--) {
        rightElts.push_front(nb % 10);
        nb /= 10;
    }
    for (ull elt : rightElts) {
        right *= 10;
        right += elt;
    }
    return std::make_pair(nb, right);
}

Stones recu(int depth, const Stones& stones) {
    Stones nextStones;
    Stones::const_iterator cit = stones.begin();
    ui size;
    std::pair<ull, ull> splitted;

    if (!depth)
        return stones;

    while (cit != stones.end()) {
        if (cit->first == 0)
            nextStones[1] += cit->second;
        else {
            size = numberSize(cit->first);
            if (!(size % 2)) {
                splitted = splitNumbers(cit->first, size / 2);
                nextStones[splitted.first] += cit->second;
                nextStones[splitted.second] += cit->second;
            }
            else
                nextStones[cit->first * 2024] += cit->second;
        }
        ++cit;
    }
    return recu(depth - 1, nextStones);
}

ull getSizeMap(const Stones& stones) {
    ull res = 0;
    
    Stones::const_iterator cit = stones.begin();
    while (cit != stones.end()) {
        res += cit->second;
        ++cit;
    }
    return res;
}

int main() {
    ui part = 0;
    std::ifstream input;
    Stones stones;
    inputLib::extracted extracted;

    if (getFileAndPart(11, input, part))
        return errno;

    while (extracted.second != EOF) {
        extracted = inputLib::extractNextNumber(input);
        if (!extracted.first.has_value())
            throw std::invalid_argument("bad file format");
        stones.insert(std::make_pair(extracted.first.value(), 1));
    }

    int recuTimes = part == 1 ?
                    25:
                    75;

    stones = recu(recuTimes, stones);

    std::cout << "result is " << getSizeMap(stones) << '\n';

    return 0;
}