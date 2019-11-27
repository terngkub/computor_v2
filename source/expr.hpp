#pragma once
#include "ast.hpp"
#include "complex.hpp"
#include "term.hpp"
#include <iostream>
#include <map>
#include <set>

namespace computorv2
{

struct expr
{
	std::map<int, term> term_map;

	// Constructor and Destructor
	expr();
	expr(complex nb);
	expr(ast::matrix matrix);
	expr(ast::variable variable);
	expr(term t);
	~expr() = default;

	// Checker
	bool is_matrix() const;

	// Operations
	expr operator+(expr const & rhs) const;
	expr operator-(expr const & rhs) const;
	expr operator*(expr const & rhs) const;
	expr operator/(expr const & rhs) const;
	// expr & operator%(expr const & rhs) const;
	expr matrix_mul(expr const & rhs) const;
	
	expr substitute_variable(expr const & value);

private:
	std::set<int> get_all_degrees(expr const & rhs) const;
};

std::ostream & operator<<(std::ostream & os, expr const & rhs);

} // namespace computorv2