#pragma once
#include "ast.hpp"
#include "complex.hpp"
#include "matrix.hpp"
#include <iostream>
#include <variant>

namespace computorv2
{

class term
{
	std::variant<complex, matrix> _coef;
	std::string _variable;

public:

	// Constructor, Destructor
	term();
	term(std::variant<complex, matrix> const & val);
	term(std::string var);
	term(std::variant<complex, matrix> const & val, std::string var);
	~term() = default;

	// Getters
	std::variant<complex, matrix> coef() const;
	std::string variable() const;
	std::string str() const;
	int degree() const;

	// Checkers
	bool is_complex() const;
	bool is_matrix() const;
	bool has_variable() const;
	bool is_zero() const;
};

// Operation Overloads
term operator+(term const & lhs, term const & rhs);
term operator-(term const & lhs, term const & rhs);
term operator*(term const & lhs, term const & rhs);
term operator/(term const & lhs, term const & rhs);
term operator%(term const & lhs, term const & rhs);
term operator-(term const & rhs);
bool operator==(term const & lhs, term const & rhs);
std::ostream & operator<<(std::ostream & os, term const & rhs);

// Other Operations
term term_matrix_mul(term const & lhs, term const & rhs);


}
