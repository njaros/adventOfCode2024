#include "utils/helpers.hpp"

#define UNKNOWN 0
#define XOR 1
#define OR 2
#define AND 3
#define LEAF 4

class Node {

public:

    std::string name;
    std::string left;
    std::string right;
    int op;
    bool has_val;
    bool val;

    Node() {}
    Node(const std::string& n, const std::string& l, const std::string& r, int o) :
    name(n), left(l), right(r), op(o), has_val(false), val(false) {}
    Node(const std::string& n, int v) :
    name(n), left(""), right(""), op(LEAF), has_val(true), val(v) {}
    Node(const Node& o) :
    name(o.name), left(o.left), right(o.right), op(o.op), has_val(o.has_val), val(o.val) {}

    bool evaluate(std::map<std::string, Node>& np) {
        bool leftVal;
        bool rightVal;

        if (has_val)
            return val;
        
        leftVal = np[left].evaluate(np);
        rightVal = np[right].evaluate(np);

        switch (op)
        {
        case XOR:
            val = leftVal ^ rightVal;
            break;
        case AND:
            val = leftVal & rightVal;
            break;
        case OR:
            val = leftVal | rightVal;
            break;
        
        default:
            std::cout << "unknown operand\n";
            break;
        }
        has_val = true;
        return val;
    }
};

typedef std::map<std::string, Node> NodeMap;

int getOperand(std::string op) {
    if (op == "XOR")
        return XOR;
    if (op == "AND")
        return AND;
    return OR;
}

int main() {
    ui part;
    std::ifstream input;
    std::string line("init");
    std::vector<std::string> splitted;
    ull resP1 = 0;
    ull exp = 0;
    NodeMap np;

    if (getFileAndPart(24, input, part))
        return errno;

    while (!line.empty()) {
        getline(input, line);
        inputLib::carriageReturnDel(line);
        np[line.substr(0, 3)] = Node(line.substr(0, 3), line[5] == '1' ? true : false);
    }

    while (!input.eof()) {
        getline(input, line);
        inputLib::carriageReturnDel(line);
        splitted = inputLib::split(line, " ");
        np[splitted[4]] = Node(splitted[4], splitted[0], splitted[2], getOperand(splitted[1]));
    }

    if (part == 1) {
        for (NodeMap::iterator it = np.begin(); it != np.end(); ++it) {
            if (it->first[0] == 'z') {
                std::cout << it->first << "-> " << it->second.evaluate(np) << '\n';
                if (it->second.val)
                    resP1 += ullPow(2, exp);
                ++exp;
            }
        }
        std::cout << "result is " << resP1 << '\n';
    }

    return 0;
}