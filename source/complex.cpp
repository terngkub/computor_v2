#include "complex.hpp"
#include <cmath>

namespace computorv2
{

// Constructors

complex::complex()
	: real{0}
	, imag{0}
{}

complex::complex(double real, double imag)
	: real{real}
	, imag{imag}
{}


// Checkers

bool complex::is_zero() const
{
	return real == 0 && imag == 0;
}

bool complex::is_complex() const
{
	return imag != 0;
}


// Operations

complex complex::operator+(complex const & rhs) const
{
	auto new_real = real + rhs.real;
	auto new_imag = imag + rhs.imag;
	return complex{new_real, new_imag};
}

complex complex::operator-(complex const & rhs) const
{
	auto new_real = real - rhs.real;
	auto new_imag = imag- rhs.imag;
	return complex{new_real, new_imag};
}

complex complex::operator*(complex const & rhs) const
{
	auto new_real = real * rhs.real - imag * rhs.imag;
	auto new_imag = real * rhs.imag + imag * rhs.real;
	return complex{new_real, new_imag};
}

complex complex::operator/(complex const & rhs) const
{
	if (rhs.is_zero())
		throw std::runtime_error("divide by zero");

	if (is_zero())
		return complex{};

	auto denom = rhs.real * rhs.real + rhs.imag * rhs.imag;
	auto new_real = (real * rhs.real + imag * rhs.imag) / denom;
	auto new_imag = (imag * rhs.real - real * rhs.imag) / denom;

	return complex{new_real, new_imag};
}

complex complex::operator%(complex const & rhs) const
{
	if (rhs.is_zero())
		throw std::runtime_error("modulo by zero");

	if (is_zero())
		return complex{};

	if (is_complex() || rhs.is_complex())	
		throw std::runtime_error("modulo with complex number isn't handled yet");

	// TODO check if I have to implement this myself
	auto new_real = fmod(real, rhs.real);
	return complex{new_real, 0};
}

complex complex::operator-() const
{
	auto new_real = -real;
	auto new_imag = -imag;
	return complex{new_real, new_imag};
}

std::ostream & operator<<(std::ostream & os, complex const & rhs)
{
	if (rhs.real == 0)
	{
		if (rhs.imag == 0)
			os << '0';
		else
			os << rhs.imag + 'i';
	}
	else
	{
		os << rhs.real;
		if (rhs.imag != 0)
			os << " + " << rhs.imag << 'i';
	}
	return os;
}

}