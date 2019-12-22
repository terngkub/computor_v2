#pragma once
#include "ast.hpp"
#include "complex.hpp"
#include "term.hpp"
#include <iostream>
#include <map>
#include <set>

namespace computorv2
{

class expr
{
	std::map<int, term> _term_map;

public:

	// Constructors and Destructor
	expr();
	expr(complex nb);
	expr(std::vector<std::vector<double>> matrix);
	expr(std::string variable);
	expr(term t);
	expr(std::map<int, term> && tm);
	~expr() = default;

	// Getters
	std::map<int, term> const & term_map() const;
	std::string variable() const;
	int degree() const;
	std::string str() const;

	// Checkers
	bool is_coef() const;
	bool is_matrix() const;
	bool is_zero() const;
};

// Operations
expr operator+(expr const & lhs, expr const & rhs);
expr operator-(expr const & lhs, expr const & rhs);
expr operator*(expr const & lhs, expr const & rhs);
expr operator/(expr const & lhs, expr const & rhs);
expr operator%(expr const & lhs, expr const & rhs);
expr operator^(expr const & lhs, expr const & rhs);
// expr expr_matrix_mul(expr const & lhs, expr const & rhs);
std::ostream & operator<<(std::ostream & os, expr const & rhs);

} // namespace computorv2