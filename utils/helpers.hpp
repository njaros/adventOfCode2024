//
// Created by njaros on 12/4/23.
//

#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <iostream>
#include <initializer_list>
#include <fstream>
#include <string>
#include <cstring>
#include <math.h>
#include <algorithm>
#include <tuple>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <deque>
#include <optional>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <bitset>
#include "Matrix.hpp"

//Usefull typedefs

typedef unsigned int ui;
typedef unsigned long long ull;
typedef long long ll;
typedef std::pair<long long, long long> Coord;
typedef std::pair<long, bool> NumberRead;

//Some << overloads

Coord& operator+=(Coord& lhs, const Coord& rhs);
Coord& operator-=(Coord& lhs, const Coord& rhs);
Coord operator+(const Coord& lhs, const Coord& rhs);
Coord operator-(const Coord& lhs, const Coord& rhs);
Coord& operator*=(Coord& c, int i);
Coord operator*(const Coord& c, int i);
Coord& operator/=(Coord& c, int i);
Coord operator/(const Coord& c, int i);

template <class T>
std::ostream& operator<<(std::ostream& o, const std::set<T>& s);

//Distance functions

long manhattanDist(const Coord& a, const Coord& b);

template <class T, class U>
std::ostream& operator<<(std::ostream& o, const std::pair<T, U>& p)
{
	o << p.first << " | " << p.second;
	return o;
}

template <class T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& v)
{
	for (const T& elt : v)
	{
		o << elt << ' ';
	}
	return o;
}

template <class T>
std::ostream& operator<<(std::ostream& o, const std::deque<T>& v)
{
	for (const T& elt : v)
	{
		o << elt;
	}
	return o;
}

template <class T>
std::ostream& operator<<(std::ostream& o, const std::set<T>& s)
{
	for (typename std::set<T>::const_iterator cit = s.begin(); cit != s.end(); ++cit)
	{
		o << *cit << ' ';
	}
	return o;
}

template <class T>
std::ostream& operator<<(std::ostream& o, const std::list<T>& l)
{
	for (typename std::list<T>::const_iterator cit = l.begin(); cit != l.end(); ++cit)
	{
		o << *cit << ' ';
	}
	return o;
}


std::ostream& operator<<(std::ostream& o, const Coord& c);

template <class T, class U>
std::ostream& operator<<(std::ostream& o, const std::map<T, U>& m)
{
	for (typename std::map<T, U>::const_iterator cit = m.begin(); cit != m.end(); ++cit)
	{
		o << cit->first << "-> " << cit->second << '\n';
	}
	return o;
}


//Usefull simple class and their typedefs

template <class T>
class Grid : public std::vector<std::vector<T>> {


	/*
	* isOk :
	* #####
	* ###
	* #
	* #
	*
	* isNotOk :
	*
	* ##
	* ####
	* ########
	*
	*/
	bool _isColumnBrowsable() const
	{
		if (this->empty() || this->front().empty())
			return false;
		typename line::size_type prev = this->front().size();
		for (const line& l : *this)
		{
			if (l.size() > prev)
				return false;
			prev = l.size();
		}
		return true;
	}

	bool _isReverseColumnBrowsable() const
	{
		if (this->empty() || this->back().empty())
			return false;
		typename line::size_type prev = this->back().size();
		for (typename daddy::size_type idx = this->size() - 1; idx != ~0; --idx)
		{
			if ((*this)[idx].size() > prev)
				return false;
			prev = (*this)[idx].size();
		}
		return true;
	}

public:

	typedef std::vector<T> line;
	typedef std::vector<T> column;
	typedef std::vector<std::vector<T>> daddy;
	typedef std::optional< std::tuple< Coord, bool, T* > > browser;

	Grid(): daddy() {}

	Grid(const std::initializer_list<std::initializer_list<T>> &il) {
		typedef typename std::initializer_list<std::initializer_list<T> > Kinilist;
		for (typename Kinilist::const_iterator cit = il.begin(); cit != il.end(); ++cit) {
			this->push_back(line(*cit));
		}
	}

	void addEmptyLine()
	{
		this->push_back(line());
	}

	template < class container >
	void addLine(size_t pos, const container& c)
	{
		typename daddy::iterator it;

		if (pos <= this->size())
		{
			it = this->insert(this->begin() + pos, line());
			for (const T& elt : c)
			{
				it->push_back(elt);
			}
		}
	}

	void addLine(size_t pos, const T& c) //optimizable
	{
		size_t sizeL;
		line l;

		if (!this->empty() && pos <= this->size())
		{
			if (pos)
				sizeL = this->at(pos - 1).size();
			else
				sizeL = this->front().size();
			l.assign(sizeL, c);
			this->insert(this->begin() + pos, l);
		}
	}

	template <class container>
	void addFrontLine(const container& c)
	{
		addLine(0, c);
	}

	void addFrontLine(const T& c)
	{
		addLine(0, c);
	}

	template <class container>
	void addBackLine(const container& c)
	{
		addLine(this->size(), c);
	}

	void addBackLine(const line& l)
	{
		this->push_back(l);
	}

	void addBackLine(const T& c)
	{
		size_t sizeL;
		line l;

		if (!this->empty())
		{
			sizeL = this->back().size();
			l.assign(sizeL, c);
			this->push_back(l);
		}
	}

	void appendInLastLine(const T& elt) {
		this->back().push_back(elt);
	}

	template <class container>
	void addColumn(size_t pos, const container& c)
	{
		typename container::const_iterator it = c.begin();

		if (c.size() != this->size())
			throw(std::invalid_argument("column doesn't have same size than the grid."));
		for (line& l : *this)
		{
			if (pos <= l.size())
			{
				l.insert(l.begin() + pos, *it);
			}
			else
				l.push_back(*it);
			++it;
		}
	}

	void addColumn(size_t pos, const T& c)
	{
		for (line& l : *this)
		{
			if (pos <= l.size())
			{
				l.insert(l.begin() + pos, c);
			}
			else
				l.push_back(c);
		}
	}

	void addFrontColumn(const column& c)
	{
		addColumn(0, c);
	}

	void addFrontColumn(const T& c)
	{
		addColumn(0, c);
	}

	void addBackColumn(const column& c)
	{
		addColumn(~0, c);
	}

	void addBackColumn(const T& c)
	{
		addColumn(~0, c);
	}

	Coord addBackElt(const T& elt)
	{
		if (this->empty())
			this->push_back(line());
		this->back().push_back(elt);
		return Coord((int)this->back().size() - 1, (int)this->size() - 1);
	}

	Coord addBackElt(const T& elt, const T& newLine)
	{
		if (this->empty())
			addEmptyLine();
		if (elt == newLine)
			addEmptyLine();
		else
			this->back().push_back(elt);
		return Coord((int)this->back().size() - 1, (int)this->size() - 1);
	}

	Coord addBackElt(const T& elt, const T& newLine, const std::set<T>& ignoreSet)
	{
		if (ignoreSet.find(elt) == ignoreSet.end())
			return this->addBackElt(elt, newLine);
		return Coord((int)this->back().size() - 1, (int)this->size() - 1);
	}

	void swapCells(Coord a, Coord b) {
		T tmp = this->get(a);
		this->get(a) = this->get(b);
		this->get(b) = tmp;
	}

	std::optional<Coord> findOne(const T& elt) const
	{
		for (size_t y = 0; y < this->size(); ++y)
		{
			for (size_t x = 0; x < this->at(y).size(); ++x)
			{
				if (this->at(y).at(x) == elt) {
					return Coord((int)x, (int)y);
				}
			}
		}
		return std::nullopt;
	}

	std::set<Coord> findAll(const T& elt) const
	{
		std::set<Coord> found;
		for (size_t y = 0; y < this->size(); ++y)
		{
			for (size_t x = 0; x < this->at(y).size(); ++x)
			{
				if (this->at(y).at(x) == elt)
					found.insert(Coord((int)x, (int)y));
			}
		}
		return found;
	}

	const line& getLine(size_t pos) const
	{
		return this[pos];
	}

	const column getcolumn(size_t pos) const
	{
		column ret;
		for (const line& l : *this)
		{
			ret.push_back(l[pos]);
		}
		return ret;
	}

	const T& get(const Coord& c) const
	{
		return this->at(c.second).at(c.first);
	}

	T& get(const Coord& c)
	{
		return this->at(c.second).at(c.first);
	}

	template <class container>
	void insertElems(const container& cont, const T& elem) {
		for (typename container::const_iterator cit = cont.begin(); cit != cont.end(); ++cit)
			get(*cit) = elem;
	}

	const T& operator()(const Coord& c) const
	{
		return get(c);
	}

	T& operator()(const Coord& c)
	{
		return get(c);
	}

	T* getPtr(const Coord& c)
	{
		return &(get(c));
	}

	T getCopy(const Coord& c) const
	{
		return this->at(c.second).at(c.first);
	}

	// Tools to "easily" browse the grid

	browser columnBrowse(const browser& current)
	{
		typename line::size_type max;
		Coord nextCoord;

		if (!_isColumnBrowsable())
			return std::nullopt;

		nextCoord = Coord(0, 0);
		max = this->front().size();
		if (!current.has_value())
			return std::make_tuple(nextCoord, 1, getPtr(nextCoord));

		nextCoord = std::get<0>(current.value()) + Coord(0, 1);
		if (nextCoord.second >= this->size() || nextCoord.first >= (*this)[nextCoord.second].size())
		{
			nextCoord = Coord(nextCoord.first + 1, 0);
			if (nextCoord.first >= max)
				return std::nullopt;
			return std::make_tuple(nextCoord, 1, getPtr(nextCoord));
		}
		return std::make_tuple(nextCoord, 0, getPtr(nextCoord));
	}

	browser columnBrowseReverse(const browser& current)
	{
		Coord nextCoord;

		if (!_isReverseColumnBrowsable())
			return std::nullopt;

		if (!current.has_value())
		{
			nextCoord = Coord((int)this->back().size() - 1, (int)this->size() - 1);
			return std::make_tuple(nextCoord, 1, getPtr(nextCoord));
		}

		nextCoord = std::get<0>(current.value()) + Coord(0, -1);
		if (nextCoord.second < 0)
		{
			nextCoord = Coord(nextCoord.first - 1, (int)this->size() - 1);
			if (nextCoord.first < 0)
				return std::nullopt;
			return std::make_tuple(nextCoord, 1, getPtr(nextCoord));
		}
		return std::make_tuple(nextCoord, 0, getPtr(nextCoord));
	}

	browser lineBrowse(const browser& current)
	{
		Coord nextCoord;

		if (!current.has_value())
		{
			nextCoord = Coord(0, 0);
			return std::make_tuple(nextCoord, 1, getPtr(nextCoord));
		}

		nextCoord = std::get<0>(current.value()) + Coord(1, 0);
		if (nextCoord.first >= this->at(nextCoord.second).size())
		{
			nextCoord = Coord(0, nextCoord.second + 1);
			if (nextCoord.second >= this->size())
				return std::nullopt;
			return std::make_tuple(nextCoord, 1, getPtr(nextCoord));
		}
		return std::make_tuple(nextCoord, 0, getPtr(nextCoord));
	}

	bool inBounds(Coord c) const
	{
		if (c.second < 0 || c.first < 0)
			return false;
		if (c.second >= this->size() || c.first >= (*this)[c.second].size())
			return false;
		return true;
	}

	bool isRectangle() const
	{
		size_t mem;
		if (this->empty())
			return false;
		mem = this->front().size();
		for (const line& l : *this)
		{
			if (l.size() != mem)
				return false;
		}
		return true;
	}

	bool isSquare() const
	{
		return isRectangle() && (this->size() == this->front().size());
	}
};

template <class T>
std::ostream& operator<<(std::ostream& o, const Grid<T>& g)
{
	for (const std::vector<T>& elt : g)
	{
		for (const T& u : elt) {
			o << u;
		}
		o << '\n';
	}
	return o;
}

//Some usefull functions I need each times

unsigned int secureGetNumber();

int getFileAndPart(int day, std::ifstream& in, unsigned int& part);

std::optional< long long > divisible(long long a, long long b);

long long intPow(long long a, long long b);

ull ullPow(ull a, ull b);

ull concatNumbers(ull a, ull b);

namespace math
{
	ui ManhattanDist(const Coord& a, const Coord& b);
	long myModulo(long a, long b);

	template<class T>
	bool nearZero(const T &val)
	{
		if (val <= 0.00001 && val >= -0.00001)
			return true;
		return false;
	}

}

namespace inputLib
{
	typedef std::pair< std::optional<long long>, char > extracted;

	std::pair<long long, bool> extractNextNumber(std::ifstream& input, char& monitorChar);

	extracted extractNextNumber(std::ifstream& input);

	extracted extractNextNumberEndline(std::ifstream& input);

	char goToNextLine(std::ifstream& input, char& monitorChar, unsigned int times = 1);

	void goToNextLine(std::ifstream& input, unsigned int times = 1);

	std::string& carriageReturnDel(std::string& line);

	std::vector<std::string> split(const std::string& str, const char* delim);

}
//Usefull class and containers

template<class T, class U>
class Tries
{
	std::allocator< Tries < T, U > > Alloc;
	std::allocator_traits< std::allocator< Tries< T, U > > > _alloc;
	std::map< T, Tries< T, U >* > _childs;
	U _value;
	size_t _weight;
	bool _valueReadable;

	template<class container>
	U& _addRecu(const container& str, const U& v, typename container::const_iterator cit, bool override)
	{
		bool writeValue = override;
		if (cit == str.end())
		{
			_valueReadable = true;
			if (override)
				_value = v;
			return _value;
		}
		else
		{
			Tries* child = 0;
			if (_childs.find(*cit) == _childs.end())
			{
				child = _alloc.allocate(Alloc, 1);
				_alloc.construct(Alloc, child, Tries< T, U >());
				_childs[*cit] = child;
				writeValue = true;
			}
			else
				child = _childs[*cit];
			return child->_addRecu(str, v, ++cit, writeValue);
		}
	}

	template<class container>
	std::pair<U, bool> _search(const container& c, typename container::const_iterator cit) const
	{
		if (cit == c.end())
		{
			if (_valueReadable)
				return std::make_pair(_value, true);
			return std::make_pair(U(), false);
		}
		else
		{
			if (_childs.find(*cit) == _childs.end())
				return std::make_pair(U(), false);
			return _childs.find(*cit)->second->_search(c, ++cit);
		}
	}

	void _print(std::vector< T > v) const
	{
		if (_valueReadable)
			std::cout << v << " : " << _value << std::endl;
		for (std::pair< T, Tries< T, U >* > elt : _childs)
		{
			v.push_back(elt.first);
			elt.second->_print(v);
			v.pop_back();
		}
	}

public:

	Tries() : _value(U()), _weight(0), _valueReadable(false) {}

	~Tries()
	{
		for (typename std::map< T, Tries< T, U>* >::iterator it = _childs.begin(); it != _childs.end(); ++it)
		{
			_alloc.destroy(Alloc, it->second);
			_alloc.deallocate(Alloc, it->second, 1);
			it->second = 0;
		}
	}

	template <class container>
	void add(const container& c, U v)
	{
		_addRecu(c, v, c.begin(), true);
	}

	const Tries< T, U >* browse(const T& c) const
	{
		typename std::map< T, Tries< T, U >* >::const_iterator find = _childs.find(c);
		if (find != _childs.end())
			return find->second;
		return 0;
	}

	U& operator[](const char* str)
	{
		std::string cont(str);
		return _addRecu(cont, U(), cont.begin(), false);
	}

	U operator[](const char* str) const
	{
		std::string cont(str);
		return _search(cont, cont.begin()).first;
	}

	template <class container>
	U& operator[](const container& c)
	{
		return _addRecu(c, U(), c.begin(), false);
	}

	template <class container>
	U operator[](const container& c) const
	{
		return _search(c, c.begin()).first;
	}

	U getValue() const
	{
		return _value;
	}

	bool valueReadable() const
	{
		return _valueReadable;
	}

	void print() const
	{
		_print(std::vector< T >());
	}
};

namespace experiment
{

    /**
     * @brief This class build a 32 sized array of bits to represents an unsigned int.
     * 
     */
    class MyNatural
    {
    protected:

        bool		_n[32];
        const int	_bitSize = 32;

    public:

        MyNatural();
        MyNatural(unsigned int n);
        MyNatural(const MyNatural &other);
        MyNatural	&operator=(const MyNatural &other);

        //GETTER

        const bool	*getData() const;
        int	getSize() const;
        unsigned int	getUnsignedInt() const;

        //OPERATORS OVERLOADS

        MyNatural	&operator+=(const MyNatural &rhs);

    };

    std::ostream	&operator<<(std::ostream &o, const MyNatural &n);

}

#endif
