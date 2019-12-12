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
	matrix(size_t row_nb, size_t col_nb);
	matrix(size_t row_nb, size_t col_nb, complex const & value);
	matrix(std::vector<std::vector<double>> const & values);
	matrix(std::vector<std::vector<complex>> && mt);
	~matrix() = default;

	// Getters
	size_t row_nb() const;
	size_t col_nb() const;
	std::vector<std::vector<complex>> const & values() const;
	std::string str() const;

	// Checkers
	bool is_empty() const;
	bool is_same_dimension(matrix const & rhs) const;
};

// Term-to-term Operations
matrix operator+(matrix const & lhs, matrix const & rhs);
matrix operator-(matrix const & lhs, matrix const & rhs);
matrix operator*(matrix const & lhs, matrix const & rhs);
matrix operator/(matrix const & lhs, matrix const & rhs);
matrix operator%(matrix const & lhs, matrix const & rhs);

// Scalar Operations
matrix operator+(matrix const & lhs, complex const & rhs);
matrix operator-(matrix const & lhs, complex const & rhs);
matrix operator*(matrix const & lhs, complex const & rhs);
matrix operator/(matrix const & lhs, complex const & rhs);
matrix operator%(matrix const & lhs, complex const & rhs);
matrix operator-(matrix const & rhs);

matrix operator+(complex const & lhs, matrix const & rhs);
matrix operator-(complex const & lhs, matrix const & rhs);
matrix operator*(complex const & lhs, matrix const & rhs);
matrix operator/(complex const & lhs, matrix const & rhs);
matrix operator%(complex const & lhs, matrix const & rhs);

// Matrix Multiplication
matrix mt_mul(matrix const & lhs, matrix const & rhs);

// Printing 
std::ostream & operator<<(std::ostream & os, matrix const & rhs);

}