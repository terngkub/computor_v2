#include "complex.hpp"
#include "math.hpp"
#include <sstream>
#include <iostream>

namespace computorv2
{

// Constructors

complex::complex()
	: _real{0}
	, _imag{0}
{}

complex::complex(double real)
	: _real{real}
	, _imag{0}
{
	// check for nan from 
	if (_real != _real)
		throw std::runtime_error("the evaluation cause the number to be NaN");
}

complex::complex(double real, double imag)
	: _real{real}
	, _imag{imag}
{
	// check for nan
	if (_real != _real || _imag != _imag)
		throw std::runtime_error("the evaluation cause the number to be NaN");
}


// Getters

double complex::real() const
{
	return _real;
}

double complex::imag() const
{
	return _imag;
}

std::string complex::str() const
{
	std::stringstream ss;
	
	if (_real == 0)
	{
		if (_imag == 0)
			ss << '0';
		else if (_imag == 1)
			ss << 'i';
		else if (_imag == -1)
			ss << "-i";
		else
			ss << _imag << 'i';
	}
	else
	{
		ss << _real;
		if (_imag == 0)
			return ss.str();
		else if (_imag == 1)
			ss << " + i";
		else if (_imag == -1)
			ss << " - i";
		else if (_imag > 0)
			ss << " + " << _imag << 'i';
		else
			ss << " - " << -_imag << 'i';
	}
	return ss.str();
}


// Checkers

bool complex::is_zero() const
{
	return _real == 0 && _imag == 0;
}

bool complex::is_complex() const
{
	return _imag != 0;
}


// Operator Overloads

complex operator+(complex const & lhs, complex const & rhs)
{
	auto real = lhs.real() + rhs.real();
	auto imag = lhs.imag() + rhs.imag();
	return complex{real, imag};
}

complex operator-(complex const & lhs, complex const & rhs)
{
	auto real = lhs.real() - rhs.real();
	auto imag = lhs.imag() - rhs.imag();
	return complex{real, imag};
}

complex operator*(complex const & lhs, complex const & rhs)
{
	auto real = lhs.real() * rhs.real() - lhs.imag() * rhs.imag();
	auto imag = lhs.real() * rhs.imag() + lhs.imag() * rhs.real();
	return complex{real, imag};
}

complex operator/(complex const & lhs, complex const & rhs)
{
	if (rhs.is_zero())
		throw std::runtime_error("divide by zero");

	if (lhs.is_zero())
		return complex{};

	auto denom = rhs.real() * rhs.real() + rhs.imag() * rhs.imag();
	auto real = (lhs.real() * rhs.real() + lhs.imag() * rhs.imag()) / denom;
	auto imag = (lhs.imag() * rhs.real() - lhs.real() * rhs.imag()) / denom;

	return complex{real, imag};
}

complex operator%(complex const & lhs, complex const & rhs)
{
	if (rhs.is_zero())
		throw std::runtime_error("modulo by zero");

	if (lhs.is_zero())
		return complex{};

	if (lhs.is_complex() || rhs.is_complex())	
		return (lhs - (rhs * complex_round(lhs / rhs)));

	auto real = math::fmod(lhs.real(), rhs.real());
	return complex{real, 0};
}

complex operator-(complex const & rhs)
{
	auto real = -rhs.real();
	auto imag = -rhs.imag();
	return complex{real, imag};
}

bool operator==(complex const & lhs, complex const & rhs)
{
	return math::fequal(lhs.real(), rhs.real()) && math::fequal(lhs.imag(), rhs.imag());
}

bool operator!=(complex const & lhs, complex const & rhs)
{
	return !(lhs == rhs);
}

std::ostream & operator<<(std::ostream & os, complex const & rhs)
{
	os << rhs.str();
	return os;
}


// Other Operations

complex complex_round(complex const & nb)
{
	auto real = math::round(nb.real());
	auto imag = math::round(nb.imag());
	return complex{real, imag};
}

complex complex_sqrt(complex const & nb)
{
	if (nb.is_zero())
		return complex{};

	auto a = nb.real();
	auto b = nb.imag();

	if (b == 0)
	{
		if (a > 0)
			return complex{math::sqrt(a), 0};
		return complex{0, math::sqrt(-a)};
	}

	auto x = math::sqrt((math::sqrt(math::power(a, 2) + math::power(b, 2)) + a ) / 2);
	auto y = b / (2 * x);

	return complex{x, y};
}

}