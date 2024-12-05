#include "utils/helpers.hpp"

typedef std::map<int, std::set<int>> Rules;

int fixUpdate(const std::vector<int>& up, const Rules& a, const Rules& b) {
    std::list<int> fix;
    int idx;
    std::list<int>::const_iterator cit;

    for (int elt: up) {
        cit = fix.begin();
        while (cit != fix.end() && b.find(elt)->second.find(*cit) == b.find(elt)->second.end()
                && a.find(*cit)->second.find(elt) == a.find(*cit)->second.end())
            ++cit;
        fix.insert(cit, elt);
    }

    idx = 0;
    cit = fix.begin();
    while (idx < fix.size() / 2) {
        ++idx;
        ++cit;
    }
    return *cit;
}

int checkUpdate(const std::vector<int>& up, const Rules& a, const Rules& b, ui part) {
    int current;
    int before;
    int after;
    for (int i = 0; i < up.size(); ++i) {
        current = up[i];
        for (int j = 0; j < i; ++j) {
            before = up[j];
            if (a.find(before)->second.find(current) != a.find(before)->second.end()
                || b.find(current)->second.find(before) != b.find(current)->second.end())
                return part == 1 ? 0 : fixUpdate(up, a, b);
        }
        for (int k = i + 1; k < up.size(); ++k) {
            after = up[k];
            if (a.find(current)->second.find(after) != a.find(current)->second.end()
                || b.find(after)->second.find(current) != b.find(after)->second.end())
                return part == 1 ? 0 : fixUpdate(up, a, b);
        }
    }
    return part == 1 ? up[up.size() / 2] : 0;
}

int main() {
    ui part = 1;
    std::ifstream input;
    inputLib::extracted extracted;
    ui res = 0;
    int first;
    int second;
    Rules beforeRules;
    Rules afterRules;
    std::vector<int> update;

    if (getFileAndPart(5, input, part))
        return errno;

    extracted = inputLib::extractNextNumberEndline(input);
    while (extracted.first.has_value()) {
        first = extracted.first.value();
        extracted = inputLib::extractNextNumberEndline(input);
        second = extracted.first.value();
        beforeRules[first].insert(second);
        afterRules[second].insert(first);
        extracted = inputLib::extractNextNumberEndline(input);
    }

    while (extracted.second != EOF) {
        update.clear();
        extracted.second = 0;
        while (extracted.second != '\n' && extracted.second != EOF) {
            extracted = inputLib::extractNextNumberEndline(input);
            update.push_back(extracted.first.value());
        }
        res += checkUpdate(update, afterRules, beforeRules, part);
    }

    std::cout << "result is " << res << '\n';

    return 0;
}