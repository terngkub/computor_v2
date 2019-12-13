#pragma once
#include <ostream>

namespace computorv2
{

class complex
{
	double _real;
	double _imag;

public:

	// Constructors and Destructor
	complex();
	complex(double real);
	complex(double real, double imag);
	~complex() = default;

	// Copy and Move
	complex(complex const &) = default;
	complex(complex &&) = default;
	complex & operator=(complex const &) = default;
	complex & operator=(complex &&) = default;

	// Getters
	double real() const;
	double imag() const;
	std::string str() const;

	// Checkers
	bool is_zero() const;
	bool is_complex() const;
};

// Opeartor Overloads
complex operator+(complex const & lhs, complex const & rhs);
complex operator-(complex const & lhs, complex const & rhs);
complex operator*(complex const & lhs, complex const & rhs);
complex operator/(complex const & lhs, complex const & rhs);
complex operator%(complex const & lhs, complex const & rhs);
complex operator-(complex const & rhs);
bool operator==(complex const & lhs, complex const & rhs);
bool operator!=(complex const & lhs, complex const & rhs);
std::ostream & operator<<(std::ostream & os, complex const & rhs);

// Other Operations
complex complex_round(complex const & nb);
complex complex_sqrt(complex const & nb);

}