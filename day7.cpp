#include "utils/helpers.hpp"

typedef std::vector<ull>::const_iterator Cit;

ull searchComb(Cit next, Cit end, ull current, ull goal, ui part) {
	ull multRes;
	ull addRes;
	
	if (current > goal)
		return 0;
	if (next == end) {
		if (goal == current)
			return goal;
	}
	else {
		multRes = searchComb(next + 1, end, current * *next, goal, part);
		if (multRes)
			return multRes;
		addRes = searchComb(next + 1, end, current + *next, goal, part);
		if (addRes)
			return addRes;
		if (part != 1)
			return searchComb(next + 1, end, concatNumbers(current, *next), goal, part);
	}
	return 0;
}

int main() {
	ui part = 0;
	std::ifstream input;
	ull res = 0;
	ull toFind;
	std::vector<ull> numbers;
	inputLib::extracted nbRead;

	if (getFileAndPart(7, input, part))
		return errno;

	while (nbRead.second != EOF) {
		numbers.clear();
		nbRead = inputLib::extractNextNumberEndline(input);
		toFind = nbRead.first.value();
		while (nbRead.second != '\n' && nbRead.second != EOF) {
			nbRead = inputLib::extractNextNumberEndline(input);
			numbers.push_back(nbRead.first.value());
		}
		res += searchComb(numbers.cbegin() + 1, numbers.cend(), numbers.front(), toFind, part);
	}
	std::cout << "result is " << res << '\n';

	return 0;
}