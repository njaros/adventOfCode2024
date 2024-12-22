#include "utils/helpers.hpp"

typedef std::map<std::string, ull> Cache;

ull combinationExists(const std::string& pattern, std::string::size_type idx, const std::vector<std::string>& towels, Cache& cache) {
    std::string::size_type len;
    ull totalPart = 0;
    ull childScore;
    Cache::const_iterator cached;
    bool contain;
    
    cached = cache.find(pattern.substr(idx));
    if (cached != cache.end())
        return cached->second;

    if (idx == pattern.size())
        return 1;

    for (std::string towel : towels) {
        len = towel.size();
        if (len + idx <= pattern.length()) {
            contain = true;
            for (std::string::size_type i = 0; i < len; ++i)
            {
                if (pattern[i + idx] != towel[i])
                {
                    contain = false;
                    break;
                }
            }
            if (contain) {
                childScore = combinationExists(pattern, idx + len, towels, cache);
                if (childScore)
                    cache[pattern.substr(idx + len)] = childScore;
                totalPart += childScore;
            }
        }
    }
    return totalPart;
}

int main() {
    ui part = 0;
    std::ifstream input;
    std::vector<std::string> towels;
    std::string line;
    ull result = 0;
    Cache cache;

    if (getFileAndPart(19, input, part))
        return errno;

    getline(input, line);
    towels = inputLib::split(line, ", ");
    getline(input, line);

    while (!input.eof())
    {
        getline(input, line);
        if (part == 1)
            result += combinationExists(line, 0, towels, cache) != 0;
        else
            result += combinationExists(line, 0, towels, cache);
    }

    std::cout << "result is " << result << '\n';
    
    return 0;
}