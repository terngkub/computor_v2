#pragma once
#include "complex.hpp"
#include <ostream>
#include <vector>

namespace computorv2
{

class matrix
{
	size_t _row_nb;
	size_t _col_nb;
	std::vector<std::vector<complex>> _values;

public:

	// Constructors
	matrix();
	matrix(std::vector<std::vector<double>> const & values);
	matrix(size_t row_nb, size_t col_nb);
	~matrix() = default;

	// Getters
	size_t row_nb() const;
	size_t col_nb() const;
	std::vector<std::vector<complex>> const & values() const;
	std::string str() const;

	// Checker
	bool is_empty() const;

	// Matrix operation
	matrix matrix_add(matrix const & rhs) const;
	matrix matrix_sub(matrix const & rhs) const;
	matrix matrix_mul(matrix const & rhs) const;

	// Scalar operation
	matrix scalar_mul(complex nb) const;
	matrix scalar_div(complex nb) const;
};

/*
m + m
m - m
m * m
m ** m
m / m
m % m

m * c
c * m
m / c

*/

// Printing
std::ostream & operator<<(std::ostream & os, matrix const & rhs);

}