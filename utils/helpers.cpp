//
// Created by njaros on 12/4/23.
//

#include "helpers.hpp"

Coord& operator+=(Coord& lhs, const Coord& rhs)
{
	lhs.first += rhs.first;
	lhs.second += rhs.second;
	return lhs;
}

Coord& operator-=(Coord& lhs, const Coord& rhs)
{
	lhs.first -= rhs.first;
	lhs.second -= rhs.second;
	return lhs;
}

Coord operator+(const Coord& lhs, const Coord& rhs)
{
	Coord ret;
	ret.first = lhs.first + rhs.first;
	ret.second = lhs.second + rhs.second;
	return ret;
}

Coord operator-(const Coord& lhs, const Coord& rhs)
{
	Coord ret;
	ret.first = lhs.first - rhs.first;
	ret.second = lhs.second - rhs.second;
	return ret;
}

Coord& operator*=(Coord& c, int i)
{
	c.first *= i;
	c.second *= i;
	return c;
}

Coord operator*(const Coord& c, int i)
{
	Coord cpy(c);
	return (cpy *= i);
}

std::ostream& operator<<(std::ostream& o, const Coord& c)
{
	return o << '(' << c.first << ", " << c.second << ')';
}

long manhattanDist(const Coord& a, const Coord& b)
{
	return (labs(a.first - b.first) + labs(a.second - b.second));
}

unsigned int secureGetNumber()
{
	std::string buffer;
	std::cin >> buffer;
	return atoi(buffer.c_str());
}

int getFileAndPart(int day, std::ifstream* in, unsigned int* part)
{
	if (in)
	{
		std::string toOpen = "./inputs/input" + std::to_string(day);
		in->open(toOpen);
		if (in->fail())
		{
			std::cerr << "Couldn't open file " << toOpen << " : " << strerror(errno) << std::endl;
			return 1;
		}
	}
	if (part)
	{
		std::cout << "which part ? (1 or 2)\r\n";
		std::cin >> *part;
		if (*part != 2)
			*part = 1;
	}
	return 0;
}

long long intPow(long long a, long long b)
{
	long long res = 1;

	if (b < 0)
		return 0;
	while (b--)
		res *= a;
	return a;
}

std::optional< long long > divisible(long long a_, long long b_) /*work in progress*/
{
	long result = 1;
	int exp = 0;
	unsigned long long a;
	unsigned long long b;
	bool sign = ((a_ < 0) != (b_ < 0));
	unsigned long long bStart;
	unsigned long long bitReader = ((long long)1) << (0x3f);

	std::cerr << "divisible function isn't finished !!\n";
	if (!b_)
		return std::nullopt;
	if (!a_)
		return 0;
	if (a_ < 0)
		a = -a_;
	else
		a = a_;
	if (b_ < 0)
		b = -b_;
	else
		b = b_;
	if (a < b)
		return std::nullopt;

	while (!(bitReader & b))
		bitReader >>= 1;
	bStart = bitReader;
	while (bitReader)
	{
		if ((bitReader & b) != (bitReader & a))
			return std::nullopt;
		--bitReader;
	}
	bitReader = bStart << 1;
	while (bitReader)
	{
		if (bitReader & a)
			result += intPow(2, exp) + 1;
		bitReader <<= 1;
	}
	return result;
}

ui math::ManhattanDist(const Coord& a, const Coord& b)
{
	return (labs(a.first - b.first) + labs(a.second - b.second));
}

std::pair<long, bool> inputLib::extractNextNumber(std::ifstream& input, char& monitorChar)
{
	long res = 0;
	long sign = 1;
	while (monitorChar != EOF)
	{
		if (isdigit(monitorChar))
		{
			while (isdigit(monitorChar))
			{
				res *= 10;
				res += monitorChar - '0';
				monitorChar = input.get();
			}
			return std::make_pair(res * sign, true);
		}
		if (monitorChar != '+' && monitorChar != '-')
			sign = 1;
		else if (monitorChar == '-')
			sign = ~sign;
		monitorChar = input.get();
	}
	return std::make_pair(res, false);
}

std::pair< std::optional<long>, char > inputLib::extractNextNumber(std::ifstream& input)
{
	char monitorChar = input.get();
	long res = 0;
	long sign = 1;
	while (monitorChar != EOF)
	{
		if (isdigit(monitorChar))
		{
			while (isdigit(monitorChar))
			{
				res *= 10;
				res += monitorChar - '0';
				monitorChar = input.get();
			}
			return std::make_pair(res * sign, monitorChar);
		}
		if (monitorChar != '+' && monitorChar != '-')
			sign = 1;
		else if (monitorChar == '-')
			sign *= -1;
		monitorChar = input.get();
	}
	return std::make_pair(std::nullopt, monitorChar);
}

char inputLib::goToNextLine(std::ifstream& input, char& monitorChar, unsigned int times)
{
	while (times-- && monitorChar != EOF)
	{
		while (monitorChar != '\n' && monitorChar != EOF)
			monitorChar = input.get();
		if (monitorChar == '\n')
			monitorChar = input.get();
	}
	return monitorChar;
}

void inputLib::goToNextLine(std::ifstream& input, unsigned int times)
{
	char monitorChar = input.get();
	while (times-- && monitorChar != EOF)
	{
		while (monitorChar != '\n' && monitorChar != EOF)
			monitorChar = input.get();
	}
}

namespace experiment
{

	MyNatural::MyNatural()
	{
		for (int i = 0; i < _bitSize; ++i)
			_n[i] = false;
	}

	MyNatural::MyNatural(unsigned int n)
	{
		unsigned int	compareTo = 1;
		for (int i = 0; i < _bitSize; ++i)
		{
			_n[_bitSize - 1 - i] = n & compareTo;
			compareTo <<= 1;
		}
	}

	MyNatural::MyNatural(const MyNatural &other)
	{
		*this = other;
	}

	MyNatural&	MyNatural::operator=(const MyNatural &other)
	{
		if (&other != this) {
			for (int i = 0; i < _bitSize; ++i)
				_n[i] = other._n[i];
		}
		return *this;
	}

	const bool*	MyNatural::getData() const {return _n;}
	int	MyNatural::getSize() const {return _bitSize;}
	unsigned int	MyNatural::getUnsignedInt() const
	{
		unsigned int	displayable = 0;
		unsigned int	constructor = 1;
		for (int i = 0; i < _bitSize; ++i)
		{
			if (_n[_bitSize - 1 - i])
				displayable |= constructor;
			constructor <<= 1;
		}
		return displayable;
	}

	MyNatural&	MyNatural::operator+=(const MyNatural &rhs)
	{
		for (int i = _bitSize - 1; i >= 0; --i)
		{
			if (rhs._n[i])
			{
				if (!_n[i])
					_n[i] = true;
				else
				{
					int j = i;
					while (_n[j] && j >= 0)
					{
						_n[j] = false;
						--j;
					}
					if (j >= 0)
						_n[j] = true;
				}
			}
		}
		return *this;
	}

	std::ostream&	operator<<(std::ostream &o, const MyNatural &n)
	{
		unsigned int	displayable = 0;
		unsigned int	constructor = 1;
		int	bitSize = n.getSize();
		const bool	*data = n.getData();
		o << '[';
		for (int i = 0; i < bitSize; ++i)
		{
			if (i && (!(i % 4)))
				o << ' ';
			o << data[i];
			if (data[bitSize - 1 - i])
				displayable |= constructor;
			constructor <<= 1;
		}
		o << "] = " << displayable;
		return o;
	}
}