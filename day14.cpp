#include "utils/helpers.hpp"

class Robot {

	Coord _c;
	Coord _s;
	static const int _maxX = 100;
	static const int _maxY = 102;

	public:

	Robot(Coord c, Coord s) : _c(c), _s(s)
	{}

	Robot(const Robot& other): _c(other._c), _s(other._s)
	{}

	void next(int loop, Grid<int> & g) {
		while (loop--) {
			g.get(_c) -= 1;
			_c = Coord(
				math::myModulo(_c.first + _s.first, _maxX + 1),
				math::myModulo(_c.second + _s.second, _maxY + 1)
			);
			g.get(_c) += 1;
		}
	}

	int getQuadrant() const {
		if ((_c.first == _maxX / 2) || (_c.second == _maxY / 2))
			return 0;
		if (_c.first < _maxX / 2)
			return _c.second < _maxY / 2 ? 1 : 2;
		return _c.second < _maxY / 2 ? 3 : 4;
	}

	Coord getPos() const {
		return this->_c;
	}

	Coord getSpeed() const {
		return this->_s;
	}
};

std::ostream& operator<<(std::ostream& o, const Robot& r) {
	o << "p= " << r.getPos() << '\n';
	o << "s= " << r.getSpeed() << '\n';
	return o;
}

bool align10(const Grid<int>& g) {
	for (int i = 0; i < g.size(); ++i) {
		for (int j = 0; j < g[i].size() - 11; ++j) {
			if (g[i][j] != 0) {
				for (int k = j + 1; k < j + 11; ++k) {
					if (g[i][k] == 0)
						break;
					if (k == j + 10)
						return true;
				}
			}
		}
	}
	return false;
}

void displayGrid(const Grid<int>& g) {
	for (int i = 0; i < g.size(); ++i) {
		std::cout << '\n';
		for (int j = 0; j < g[i].size(); ++j)
		{
			if (g[i][j] == 0)
				std::cout << '.';
			else
				std::cout << g[i][j];
		}
	}
}

int main() {
	ui part = 0;
	std::ifstream input;
	inputLib::extracted ext;
	std::vector<Robot> robots;
	std::map<int, long> families;
	Grid<int> grid;
	Coord c;
	Coord v;

	for (int i = 0; i < 103; ++i)
	{
		grid.addEmptyLine();
		for (int j = 0; j < 101; ++j) {
			grid.back().push_back(0);
		}
	}

	if (getFileAndPart(14, input, part))
		return errno;

	while (!input.eof()) {
		c.first = inputLib::extractNextNumber(input).first.value();
		c.second = inputLib::extractNextNumber(input).first.value();
		v.first = inputLib::extractNextNumber(input).first.value();
		v.second = inputLib::extractNextNumber(input).first.value();
		robots.push_back(Robot(c, v));
	}

	std::cout << robots.size() << '\n';

	if (part == 1)
	{

		for (Robot& r : robots) {
			r.next(100, grid);
			if (r.getQuadrant() != 0)
				families[r.getQuadrant()] += 1;
		}

		std::cout << families << '\n';

		long long res = 1;

		for (std::map<int, long>::const_iterator cit = families.begin(); cit != families.end(); ++cit) {
			res *= cit->second;
		}

		std::cout << "result is " << res << '\n';
	}

	else {
		int a;
		int l = 0;

		for (int i = 0; i < robots.size(); ++i) {
			std::cout << robots[i].getPos() << '\n';
			grid.get(robots[i].getPos()) += 1;
		}


		while (1) {
			l++;
			for (Robot& r : robots) {
				r.next(1, grid);
			}
			if (align10(grid)) {
				displayGrid(grid);
				std::cout << '\n' << l << '\n';
				std::cin >> a;
			}
		}

	}

	return  0;
}