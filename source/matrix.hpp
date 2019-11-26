#pragma once
#include "complex.hpp"
#include <ostream>
#include <vector>

namespace computorv2
{

struct matrix
{
	size_t row_nb;
	size_t col_nb;
	std::vector<std::vector<complex>> values;

	// Constructors
	matrix();
	matrix(std::vector<std::vector<double>> const & values);
	matrix(size_t row_nb, size_t col_nb);
	~matrix() = default;

	// Checker
	bool empty() const;

	// Matrix operation
	matrix matrix_add(matrix const & rhs) const;
	matrix matrix_sub(matrix const & rhs) const;
	matrix matrix_mul(matrix const & rhs) const;

	// Scalar operation
	matrix scalar_mul(complex nb) const;
	matrix scalar_div(complex nb) const;
};

// Printing
std::ostream & operator<<(std::ostream & os, matrix const & rhs);

}