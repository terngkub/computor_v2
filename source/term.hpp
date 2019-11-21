#pragma once
#include "ast.hpp"
#include "complex.hpp"
#include <iostream>

namespace computorv2
{

struct term
{
public:
	complex coef;
	std::string variable;
	ast::matrix matrix;

	// Constructor, Destructor
	term();
	term(complex nb);
	term(ast::matrix matrix);
	term(ast::variable variable);
	~term() = default;

	// Operations
	term & operator+(term const &rhs);
	term & operator-(term const &rhs);
	term & operator*(term const &rhs);
	term & operator/(term const &rhs);
	term & operator%(term const &rhs);
	term & operator-();
};

std::ostream & operator<<(std::ostream & os, term const & rhs);

}
