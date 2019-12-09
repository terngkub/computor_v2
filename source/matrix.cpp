#include "matrix.hpp"
#include <sstream>

namespace computorv2
{

// Constructors

matrix::matrix()
	: _row_nb{0}
	, _col_nb{0}
	, _values{}
{}

matrix::matrix(std::vector<std::vector<double>> const & double_matrix)
	: _row_nb{double_matrix.size()}
	, _col_nb{_row_nb > 0 ? double_matrix[0].size() : 0}
	, _values{}
{
	for (auto const & row : double_matrix)
	{
		if (row.size() != _col_nb)
			throw std::runtime_error("matrix doesn't contain the same column numbers for each rows");

		_values.push_back(std::vector<complex>{});
		auto current_row = _values.end() - 1;
		for (auto const & col : row)
		{
			current_row->emplace_back(col, 0);
		}
	}
}

matrix::matrix(size_t row_nb, size_t col_nb)
	: _row_nb{row_nb}
	, _col_nb{col_nb}
	, _values(row_nb, std::vector<complex>(col_nb, complex{}))
{}


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


// Checker

bool matrix::is_empty() const
{
	return (row_nb == 0 || col_nb == 0);
}


// Matrix Operations

matrix matrix::matrix_add(matrix const & rhs) const
{
	auto const & lhs = *this;

	if (lhs.row_nb != rhs.row_nb || lhs.col_nb != rhs.col_nb)
		throw std::runtime_error("matrix addition: left and right don't have the same dimension");

	matrix ret{lhs};

	for (auto i = 0; i < lhs.row_nb; ++i)
	{
		for (auto j = 0; j < lhs.col_nb; ++j)
		{
			ret.values[i][j] = ret.values[i][j] + rhs.values[i][j];
		}
	}

	return ret;
}

matrix matrix::matrix_sub(matrix const & rhs) const
{
	auto const & lhs = *this;

	if (lhs.row_nb != rhs.row_nb || lhs.col_nb != rhs.col_nb)
		throw std::runtime_error("matrix subtraction: left and right don't have the same dimension");

	matrix ret{lhs};

	for (auto i = 0; i < lhs.row_nb; ++i)
	{
		for (auto j = 0; j < lhs.col_nb; ++j)
		{
			ret.values[i][j] = ret.values[i][j] - rhs.values[i][j];
		}
	}

	return ret;
}

matrix matrix::matrix_mul(matrix const & rhs) const
{
	auto const & lhs = *this;

	if (lhs.col_nb != rhs.row_nb)
		throw std::runtime_error("matrix multiplication: left column number and right row number aren't the same");

	matrix ret{lhs.row_nb, rhs.col_nb};

	for (auto lr = 0; lr < lhs.row_nb; ++lr)
	{
		for (auto rc = 0; rc < rhs.col_nb; ++rc)
		{
			for (auto i = 0; i < lhs.col_nb; ++i)
			{
				ret.values[lr][rc] = ret.values[lr][rc] + lhs.values[lr][i] * rhs.values[i][rc];
			}
		}
	}

	return ret;
}

matrix matrix::scalar_mul(complex nb) const
{
	matrix ret{*this};

	for (auto & row : ret.values)
	{
		for (auto & col : row)
		{
			col = col * nb;
		}
	}

	return ret;
}

matrix matrix::scalar_div(complex nb) const
{
	if (nb.is_zero())
		throw std::runtime_error("division by zero");

	matrix ret{*this};

	for (auto & row : ret.values)
	{
		for (auto & col : row)
		{
			col = col / nb;
		}
	}

	return ret;
}


// Printing

std::ostream & operator<<(std::ostream & os, matrix const & rhs)
{
	os << rhs.str();
	return os;
}

}