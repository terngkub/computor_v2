#pragma once
#include "ast.hpp"
#include "complex.hpp"
#include <map>
#include <unordered_map>
#include <iostream>

namespace computorv2
{

struct term
{
public:
	// Constructor, Destructor
	term();
	term(complex nb);
	term(client::ast::matrix matrix);
	term(client::ast::variable variable);
	~term() = default;

	// Operations
	term operator+(term const &rhs) const;

	complex coef;
	std::string variable;
	client::ast::matrix matrix;
	bool has_matrix;
};

std::ostream & operator<<(std::ostream & os, term const & rhs);


struct expr
{
	// Constructor and Destructor
	expr();
	expr(complex nb);
	expr(client::ast::matrix matrix);
	expr(client::ast::variable variable);
	expr(term t);
	~expr() = default;

	// Operations
	expr operator+(expr const &rhs) const;
	std::map<int, term> term_map;
};

std::ostream & operator<<(std::ostream & os, expr const & rhs);


struct expr_factory : public boost::static_visitor<expr>
{
	std::unordered_map<client::ast::variable, expr> variable_map;
	std::unordered_map<client::ast::name, std::pair<client::ast::variable, expr>> function_map;

	expr operator()(client::ast::operand operand);
	expr operator()(client::ast::rational rational);
	expr operator()(client::ast::imaginary imaginary);
	expr operator()(client::ast::matrix matrix);
	expr operator()(client::ast::variable variable);
	expr operator()(client::ast::function function);
	expr operator()(client::ast::expression function);

	void print_variables() const;
	void print_functions() const;
};

} // namespace computorv2