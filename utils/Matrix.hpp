#ifndef MAT
#define MAT

#include "Vector.hpp"

template<class T>
class Vector;

template<class T = double>
class Matrix {

public:

    typedef T value_type;
    typedef Vector<value_type> row;
    typedef std::initializer_list<std::initializer_list<T> > inilist;
    typedef Vector<row> matrix;
    typedef typename std::initializer_list<value_type>::size_type	width_size;
    typedef typename inilist::size_type	high_size;
    typedef typename row::size_type row_size_type;
    typedef typename matrix::size_type matrix_size_type;
    typedef typename matrix::iterator	iterator;
    typedef typename matrix::const_iterator	const_iterator;

protected :

    matrix				_mat;
    row_size_type		_width;
    matrix_size_type	_high;
    bool				_fail;

    Matrix(bool fail) : _width(0), _high(0), _fail(fail) {}

    void	_killUpDown(matrix_size_type killer, row_size_type nonZero)
    {
        for (matrix_size_type idx = 0; idx < _high; ++idx)
        {
            if (idx != killer)
            {
                _mat[idx] -= (_mat[idx][nonZero] * _mat[killer]);
            }
        }
    }

    void	_killDownTriangle(matrix_size_type killer, row_size_type nonZero)
    {
        for (matrix_size_type idx = killer + 1; idx < _high; ++idx)
            if (idx != killer)
                _mat[idx] -= (_mat[idx][nonZero] / _mat[killer][nonZero]) * _mat[killer];
    }

    void	_killUpDownInverse(matrix_size_type killer, row_size_type nonZero, Matrix &inverse)
    {
        for (matrix_size_type idx = 0; idx < _high; ++idx)
        {
            value_type	mlt = _mat[idx][nonZero];
            if (idx != killer)
            {
                _mat[idx] -= mlt * _mat[killer];
                inverse._mat[idx] -= mlt * inverse._mat[killer];
            }
        }
    }

    Matrix	&_buildInverse(Matrix &inverse)
    {
        for (matrix_size_type idx = 0; idx != _high; ++idx)
        {
            row_size_type	nonZero = _mat[idx]._firstNonZero();
            matrix_size_type	toSwap = idx;
            matrix_size_type	current = idx + 1;
            while (current < _high)
            {
                row_size_type	currentNonZero = _mat[current]._firstNonZero();
                if (currentNonZero < nonZero)
                {
                    toSwap = current;
                    nonZero = currentNonZero;
                }
                ++current;
            }
            if (nonZero == _width)
            {
                inverse = Matrix(true);
                break;
            }
            _mat[idx].swap(_mat[toSwap]);
            inverse._mat[idx].swap(inverse._mat[toSwap]);
            value_type	divide = _mat[idx][nonZero];
            _mat[idx] /= divide;
            inverse._mat[idx] /= divide;
            _killUpDownInverse(idx, nonZero, inverse);
        }
        return *this;
    }

public:

    Matrix() : _width(0), _high(0), _fail(false) {}

    Matrix(matrix_size_type n, row_size_type m, const value_type &val = value_type()) : _mat(n) , _width(m), _high(n), _fail(false)
    {
        for (iterator it = _mat.begin(); it != _mat.end(); ++it)
            *it = Vector<value_type>(m, val);
    }

    template< class K >
    Matrix(const std::initializer_list<std::initializer_list<K> > &il) : _high(0), _fail(false)
    {
        typedef typename std::initializer_list<std::initializer_list<K> > Kinilist;
        for (typename Kinilist::const_iterator cit = il.begin(); cit != il.end(); ++cit)
        {
            ++_high;
            if (cit == il.begin())
                _width = cit->size();
            else if (cit->size() != _width)
            {
                _mat.getVector().clear();
                _width = 0;
                _high = 0;
                _fail = true;
                return;
            }
            _mat.getVector().push_back(row(*cit));
        }
    }

    template< class K >
    Matrix(const Matrix<K> &other,
        typename std::enable_if<!std::is_same<T, K>::value, void>::type* = 0)
            : _mat(other.high()), _width(other.width()), _high(other.high()), _fail(other.fail())
    {
        iterator start = begin();
        for (typename Matrix<K>::const_iterator cit = other.begin(); cit != other.end(); ++cit)
        {
            *start = Vector(*cit);
            ++start;
        }
    }

    Matrix(const Matrix &o) : _mat(o._mat), _width(o._width), _high(o._high), _fail(o._fail) {}

    ~Matrix() {}

    Matrix &operator=(const Matrix &o) {	_mat = o._mat; _width = o._width;
                                            _high = o._high; _fail = o._fail;
                                            return *this;}

    //GETTER, DISPLAYS

    matrix&	getMatrix() {return _mat;}
    const matrix&	getMatrix()	const {return _mat;}

    void displayMatrix() const {
        if (_fail)
        {
            std::cout << "This matrix was impossible to build";
            return;
        }
        for (const_iterator cit = _mat.begin(); cit != _mat.end(); ++cit)
        {
            if (cit != _mat.end() - 1)
                cit->displayVectorLn();
            else
                cit->displayVector();
        }
    }

    void displayMatrixLn() const {
        if (_fail)
        {
            std::cout << "This matrix was impossible to build\n";
            return;
        }
        for (const_iterator cit = _mat.begin(); cit != _mat.end(); ++cit)
            cit->displayVectorLn();
        std::cout << '\n';
    }

    Vector<value_type>			&operator[](matrix_size_type n)			{return _mat[n];}
    const Vector<value_type>	&operator[](matrix_size_type n)	const	{return _mat[n];}

    //UTILITY

    bool				fail()	const	{return _fail;}
    bool				good()	const	{return !_fail;}
    bool				empty()	const	{return _mat.getVector().empty();}
    row_size_type		width()	const	{return _width;}
    matrix_size_type	high()	const	{return _high;}
    iterator 			begin()			{return _mat.begin();}
    const_iterator 		begin()	const	{return _mat.begin();}
    iterator 			end()			{return _mat.end();}
    const_iterator 		end()	const	{return _mat.end();}

    //OPERATORS

    Matrix	&operator+=(const Matrix &rhs)
    {
        if (fail() || rhs.fail())
            return (*this);
        if (_mat.getVector().empty())
            *this = rhs;
        else if (_width == rhs._width && _high == rhs._high)
        {
            const_iterator cit = rhs.begin();
            for (iterator it = _mat.begin(); it != _mat.end(); ++it)
            {
                *it += *cit;
                ++cit;
            }
        }
        return *this;
    }

    Matrix	&operator-=(const Matrix &rhs)
    {
        if (fail() || rhs.fail())
            return (*this);
        if (_mat.getVector().empty())
            *this = rhs * -1;
        if (_width == rhs._width && _high == rhs._high)
        {
            const_iterator cit = rhs.begin();
            for (iterator it = _mat.begin(); it != _mat.end(); ++it) {
                *it -= *cit;
                ++cit;
            }
        }
        return *this;
    }

    Matrix	operator+(const Matrix &rhs) const
    {
        if (fail() || rhs.fail() || (!empty() && !(rhs._high == _high && rhs._width == _width)))
            return (Matrix(true));
        Matrix	m(*this);
        return (m += rhs);
    }

    Matrix	operator-(const Matrix &rhs) const
    {
        if (fail() || rhs.fail() || (!empty() && !(rhs._high == _high && rhs._width == _width)))
            return (Matrix(true));
        Matrix	m(*this);
        return (m -= rhs);
    }

    template<class K> Matrix	&operator*=(const K &scalar)
    {
        if (fail())
            return (*this);
        for (iterator it = _mat.begin(); it != _mat.end(); ++it)
            *it *= scalar;
        return (*this);
    }

    Matrix	operator*(const Matrix &rhs) const
    {
        if (fail() || rhs.fail() || _width != rhs._high)
            return Matrix(true);
        Matrix	m(_high, rhs._width);
        for (matrix_size_type i = 0; i < m._high; ++i)
        {
            for (row_size_type j = 0; j < m._width; ++j)
            {
                matrix_size_type l = 0;
                for (row_size_type k = 0; k < _width; ++k)
                {
                    m[i][j] += _mat[i][k] * rhs[l][j];
                    ++l;
                }
            }
        }
        return m;
    }

    Vector<value_type>	operator*(const Vector<value_type> &v) const
    {
        if (fail())
            return Vector<value_type>();
        return (v * *this);
    }

    template<class K> Matrix	operator*(const K &scalar)	const
    {
        if (fail())
            return Matrix(true);
        Matrix	m(*this);
        return (m *= scalar);
    }

    //PROPER MATRIX OPERATORS

    //Given a square matrix m belong to K^(n*n),
    //trace(n) is the sum of each component of the diagonal of m.
    value_type	trace()	const
    {
        value_type	result = value_type();
        if (_width != _high)
        {
            std::cerr << "Warning: Matrix::trace: this isn't a square matrix.\n";
            return result;
        }
        for (matrix_size_type idx = 0; idx < _high; ++idx)
            result += _mat[idx][idx];
        return result;
    }

    //Given a matrix M belong to n*m
    //The transpose is the matrix tM belong to m*n which is
    //created by symmetry with the diagonal of M
    Matrix	transpose() const
    {
        Matrix	transposed(_width, _high);
        for (matrix_size_type idxH = 0; idxH < transposed._high; ++idxH)
        {
            for (row_size_type idxW = 0; idxW < transposed._width; ++idxW)
                transposed[idxH][idxW] = _mat[idxW][idxH];
        }
        return transposed;
    }

    //The row echelon form is used to build the inverted matrix of M
    //Beware: this function will transform your current object and
    //not create a new one !
    Matrix	&rowEchelonSelf()
    {
        for (matrix_size_type idx = 0; idx != _high; ++idx)
        {
            if (idx >= _width) {
                while (idx < _high)
                    _mat[idx++] = Vector<value_type>(_width);
                break;
            }
            row_size_type	nonZero = _mat[idx]._firstNonZero();
            matrix_size_type	toSwap = idx;
            matrix_size_type	current = idx + 1;
            while (current < _high)
            {
                row_size_type	currentNonZero = _mat[current]._firstNonZero();
                if (currentNonZero < nonZero)
                {
                    toSwap = current;
                    nonZero = currentNonZero;
                }
                ++current;
            }
            if (nonZero == _width)
                break;
            _mat[idx].swap(_mat[toSwap]);
            value_type	divide = _mat[idx][nonZero];
            _mat[idx] /= divide;
            _killUpDown(idx, nonZero);
        }
        return *this;
    }

    //The row echelon form is used to build the inverted matrix of M
    Matrix	rowEchelon() const
    {
        Matrix	result(*this);
        return (result.rowEchelonSelf());
    }

    //Given a Matrix, his triangular form is a matrix constructed
    //with Gauss transmutation to obtain a matrix with only 0
    //in his bottom left triangle, diagonal not included.
    //Beware: this function will transform your current object and
    //not create a new one !
    Matrix	&triangularSelf()
    {
        for (matrix_size_type idx = 0; idx != _high; ++idx)
        {
            if (idx >= _width) {
                while (idx < _high)
                    _mat[idx++] = Vector<value_type>(_width);
                break;
            }
            row_size_type	nonZero = _mat[idx]._firstNonZero();
            matrix_size_type	toSwap = idx;
            matrix_size_type	current = idx + 1;
            while (current < _high)
            {
                row_size_type	currentNonZero = _mat[current]._firstNonZero();
                if (currentNonZero < nonZero)
                {
                    toSwap = current;
                    nonZero = currentNonZero;
                }
                ++current;
            }
            if (nonZero == _width)
                break;
            _mat[idx].swap(_mat[toSwap]);
            if (idx != toSwap)
                _mat[idx] *= -1;
            _killDownTriangle(idx, nonZero);
        }

        return *this;
    }

    //Given a Matrix, his triangular form is a matrix constructed
    //with Gauss transmutation to obtain a matrix with only 0
    //in his bottom left triangle, diagonal not included.
    Matrix	triangular() const
    {
        Matrix	triangle(*this);
        return (triangle.triangularSelf());
    }

    //The determinant of a square matrix M is the multiplication
    //of all the coefficients in the diagonal of the
    //triangular matrix form of M.
    value_type	determinant() const
    {
        value_type	result = value_type();
        int sign;
        if (_width != _high)
            return result;
        Matrix	ech = triangular();
        for (matrix_size_type idx = 0; idx < _high; ++idx)
        {
            if (!idx)
                result = ech._mat[idx][idx];
            else
                result *= ech._mat[idx][idx];
        }
        if (result == -0)
            result = 0;
        return result;
    }

    //If the given matrix M is a square matrix n*n and have a non-null
    //determinant, his inverse is the matrix M^-1 which has those properties :
    //M * M^-1 = Idn
    //M^-1 * M = Idn
    //Where Idn is the square matrix n*n with only non-null coefficients
    //are 1 in his diagonal.
    Matrix	inverse() const
    {
        if (_width != _high)
            return Matrix(true);
        Matrix	inverse(_high, _width);
        Matrix	tool(*this);
        for (matrix_size_type idx = 0; idx < inverse._high; ++idx)
            inverse[idx][idx] = 1;
        tool._buildInverse(inverse);
        return inverse;
    }

    matrix_size_type	rank()	const
    {
        Matrix	triangle(triangular());
        matrix_size_type	idx = 0;
        while (idx < _high)
        {
            if (triangle[idx]._firstNonZero() == _width)
                return idx;
            ++idx;
        }
        return idx;
    }

};

template<class T, class K> Matrix<T>	operator*(const K &scalar, const Matrix<T> &m)
{
    return (m * scalar);
}

template<class T>
std::ostream &operator<<(std::ostream &o, const Matrix<T> &m)
{
    if (m.fail())
    {
        o << "This matrix was impossible to build\n";
        return o;
    }
    for (typename Matrix<T>::const_iterator cit = m.begin(); cit != m.end(); ++cit)
    {
        o << *cit;
        if (cit != m.end() - 1)
            o << '\n';
    }
    return o;
}

#endif