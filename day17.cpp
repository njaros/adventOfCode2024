#include "utils/helpers.hpp"

class Machine {
    
    int combo(int n) {
        switch (n)
        {
        case 4:
            n = A;
            break;
        case 5:
            n = B;
            break;
        case 6:
            n = C;
            break;
        case 7:
            throw(std::logic_error("7 incorrect for combo operand"));
        default:
            break;
        }
        return n;
    }
    
    void adv(int n) {
        A = A / pow(2, combo(n));
        ptr += 2;
    }

    void bxl(int n) {
        B = B ^ n;
        ptr += 2;
    }

    void bst(int n) {
        B = combo(n) % 8;
        ptr += 2;
    }

    void jnz(int n) {
        if (A != 0) {
            if (ptr != n)
                ptr = n;
            else
                ptr += 2;
        }
        else
            ptr += 2;
    }

    void bxc(int n) {
        B = B ^ C;
        ptr += 2;
    }

    void out(int n) {
        output.push_back(combo(n) % 8);
        ptr += 2;
    }

    void bdv(int n) {
        B = A / pow(2, combo(n));
        ptr += 2;
    }

    void cdv(int n) {
        C = A / pow(2, combo(n));
        ptr += 2;
    }
    
    void fun() {
        int opcode = program[ptr];
        int n = program[ptr + 1];
        switch (opcode)
        {
        case 0:
            return adv(n);
        case 1:
            return bxl(n);
        case 2:
            return bst(n);
        case 3:
            return jnz(n);
        case 4:
            return bxc(n);
        case 5:
            return out(n);
        case 6:
            return bdv(n);
        case 7:
            return cdv(n);
        }
    }

    public:

    ull A;
    ull B;
    ull C;
    int ptr;
    std::vector<ull> program;
    std::vector<ull> output;


    Machine(): A(0), B(0), C(0), ptr(0) {}

    Machine(int a, int b, int c, int p): A(a), B(b), C(c), ptr(p) {}

    void run() {
        while (ptr < program.size()) {
            fun();
        }
    }

};

std::ostream& operator<<(std::ostream& o, const Machine& m) {
    o << "A= " << m.A << "\nB= " << m.B << "\nC= " << m.C << "\nprog= " << m.program << "\noutPUTE= " << m.output << '\n';
    return o;
}

ull printed(ull A) {
    return (((7 - (A % 8)) ^ (A >> (7 - (A % 8)))) ^ 4) % 8;
}

void recuP2(const std::vector<ull>& goal, int idx, ull A, std::set<ull>& res) {
    ull next;
    A *= 8;
    for (ull i = 0; i < 8; ++i) {
        next = A + i;
        if (goal[idx] == printed(next))
        {
            if (idx == 0)
                res.insert(next);
            else
                recuP2(goal, idx - 1, next, res);
        }
    }
}

int main() {
    ui part = 0;
    std::ifstream input;
    Machine m;

    if (getFileAndPart(17, input, part))
        return errno;

    m.A = inputLib::extractNextNumber(input).first.value();
    m.B = inputLib::extractNextNumber(input).first.value();
    m.C = inputLib::extractNextNumber(input).first.value();

    while (!input.eof())
        m.program.push_back(inputLib::extractNextNumber(input).first.value());

    if (part == 1) {
        m.run();
        std::cout << "result is ";
        for (int i = 0; i < m.output.size(); ++i) {
            if (i > 0)
                std::cout << ',';
            std::cout << m.output[i];
        }
        std::cout << '\n';
    }
    else 
    {
        std::set<ull> result;
        recuP2(m.program, m.program.size() - 1, 0, result);
        std::cout << "result is " << *(result.begin()) << '\n';
    }
    return 0;
}