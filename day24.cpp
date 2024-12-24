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

std::string opToStr(int op) {
    switch (op)
    {
    case XOR:
        return " ^ ";
        break;
    case AND:
        return " & ";
        break;
    case OR:
        return " | ";
        break;
    

    default:
        return " = ";
        break;
    }
}

int main() {
    ui part;
    std::ifstream input;
    std::string line("init");
    std::vector<std::string> splitted;
    ull resP1 = 0;
    ull exp = 0;
    NodeMap np;
    std::set<std::string> resP2;

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
                it->second.evaluate(np);
                if (it->second.val)
                    resP1 += ullPow(2, exp);
                ++exp;
            }
        }
        std::cout << "result is " << resP1 << '\n';
    }

    else {
        for (NodeMap::iterator it = np.begin(); it != np.end(); ++it) {
            if (it->first[0] == 'z') {
                if (it->second.op != XOR) {
                    std::cout << it->first << " has wrong operator\n";
                    resP2.insert(it->first);
                }
                else {
                    int opL = np[it->second.left].op;
                    int opR = np[it->second.right].op;
                    // std::cout << opToStr(np[it->second.left].op) << " - " << opToStr(np[it->second.right].op) << '\n';
                    if (opL == AND || opR == AND) {
                        if (opL == AND) {
                            std::cout << it->second.left << " has wrong operator\n";
                            resP2.insert(it->second.left);
                        }
                        else {
                            std::cout << it->second.right << " has wrong operator\n";
                            resP2.insert(it->second.right);
                        }
                    }
                    else {
                        NodeMap::const_iterator itR = np.find(it->second.right);
                        NodeMap::const_iterator itL = np.find(it->second.left);
                        if (opR == OR) {
                            // std::cout << opToStr(np[itR->second.left].op) << " - " << opToStr(np[itR->second.right].op) << '\n';
                            if (np[itR->second.left].op != AND) {
                                std::cout << itR->second.left << " has wrong operator \n";
                                resP2.insert(itR->second.left);
                            }
                            if (np[itR->second.right].op != AND) {
                                std::cout << itR->second.right << " has wrong operator \n";    
                                resP2.insert(itR->second.right);
                            }
                        }
                        else if (opL == OR) {
                            // std::cout << opToStr(np[itL->second.left].op) << " - " << opToStr(np[itL->second.right].op) << '\n';
                            if (np[itL->second.left].op != AND) {
                                std::cout << itL->second.left << " has wrong operator \n";
                                resP2.insert(itL->second.left);
                            }
                            if (np[itL->second.right].op != AND) {
                                std::cout << itL->second.right << " has wrong operator \n";
                                resP2.insert(itL->second.right);
                            }
                        }
                    }
                }
            }
        }
        for (const std::string& elt : resP2) {
            std::cout << elt << ',';
        }
        std::cout << '\n';
        std::cout << "not jfb,jgt,mht,nbf,z05,z09,z30,z45\n";
        std::cout << "not gbf,jfb,jgt,mht,nbf,z05,z09,z30\n";
    }

    return 0;
}