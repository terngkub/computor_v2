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
	term(complex const & nb, ast::variable const & variable, ast::matrix const & matrix);
	~term() = default;

	// Operations
	term operator+(term const &rhs) const;
	term operator-(term const &rhs) const;
	term operator*(term const &rhs) const;
	term operator/(term const &rhs) const;
	term operator%(term const &rhs) const;
	term operator-() const;

private:
	void check_validity(term const & rhs) const;
};

std::ostream & operator<<(std::ostream & os, term const & rhs);

}
