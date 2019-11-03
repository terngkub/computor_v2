#pragma once
#include "expr.hpp"
#include <unordered_map>

namespace computorv2
{

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

}