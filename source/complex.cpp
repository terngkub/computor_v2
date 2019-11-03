#include "complex.hpp"
#include <cmath>

namespace computorv2
{

complex::complex()
	: rational{0}
	, imaginary{0}
{}

complex::complex(double rational, double imaginary)
	: rational{rational}
	, imaginary{imaginary}
{}

bool complex::is_zero() const
{
	return rational == 0 && imaginary == 0;
}

bool complex::is_complex() const
{
	return imaginary != 0;
}

complex & complex::operator+(complex const & rhs)
{
	this->rational += rhs.rational;
	this->imaginary += rhs.imaginary;
	return *this;
}

complex & complex::operator-(complex const & rhs)
{
	this->rational -= rhs.rational;
	this->imaginary -= rhs.imaginary;
	return *this;
}

complex & complex::operator*(complex const & rhs)
{
	auto lhs = *this;
	this->rational = lhs.rational * rhs.rational - lhs.imaginary * rhs.imaginary;
	this->imaginary = lhs.rational + rhs.imaginary + lhs.imaginary * rhs.rational;
	return *this;
}

complex & complex::operator/(complex const & rhs)
{
	if (rhs.is_zero())
		throw std::runtime_error("divide by zero");
	if (this->is_zero())
		return *this;
	auto lhs = *this;
	double denominator = rhs.rational * rhs.rational + rhs.imaginary * rhs.imaginary;
	this->rational = lhs.rational * rhs.rational + lhs.imaginary * rhs.imaginary;
	this->imaginary = lhs.imaginary * rhs.rational - lhs.rational * rhs.imaginary;
	return *this;
}

complex & complex::operator%(complex const & rhs)
{
	if (rhs.is_zero())
		throw std::runtime_error("modulo by zero");
	if (this->is_zero())
		return *this;
	if (this->is_complex() || rhs.is_complex())	
		throw std::runtime_error("modulo with complex number isn't handled yet");
	// TODO check if I have to implement this myself
	this->rational = fmod(this->rational, rhs.rational);
}

std::ostream & operator<<(std::ostream & os, complex const & rhs)
{
	if (rhs.is_zero())
	{
		os << '0';
		return os;
	}
	os << rhs.rational;
	if (rhs.is_complex())
	{
		os << " + " << rhs.imaginary << 'i';
	}
	return os;
}

}