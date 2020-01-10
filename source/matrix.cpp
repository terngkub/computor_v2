#include "matrix.hpp"
#include <functional>
#include <sstream>

namespace computorv2
{

// Constructors

matrix::matrix()
	: _row_nb{0}
	, _col_nb{0}
	, _values{}
{}

matrix::matrix(size_t row_nb, size_t col_nb)
	: _row_nb{row_nb}
	, _col_nb{col_nb}
	, _values(row_nb, std::vector<complex>(col_nb, complex{}))
{}

matrix::matrix(size_t row_nb, size_t col_nb, complex const & value)
	: _row_nb{row_nb}
	, _col_nb{col_nb}
	, _values(row_nb, std::vector<complex>(col_nb, complex{value}))
{}

// MATRIX fix this
matrix::matrix(std::vector<std::vector<complex>> const & complex_matrix)
	: _row_nb{complex_matrix.size()}
	, _col_nb{_row_nb > 0 ? complex_matrix[0].size() : 0}
	, _values{complex_matrix}
{
	for (auto const & row : complex_matrix)
	{
		if (row.size() != _col_nb)
			throw std::runtime_error("matrix doesn't contain the same column numbers for each rows");
	}
}

matrix::matrix(std::vector<std::vector<complex>> && mt)
	: _row_nb{mt.size()}
	, _col_nb{_row_nb > 0 ? mt[0].size() : 0}
	, _values{mt}
{
	for (auto const & row : mt)
	{
		if (row.size() != _col_nb)
			throw std::runtime_error("matrix doesn't contain the same column numbers for each rows");
	}
}


// Getters

size_t matrix::row_nb() const
{
	return _row_nb;
}

size_t matrix::col_nb() const
{
	return _col_nb;
}

std::vector<std::vector<complex>> const & matrix::values() const
{
	return _values;
}

std::string matrix::str() const
{
	std::stringstream ss;

	ss << '[';

	for (auto rit = _values.cbegin(); rit < _values.cend(); ++rit)
	{
		ss << '[';

		for (auto cit = rit->cbegin(); cit < rit->cend(); ++cit)
		{
			ss << *cit;

			if (cit != rit->cend() - 1)
				ss << ", ";
		}

		ss << ']';

		if (rit != _values.cend() - 1)
			ss << "; ";
	}

	ss << ']';

	return ss.str();
}


// Checkers

bool matrix::is_empty() const
{
	return (_row_nb == 0 || _col_nb == 0);
}

bool matrix::is_same_dimension(matrix const & rhs) const
{
	return _row_nb == rhs._row_nb || _col_nb == rhs._col_nb;
}


// Term-to-term Operations

static matrix term_to_term_operation(matrix const & lhs, matrix const & rhs, std::function<complex (complex const &, complex const &)> op_func)
{
	if (!lhs.is_same_dimension(rhs))
		throw std::runtime_error("matrix operation with different dimensions");

	std::vector<std::vector<complex>> ret(lhs.row_nb(), std::vector<complex>(lhs.col_nb()));

	for (size_t i = 0; i < lhs.row_nb(); ++i)
	{
		for (size_t j = 0; j < lhs.col_nb(); ++j)
		{
			ret[i][j] = op_func(lhs.values()[i][j], rhs.values()[i][j]);
		}
	}

	return matrix{std::move(ret)};
}

matrix operator+(matrix const & lhs, matrix const & rhs)
{
	return term_to_term_operation(lhs, rhs, [](complex const & lhs, complex const & rhs){ return lhs + rhs; });
}

matrix operator-(matrix const & lhs, matrix const & rhs)
{
	return term_to_term_operation(lhs, rhs, [](complex const & lhs, complex const & rhs){ return lhs - rhs; });
}

matrix operator*(matrix const & lhs, matrix const & rhs)
{
	return term_to_term_operation(lhs, rhs, [](complex const & lhs, complex const & rhs){ return lhs * rhs; });
}

matrix operator/(matrix const & lhs, matrix const & rhs)
{
	return term_to_term_operation(lhs, rhs, [](complex const & lhs, complex const & rhs){ return lhs / rhs; });
}

matrix operator%(matrix const & lhs, matrix const & rhs)
{
	return term_to_term_operation(lhs, rhs, [](complex const & lhs, complex const & rhs){ return lhs % rhs; });
}

// Scalar Operations

static matrix scalar_operation(matrix const & lhs, complex const & rhs, std::function<complex (complex const &, complex const &)> op_func)
{
	std::vector<std::vector<complex>> ret(lhs.row_nb(), std::vector<complex>(lhs.col_nb()));

	for (size_t i = 0; i < lhs.row_nb(); ++i)
	{
		for (size_t j = 0; j < lhs.col_nb(); ++j)
		{
			ret[i][j] = op_func(lhs.values()[i][j], rhs);
		}
	}

	return matrix{std::move(ret)};
}

matrix operator+(matrix const & lhs, complex const & rhs)
{
	return scalar_operation(lhs, rhs, [](complex const & a, complex const & b){ return a + b; });
}

matrix operator-(matrix const & lhs, complex const & rhs)
{
	return scalar_operation(lhs, rhs, [](complex const & a, complex const & b){ return a - b; });
}

matrix operator*(matrix const & lhs, complex const & rhs)
{
	return scalar_operation(lhs, rhs, [](complex const & a, complex const & b){ return a * b; });
}

matrix operator/(matrix const & lhs, complex const & rhs)
{
	return scalar_operation(lhs, rhs, [](complex const & a, complex const & b){ return a / b; });
}

matrix operator%(matrix const & lhs, complex const & rhs)
{
	return scalar_operation(lhs, rhs, [](complex const & a, complex const & b){ return a % b; });
}

matrix operator-(matrix const & rhs)
{
	return rhs * -1;
}

matrix operator+(complex const & lhs, matrix const & rhs)
{
	return rhs + lhs;
}

matrix operator-(complex const & lhs, matrix const & rhs)
{
	return matrix(rhs.row_nb(), rhs.col_nb(), lhs) - rhs;
}

matrix operator*(complex const & lhs, matrix const & rhs)
{
	return rhs * lhs;
}

matrix operator/(complex const & lhs, matrix const & rhs)
{
	return matrix(rhs.row_nb(), rhs.col_nb(), lhs) / rhs;
}

matrix operator%(complex const & lhs, matrix const & rhs)
{
	return matrix(rhs.row_nb(), rhs.col_nb(), lhs) % rhs;
}


// Matrix Multiplication

matrix mt_mul(matrix const & lhs, matrix const & rhs)
{
	if (lhs.col_nb() != rhs.row_nb())
		throw std::runtime_error("matrix multiplication: left column number and right row number aren't the same");

	std::vector<std::vector<complex>> ret(lhs.row_nb(), std::vector<complex>(rhs.col_nb()));

	for (size_t lr = 0; lr < lhs.row_nb(); ++lr)
	{
		for (size_t rc = 0; rc < rhs.col_nb(); ++rc)
		{
			for (size_t i = 0; i < lhs.col_nb(); ++i)
			{
				ret[lr][rc] = ret[lr][rc] + lhs.values()[lr][i] * rhs.values()[i][rc];
			}
		}
	}

	return matrix{std::move(ret)};
}


// Other Operations

bool operator==(matrix const & lhs, matrix const & rhs)
{
	if (lhs.row_nb() != rhs.row_nb() || lhs.col_nb() != rhs.col_nb())
		return false;

	for (size_t r = 0; r < lhs.row_nb(); ++r)
	{
		for (size_t c = 0; c < lhs.col_nb(); ++c)
		{
			if (lhs.values()[r][c] != rhs.values()[r][c])
				return false;
		}
	}

	return true;
}

bool operator!=(matrix const & lhs, matrix const & rhs)
{
	return !(lhs == rhs);
}


std::ostream & operator<<(std::ostream & os, matrix const & rhs)
{
	os << rhs.str();
	return os;
}

} // namespace computorv2