#include "utils/helpers.hpp"

typedef std::set<std::string> Computers;
typedef std::map<std::string, Computers> Connections; 
typedef std::set<Computers> LanSet;

void addNetwork3(const std::string& current, const Computers& comps, const Connections& conn, LanSet& lan) {
    Computers node;
    Connections::const_iterator found;

    for (const std::string& comp: comps) {
        found = conn.find(comp);
        if (found != conn.end()) {
            node = found->second;
            for (const std::string& comp2: node) {
                if (comps.find(comp2) != comps.end()) {
                    lan.insert(Computers({current, comp, comp2}));
                }
            }
        }
    }
}

void addNetwork(const std::string& current, const Connections& conn, LanSet& lans, Computers lan, Computers& checked) {
    Connections::const_iterator found;

    if (checked.find(current) != checked.end())
        return;
    checked.insert(current);
    found = conn.find(current);

    if (lan.empty()) {
        lan.insert(current);
        for (const std::string& c : found->second) {
            addNetwork(c, conn, lans, lan, checked);
        }
    }

    else {
        found = conn.find(current);
        for (const std::string& c : lan) {
            if (found->second.find(c) == found->second.end()) {
                lans.insert(lan);
                return;
            }
        }
        lan.insert(current);
        for (const std::string& c : found->second) {
            addNetwork(c, conn, lans, lan, checked);
        }
    }
}

ui countT(const LanSet& lan) {
    ui count = 0;

    for (const Computers& comps : lan) {
        for (const std::string& c : comps) {
            if (c[0] == 't') {
                ++count;
                break;
            }
        }
    }
    return count;
}

std::string getHigherLan(const LanSet& l) {
    std::string res;
    LanSet::const_iterator bigger;
    int biggerSize = 0;

    for (LanSet::const_iterator c = l.begin(); c != l.end(); ++c) {
        if (c->size() > biggerSize) {
            biggerSize = c->size();
            bigger = c;
        }
    }

    for (const std::string& str: *bigger) {
        res += str;
        res += ',';
    }
    res.erase(res.end() - 1);
    return res;
}

int main() {
    ui part = 0;
    std::ifstream input;
    Computers checked;
    Connections conn;
    LanSet lans;
    std::string line;
    std::string right;
    std::string left;

    if (getFileAndPart(23, input, part)) {
        return errno;
    }

    while (!input.eof()) {
        getline(input, line);
        right = line.substr(3, 2);
        left = line.substr(0, 2);
        conn[right].insert(left);
        conn[left].insert(right);
    }

    if (part == 1) {

        for (Connections::const_iterator cit = conn.begin(); cit != conn.end(); ++cit) {
            addNetwork3(cit->first, cit->second, conn, lans);
        }

        std::cout << "result is " << countT(lans) << '\n';   
    }

    else {
        for (Connections::const_iterator cit = conn.begin(); cit != conn.end(); ++cit) {
            checked.clear();
            addNetwork(cit->first, conn, lans, Computers(), checked);
        }

        std::cout << "result is " << getHigherLan(lans) << '\n';
    }

    return 0;
}