#include "matrix.hpp"

namespace computorv2
{

Matrix::Matrix()
	: row_nb{0}
	, col_nb{0}
	, values{}
{}

Matrix::Matrix(std::vector<std::vector<double>> const & values)
	: row_nb{values.size()}
	, col_nb{row_nb > 0 ? values[0].size() : 0}
	, values{values}
{
	for (auto const & row : values)
	{
		if (row.size() != col_nb)
			throw std::runtime_error("Matrix doesn't contain the same column numbers for each rows");
	}
}

Matrix::Matrix(size_t row_nb, size_t col_nb)
	: row_nb{row_nb}
	, col_nb{col_nb}
	, values(row_nb, std::vector<double>(col_nb, 0))
{}

bool Matrix::empty() const
{
	return (row_nb == 0 || col_nb == 0);
}

Matrix Matrix::operator+(Matrix const & rhs) const
{
	if (row_nb != rhs.row_nb || col_nb != rhs.col_nb)
		throw std::runtime_error("Matrix addition: left and right don't have the same dimension");

	Matrix ret{*this};

	for (auto i = 0; i < row_nb; ++i)
	{
		for (auto j = 0; j < col_nb; ++j)
		{
			ret.values[i][j] += rhs.values[i][j];
		}
	}

	return ret;
}

Matrix Matrix::operator-(Matrix const & rhs) const
{
	if (row_nb != rhs.row_nb || col_nb != rhs.col_nb)
		throw std::runtime_error("Matrix subtraction: left and right don't have the same dimension");

	Matrix ret{*this};

	for (auto i = 0; i < row_nb; ++i)
	{
		for (auto j = 0; j < col_nb; ++j)
		{
			ret.values[i][j] -= rhs.values[i][j];
		}
	}

	return ret;
}

Matrix Matrix::multiply(Matrix const & rhs) const
{
	if (col_nb != rhs.row_nb)
		throw std::runtime_error("Matrix multiplication: left column number and right row number aren't the same");

	Matrix ret{row_nb, rhs.col_nb};

	for (auto lr = 0; lr < row_nb; ++lr)
	{
		for (auto rc = 0; rc < rhs.col_nb; ++rc)
		{
			for (auto i = 0; i < col_nb; ++i)
			{
				ret.values[lr][rc] += values[lr][i] * rhs.values[i][rc];
			}
		}
	}

	return ret;
}

std::ostream & operator<<(std::ostream & os, Matrix const & rhs)
{
	for (auto rit = rhs.values.cbegin(); rit != rhs.values.cend(); ++rit)
	{
		os << "| ";

		auto last = rit->cend() - 1;

		for (auto cit = rit->cbegin(); cit != rit->cend(); ++cit)
		{
			os << *cit;
			if (cit != last)
				os << ", ";
		}
		os << " |\n";
	}

	return os;
}

}