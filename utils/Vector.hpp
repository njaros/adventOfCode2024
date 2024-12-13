#ifndef VEC
#define VEC

#include <vector>
#include <iostream>
#include <initializer_list>
#include <cmath>
#include <type_traits>
#include "Matrix.hpp"
#include "helpers.hpp"

template<class T>
bool nearZero(const T &val)
{
    if (val <= 0.00001 && val >= -0.00001)
        return true;
    return false;
}

template<class T>
class Matrix;

template<class T = double>
class Vector {

public :

    typedef T value_type;
    typedef std::initializer_list<value_type> inilist;
    typedef std::vector<value_type> container;
    typedef typename container::size_type size_type;
    typedef typename container::iterator iterator;
    typedef typename container::const_iterator const_iterator;

protected :

    friend class Matrix<T>;

    container _components;

    size_type	_firstNonZero()
    {
        size_type	idx = 0;
        while (!_components[idx] && idx < size())
            ++idx;
        return idx;
    }

public :

    Vector() {}

    Vector(size_type n, const value_type &val = value_type()) : _components(n, val) {};

    template <class K>
    Vector(const std::initializer_list<K> &il)
    {
        for (typename std::initializer_list<K>::const_iterator cit = il.begin(); cit != il.end(); ++cit)
                _components.push_back(static_cast<T>(*cit));
    }

    Vector(const Vector& other) : _components(other._components) {}

    template< class K >
    Vector(const Vector<K> &other,
            typename std::enable_if<!std::is_same<T, K>::value>::type* = 0)
    {
        for (typename Vector<K>::const_iterator cit = other.begin(); cit != other.end(); ++cit)
            _components.push_back(static_cast<T>(*cit));
    }

    ~Vector() {}

    Vector &operator=(const Vector &other) {
        if (&other != this)
            _components = other._components;
        return (*this);
    }

    //GETTER, DISPLAY

    std::vector<value_type> &getVector() { return _components;}
    const std::vector<value_type> &getVector() const { return _components;}

    value_type	&operator[](size_type n) 				{return _components[n];}
    const value_type &operator[](size_type n)	const	{return _components[n];}

    void displayVector() const {
        std::cout << '(';
        for (const_iterator cit = _components.begin(); cit != _components.end(); ++cit)
        {
            if (nearZero(*cit))
                std::cout << 0;
            else
                std::cout << *cit;
            if (cit != _components.end() - 1)
                std::cout << ", ";
        }
        std::cout << ')';
    }

    void displayVectorLn() const {
        std::cout << '(';
        for (const_iterator cit = _components.begin(); cit != _components.end(); ++cit)
        {
            if (nearZero(*cit))
                std::cout << 0;
            else
                std::cout << *cit;
            if (cit != _components.end() - 1)
                std::cout << ", ";
        }
        std::cout << ")\n";
    }

    //UTILITY

    size_type size() const { return _components.size(); }

    iterator	begin() {return _components.begin();}
    const_iterator	begin() const {return _components.begin();}

    iterator	end() {return _components.end();}
    const_iterator	end() const {return _components.end();}

    void	swap(Vector &other)
    {
        if (&other != this)
            _components.swap(other._components);
    }

    //OPERATOR OVERLOADS

    Vector &operator+=(const Vector &rhs) {
        if (_components.empty())
            *this = rhs;
        else if (rhs.size() == size()) {
            size_type idx = 0;
            for (iterator it = _components.begin(); it != _components.end(); ++it)
                *it += rhs._components[idx++];
        }
        return (*this);
    }

    Vector &operator-=(const Vector &rhs) {
        if (_components.empty())
            *this = rhs * -1;
        else if (rhs.size() == size()) {
            size_type idx = 0;
            for (iterator it = _components.begin(); it != _components.end(); ++it)
                *it -= rhs._components[idx++];
        }
        return (*this);
    }

    Vector operator+(const Vector &rhs) const {
        Vector salut(*this);
        return (salut += rhs);
    }

    Vector operator-(const Vector &rhs) const {
        Vector salut(*this);
        return (salut -= rhs);
    }

    template<class K>
    Vector &operator*=(const K &scalar) {
        for (iterator it = _components.begin(); it != _components.end(); ++it)
            *it *= scalar;
        return (*this);
    }

    template<class K>
    Vector operator*(const K &scalar) const {
        Vector du(*this);
        return (du *= scalar);
    }

    template<class K>
    Vector &operator/=(const K &scalar) {
        for (iterator it = _components.begin(); it != _components.end(); ++it)
            *it /= scalar;
        return (*this);
    }

    template<class K>
    Vector operator/(const K &scalar) const {
        Vector du(*this);
        return (du /= scalar);
    }

    Vector	&operator*=(const Vector &rhs)
    {
        if (rhs.size() == size())
        {
            const_iterator rhsIt = rhs.begin();
            for (iterator lhsIt = begin(); lhsIt != end(); ++lhsIt)
                *lhsIt += *rhsIt++;
        }
        return *this;
    }

    Vector	operator*(const Matrix<T> &m) const
    {
        if (size() != m.width())
            return Vector();
        Vector	v(m.high());
        for (size_type i = 0; i < v.size(); ++i)
        {
            for (typename Matrix<T>::row_size_type j = 0; j < m.width(); ++j)
                v[i] += m[i][j] * _components[j];
        }
        return v;
    }

    Vector	operator*(const Vector &rhs) const
    {
        Vector	ino(*this);
        return (ino *= rhs);
    }

    //PROPER VECTOR OPERATORS

    //Given a vector a{a1, a2, ..., an-1, an},
    //Sum return sum = a1 + a2 + ... + an-1, an.
    value_type	sum() const
    {
        value_type sum = value_type();
        for (const_iterator cit = begin(); cit != end(); ++cit)
            sum += *cit;
        return sum;
    }

    //Given 2 vectors a{a1, a2, ..., an-1, an} and b{b1, b2, ..., bn-1, bn},
    //dot product k = <a|b> = a.b = a1 * b1 + a2 * b2 + ... + an-1 * bn-1 + an * bn.
    //dot product returns a value_type and not a vector.
    template <class K = T>
    value_type  dot(const Vector<K> &rhs)	const
    {
        if (rhs.size() != size())
        {
            std::cerr << "Warning: Vector::dot: vectors must belong to the same vector space (= must have same size).\n";
            return value_type();
        }
        value_type	dotted = value_type();
        const_iterator	rhsIt = rhs.begin();
        for (const_iterator lhsIt = begin(); lhsIt != end(); ++lhsIt)
        {
            dotted += *rhsIt * *lhsIt;
            ++rhsIt;
        }
        return dotted;
    }

    //Given a vector a{a1, a2, ..., an-1, an}.
    //The Manhattan norm of a = ||a||1 = |a1| + |a2| + ... + |an-1| + |an|.
    //Where |x| = x if x >=0 and |x| = -x if x < 0.
    value_type	normManhattan()	const
    {
        value_type	norm = value_type();
        for (const_iterator cit = begin(); cit != end(); ++cit)
            norm += std::abs(*cit);
        return norm;
    }

    //Given a vector a{a1, a2, ..., an-1, an}.
    //The Square norm of a = a1^2 + a2^2 + ... + an-1^2 + an^2.
    //This is only useful to compare distance using lower resources than Euclidean norm.
    value_type	normSquare() const
    {
        value_type	norm = value_type();
        for (const_iterator cit = begin(); cit != end(); ++cit)
            norm += pow(*cit, 2.0);
        return norm;
    }

    //Given a vector a{a1, a2, ..., an-1, an}.
    //The Euclidean norm of a = ||a|| = ||a||2 = sqrt(a1^2 + a2^2 + ... + an-1^2 + an^2).
    //Beware: this operation is painful for computers, avoid it as soon as possible.
    double	normEuclidean()	const
    {
        return std::sqrt(normSquare());
    }

    //Given a vector a{a1, a2, ..., an-1, an}.
    //The infinite norm of a = ||a||∞ = max(i in range 1, n)|ai|.
    //Where |ai| = ai if ai >=0 and |ai| = -ai if ai < 0.
    value_type	normInfinite() const
    {
        value_type	norm = value_type();
        for (const_iterator cit = begin(); cit != end(); ++cit)
        {
            value_type	current = std::abs(*cit);
            if (current > norm)
                norm = current;
        }
        return norm;
    }

    //Given 2 vectors v1 and v2
    //This return cos(a) where 'a' is the radian angle between v1 and v2
    double	cos(const Vector &other) const
    {
        return (dot(other) / sqrt(normSquare() * other.normSquare()));
    }

};

template<class T, class K>
Vector<T> operator*(const K &scalar, const Vector<T> &rhs) {
    return (rhs * scalar);
}

template<class T, class K>
Vector<T> operator/(const K &scalar, const Vector<T> &rhs) {
    return (rhs / scalar);
}

template<class T>
std::ostream &operator<<(std::ostream &o, const Vector<T> &v)
{
    o << '(';
    for (typename Vector<T>::const_iterator cit = v.begin(); cit != v.end(); ++cit)
    {
        if (nearZero(*cit))
            o << 0;
        else
            o << *cit;
        if (cit != v.end() - 1)
            o << ", ";
    }
    o << ')';
    return o;
}

//Given 2 vectors a{a1, a2, ..., an-1, an} and b{b1, b2, ..., bn-1, bn},
//dot product k = <a|b> = a.b = a1 * b1 + a2 * b2 + ... + an-1 * bn-1 + an * bn.
//dot product returns a value_type and not a vector.
template<class T> T	dot(const Vector<T> &lhs, const Vector<T> &rhs)
{
    return lhs.dot(rhs);
}

//Given 2 vectors v1 and v2
//This return cos(a) where 'a' is the radian angle between v1 and v2
template<class T> T	cos(const Vector<T> &lhs, const Vector<T> &rhs)
{
    return lhs.cos(rhs);
}

template<class T> void swap(Vector<T> &lhs, Vector<T> &rhs)
{
    lhs.swap(rhs);
}

#endif