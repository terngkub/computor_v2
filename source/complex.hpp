#pragma once
#include <ostream>

namespace computorv2
{

struct complex
{
	double rational;
	double imaginary;

	complex();
	complex(double rational, double imaginary);
	~complex() = default;
	complex(complex const &) = default;
	complex(complex &&) = default;
	complex & operator=(complex const &) = default;
	complex & operator=(complex &&) = default;

	bool is_zero() const;
	bool is_complex() const;

	complex & operator+(complex const & rhs);
	complex & operator-(complex const & rhs);
	complex & operator*(complex const & rhs);
	complex & operator/(complex const & rhs);
	complex & operator%(complex const & rhs);
};

std::ostream & operator<<(std::ostream & os, complex const & rhs);

}