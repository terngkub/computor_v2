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
	expr(client::ast::matrix matrix);
	expr(client::ast::variable variable);
	expr(term t);
	~expr() = default;

	// Operations
	expr & operator+(expr & rhs);
	expr & operator-(expr & rhs);
	/*
	expr & operator*(expr & rhs);
	expr & operator/(expr & rhs);
	expr & operator%(expr & rhs);
	*/

private:
	std::set<int> get_all_degrees(expr const & rhs);
};

std::ostream & operator<<(std::ostream & os, expr const & rhs);

} // namespace computorv2