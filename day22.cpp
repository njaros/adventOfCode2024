#include "utils/helpers.hpp"

typedef std::deque<int> Sequence;

class Secret {

    void prune() {
        seed %= 16777216;
    }

    void mix(ull n) {
        seed ^= n;
    }

public:

    ull seed;

    std::map<Sequence, int> seqMap;

    Sequence currentSeq;

    Secret(ull n) : seed(n) {}

    Secret(const Secret& o) : seed(o.seed), seqMap(o.seqMap), currentSeq(o.currentSeq) {}

    void loop() {
        ull n;

        n = seed * 64;
        mix(n);
        prune();

        n = seed / 32;
        mix(n);
        prune();

        n = seed * 2048;
        mix(n);
        prune(); 
    }

    ull get2000() {
        ull tmp;

        for (int i = 0; i < 4; ++i) {
            tmp = seed % 10;
            loop();
            currentSeq.push_back((seed % 10) - tmp);
        }
        seqMap[currentSeq] = seed % 10;
        for (int i = 0; i < 1996; ++i) {
            tmp = seed % 10;
            loop();
            currentSeq.pop_front();
            currentSeq.push_back((seed % 10) - tmp);
            if (seqMap.find(currentSeq) == seqMap.end())
                seqMap[currentSeq] = seed % 10;
        }
        return seed;
    }

};

int getMax(const std::map<Sequence, int>& m) {
    int max = 0;

    for (std::map<Sequence, int>::const_iterator cit = m.begin(); cit != m.end(); ++cit) {
        if (cit->second > max)
            max = cit->second;
    }
    return max;
}

int main() {
    ui part = 0;
    ull res = 0;
    std::ifstream input;
    std::vector<Secret> secrets;
    std::map<Sequence, int> sequences;

    if (getFileAndPart(22, input, part))
        return errno;

    while (!input.eof()) {
        secrets.push_back(Secret(inputLib::extractNextNumber(input).first.value()));
        res += secrets.back().get2000();
    }
    if (part != 1) {
        for (const Secret& s : secrets) {
            for (std::map<Sequence, int>::const_iterator cit = s.seqMap.begin(); cit != s.seqMap.end(); ++cit) {
                sequences[cit->first] += cit->second;
            }
        }
        res = getMax(sequences);
    }

    std::cout << "result is " << res << '\n';

    return 0;
}