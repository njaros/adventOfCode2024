#include "utils/helpers.hpp"

typedef std::vector<std::pair<int, int>> Memory;

ull calcCheckSum(size_t &pos, int len, int val) {
    ull res = 0;
    for (int i = 0; i < len; ++i)
        res += pos++ * val;
    return res;
}

ull calcCheckSumPart2(Memory::size_type pos, int len, int val) {
    ull res = 0;
    while (len--)
        res += pos++ * val;
    return res;
}

ull algoPart1(const std::string& line)
{
    size_t leftMost = 0;
    size_t memoryPos = 0;
    size_t rightMost;
    int valRemain = 0;
    int freeRemain = 0;
    ull res = 0;

    rightMost = line.length();
    if (rightMost % 2)
        ++rightMost;

    while (leftMost < rightMost)
    {
        res += calcCheckSum(memoryPos, line[leftMost] - '0', leftMost / 2);
        ++leftMost;
        freeRemain = line[leftMost] - '0';
        while (freeRemain) {
            if (!valRemain) {
                rightMost -= 2;
                valRemain = line[rightMost] - '0';
            }
            if (freeRemain < valRemain) {
                res += calcCheckSum(memoryPos, freeRemain, rightMost / 2);
                valRemain -= freeRemain;
                freeRemain = 0;
            }
            else {
                res += calcCheckSum(memoryPos, valRemain, rightMost / 2);
                freeRemain -= valRemain;
                valRemain = 0;
            }
        }
        ++leftMost;
    }
    res += calcCheckSum(memoryPos, valRemain, rightMost / 2);

    return res;
}

Memory buildMemory(const std::string& line) {
    Memory memory;
    bool freeSpace = false;
    int valRead;

    for (std::string::size_type idx = 0; idx < line.length(); ++idx) {
        valRead = line[idx] - '0';
        if (valRead)
        {
            if (freeSpace)
                memory.push_back(std::make_pair(valRead, -1));
            else
                memory.push_back(std::make_pair(valRead, idx / 2));
        }
        freeSpace = !freeSpace;
    }

    return memory;
}

void swapMemory(Memory& mem, Memory::size_type a, Memory::size_type b) {
    std::pair<int, int> tmp;

    tmp = mem[a];
    mem[a] = mem[b];
    mem[b] = tmp;
}

int concatFreeMem(Memory& mem, Memory::size_type idx)
{
    int reduc = 0;

    if (idx > 0 && mem[idx - 1].second == -1) {
        mem[idx].first += mem[idx - 1].first;
        mem.erase(mem.begin() + idx - 1);
        ++reduc;
        --idx;
    }

    if (idx + 1 < mem.size() && mem[idx + 1].second == -1) {
        mem[idx].first += mem[idx + 1].first;
        mem.erase(mem.begin() + idx + 1);
    }

    return reduc;
}

int mooveMemory(Memory& mem, Memory::size_type a, Memory::size_type b) {
    std::pair<int, int> tmpA = mem[a];
    std::pair<int, int> tmpB = mem[b];
    int dif = tmpA.first - tmpB.first;

    mem[a] = mem[b];
    mem[b] = std::make_pair(tmpA.first - dif, tmpA.second);

    mem.insert(mem.begin() + a + 1, std::make_pair(dif, tmpA.second));
    return 0;
}

Memory::size_type nextVal(const Memory& mem, Memory::size_type idx)
{
    while (mem[idx].second == -1)
        --idx;
    return idx;
}

size_t sumStr(const std::string& line) {
    size_t res = 0;
    for (char c : line) {
        res += c - '0';
    }
    return res;
}

ull algoPart2(const std::string &line) {
    Memory memory = buildMemory(line);
    Memory::size_type right = memory.size() - 1;
    Memory::size_type idxRight;
    Memory::size_type left;
    bool mooved;
    bool found;
    int freeSpace;
    ull res = 0;

    while (right) {
        found = false;
        if (memory[right].second != -1) {
            left = 0;
            while (left < right && !found)
            {
                if (memory[left].second == -1) {
                    if (memory[left].first > memory[right].first)
                    {
                        found = true;
                        mooveMemory(memory, left, right);
                        ++right;
                    }
                    else if (memory[left].first == memory[right].first)
                    {
                        found = true;
                        swapMemory(memory, left, right);
                    }
                }
                ++left;
            }
        }
        --right;
    }

    int trueIdx = 0;

    for (Memory::size_type idx = 0; idx < memory.size(); ++idx) {
        if (memory[idx].second != -1)
            res += calcCheckSumPart2(trueIdx, memory[idx].first, memory[idx].second);
        trueIdx += memory[idx].first;
    }

    return res;
}

int main() {
    ui part;
    std::ifstream input;
    std::string line;

    if (getFileAndPart(9, input, part))
        return errno;
    
    getline(input, line);

    if (part == 1)
        std::cout << "result is " << algoPart1(line) << '\n';
    else
        std::cout << "result is " << algoPart2(line) << '\n';

    return 0;
}