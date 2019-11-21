#pragma once
#include <ostream>

namespace computorv2
{

struct complex
{
	double real;
	double imag;

	// Constructors and Destructors
	complex();
	complex(double rational, double imaginary);
	~complex() = default;

	// Copy - Enable
	complex(complex const &) = default;
	complex & operator=(complex const &) = default;

	// Move - Enable
	complex(complex &&) = default;
	complex & operator=(complex &&) = default;

	// Checkers
	bool is_zero() const;
	bool is_complex() const;

	// Operations
	complex operator+(complex const & rhs) const;
	complex operator-(complex const & rhs) const;
	complex operator*(complex const & rhs) const;
	complex operator/(complex const & rhs) const;
	complex operator%(complex const & rhs) const;
	complex operator-() const;
};

std::ostream & operator<<(std::ostream & os, complex const & rhs);

}