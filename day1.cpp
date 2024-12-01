#include "utils/helpers.hpp"

int main(void) {
    unsigned int part;
    std::ifstream input;    
    std::string line;
    std::priority_queue<int> right;
    std::priority_queue<int> left;
    std::vector<int> leftVec;
    std::map<int, int> rightMap;
    std::pair<std::optional<long>, char> extracted;
    long result = 0;

    if (getFileAndPart(1, &input, &part))
        return errno;

    while (extracted.second != EOF) {
        if (part == 1) {
            extracted = inputLib::extractNextNumber(input);
            left.push(extracted.first.value());
            extracted = inputLib::extractNextNumber(input);
            right.push(extracted.first.value());
        }
        else {
            extracted = inputLib::extractNextNumber(input);
            leftVec.push_back(extracted.first.value());
            extracted = inputLib::extractNextNumber(input);
            rightMap[extracted.first.value()] += 1;
        }
    }
    if (part == 1) {
        while (!right.empty()) {
            result += abs(right.top() - left.top());
            right.pop();
            left.pop();
        }
    }
    else {
        std::map<int, int>::const_iterator found;
        for (int nb : leftVec) {
            found = rightMap.find(nb);
            if (found != rightMap.end())
                result += found->second * found->first;
        }
    }
    std::cout << "result is " << result << '\n';
    return 0;
}