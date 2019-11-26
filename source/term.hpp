#pragma once
#include "ast.hpp"
#include "complex.hpp"
#include "matrix.hpp"
#include <iostream>

namespace computorv2
{

struct term
{
public:
	complex coef;
	std::string variable;
	matrix mt;

	// Constructor, Destructor
	term();
	term(complex nb);
	term(matrix mt);
	term(ast::variable variable);
	term(complex const & nb, ast::variable const & variable, matrix const & mt);
	~term() = default;

	// Checker
	bool is_matrix() const;

	// Operations
	term operator+(term const & rhs) const;
	term operator-(term const & rhs) const;
	term operator*(term const & rhs) const;
	term operator/(term const & rhs) const;
	// term operator%(term const &rhs) const;
	term operator-() const;
	term matrix_mul(term const & rhs) const;
};

std::ostream & operator<<(std::ostream & os, term const & rhs);

}
