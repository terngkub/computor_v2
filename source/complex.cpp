#include "complex.hpp"
#include <cmath>
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
{}

complex::complex(double real, double imag)
	: _real{real}
	, _imag{imag}
{}


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


// Operations

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
		throw std::runtime_error("modulo with complex number isn't handled yet");

	// TODO check if I have to implement this myself
	auto real = fmod(lhs.real(), rhs.real());
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
	return lhs.real() == rhs.real() && lhs.imag() == rhs.imag();
}

std::ostream & operator<<(std::ostream & os, complex const & rhs)
{
	os << rhs.str();
}

static double power(double base, int degree)
{
    if (degree == 0)
        return 1;

    bool neg = false;
    if (degree < 0)
    {
        degree *= -1;
        neg = true;
    }

    double ret = base;
    while (--degree)
        ret *= base;
    
    return (neg ? 1 / ret : ret);
}

static double sqrt(double nb)
{
    if (nb == 0)
        return 0;

    double x = 0;
    while (x * x <= nb)
        x += 0.1;

    double prev_x = x + 1;
    while (prev_x - x >= 0.000001)
    {
        prev_x = x;
        x = (x + nb / x) / 2;
    }

    return x;
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
			return complex{sqrt(a), 0};
		return complex{0, sqrt(-a)};
	}

	auto x = sqrt((sqrt(power(a, 2) + power(b, 2)) + a ) / 2);
	auto y = b / (2 * x);

	return complex{x, y};
}

}