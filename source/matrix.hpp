#pragma once
#include <ostream>
#include <vector>

namespace computorv2
{

struct matrix
{
	size_t row_nb;
	size_t col_nb;
	std::vector<std::vector<double>> values;

	// Constructors
	matrix();
	matrix(std::vector<std::vector<double>> const & values);
	matrix(size_t row_nb, size_t col_nb);
	~matrix() = default;

	// Operation
	matrix operator+(matrix const & rhs) const;
	matrix operator-(matrix const & rhs) const;
	matrix multiply(matrix const & rhs) const;
};

std::ostream & operator<<(std::ostream & os, matrix const & rhs);

}