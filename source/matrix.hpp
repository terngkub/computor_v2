#pragma once
#include <ostream>
#include <vector>

namespace computorv2
{

struct Matrix
{
	size_t row_nb;
	size_t col_nb;
	std::vector<std::vector<double>> values;

	// Constructors
	Matrix();
	Matrix(std::vector<std::vector<double>> const & values);
	Matrix(size_t row_nb, size_t col_nb);
	~Matrix() = default;

	bool empty() const;

	// Operation
	Matrix operator+(Matrix const & rhs) const;
	Matrix operator-(Matrix const & rhs) const;
	Matrix multiply(Matrix const & rhs) const;
};

std::ostream & operator<<(std::ostream & os, Matrix const & rhs);

}