#include "utils/helpers.hpp"

typedef std::map<char, std::vector<Coord>> Antennas;

size_t countAntinodes(const Grid<char>& grid, const Antennas& ants, ui part) {
	Coord dif;
	Coord anti;
	std::set<Coord> antinodes;

	for (Antennas::const_iterator freq = ants.begin(); freq != ants.end(); ++freq) {
		for (int i = 0; i < freq->second.size() - 1; ++i) {
			for (int j = i + 1; j < freq->second.size(); ++j) {
				dif = freq->second[i] - freq->second[j];
				if (part == 1) {
					anti = dif + freq->second[i];
					if (grid.inBounds(anti))
						antinodes.insert(anti);
					anti = freq->second[j] - dif;
					if (grid.inBounds(anti))
						antinodes.insert(anti);
				}
				else {
					anti = freq->second[i];
					while (grid.inBounds(anti)) {
						antinodes.insert(anti);
						anti += dif;
					}
					anti = freq->second[j];
					while (grid.inBounds(anti)) {
						antinodes.insert(anti);
						anti -= dif;
					}
				}
			}
		}
	}
	return antinodes.size();
}

int main() {
	ui part = 0;
	std::ifstream input;
	Antennas ants;
	std::string line;
	Grid<char> grid;
	Grid<char>::browser it = std::nullopt;
	char charRead;

	if (getFileAndPart(8, input, part))
		return errno;

	while (!input.eof()) {
		getline(input, line);
		grid.addBackLine(line);
	}

	it = grid.lineBrowse(it);
	while (it.has_value()) {
		charRead = *(std::get<2>(it.value()));
		if (charRead != '.')
			ants[charRead].push_back(std::get<0>(it.value()));
		it = grid.lineBrowse(it);
	}

	std::cout << "result is " << countAntinodes(grid, ants, part) << '\n';

	return 0;
}