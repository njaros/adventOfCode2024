#include "utils/helpers.hpp"

typedef std::vector<int> Sequence;
typedef std::vector<Sequence> Sequences;
typedef std::map<Sequence, std::set<Sequence>> Pairs;

void sortGrid(Grid<char>& g, Sequences& k, Sequences& l) {
    Sequence s;
    int count;
    bool start = true;
    Grid<char>::browser br;
    br = g.columnBrowse(br);
    while (br.has_value()) {
        if (std::get<1>(br.value()) == 1) {
            if (start) {
                start = false;
            }
            else
                s.push_back(count);
            count = -1;
        }
        if (g.get(std::get<0>(br.value())) == '#')
            ++count;
        br = g.columnBrowse(br);
    }
    s.push_back(count);
    if (g.get(Coord(0, 0)) == '#')
        l.push_back(s);
    else k.push_back(s);
}

bool fit(const Sequence& l, const Sequence& k) {
    for (int i = 0; i < 5; ++i) {
        if (l[i] + k[i] >= 6)
            return false;
    }
    return true;
}

int main() {
    std::ifstream input;
    ui part = 5;
    Pairs pairs;
    ui result = 0;
    Sequences keys;
    Sequences locks;
    Grid<char> read;
    std::string line;

    if (getFileAndPart(25, input, part))
        return errno;

    while (!input.eof()) {
        getline(input, line);
        if (line.empty()){
            sortGrid(read, keys, locks);
            read.clear();
        }
        else {
            read.addBackLine(line);
        }
    }
    sortGrid(read, keys, locks);

    for (const Sequence& k: keys) {
        for (const Sequence& l: locks) {
            if (fit(l, k)) {
                pairs[k].insert(l);
            }
        }
    }

    for (Pairs::const_iterator cit = pairs.begin(); cit != pairs.end(); ++cit) {
        result += cit->second.size();
    }

    std::cout << "result is " << result << '\n';

    return 0;
}