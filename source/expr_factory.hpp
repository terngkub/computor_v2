#pragma once
#include "expr.hpp"
#include <unordered_map>

namespace computorv2
{

struct expr_factory : public boost::static_visitor<expr>
{
	std::unordered_map<ast::variable, expr> variable_map;
	std::unordered_map<ast::name, std::pair<ast::variable, expr>> function_map;

	expr operator()(ast::operand operand);
	expr operator()(ast::rational rational);
	expr operator()(ast::imaginary imaginary);
	expr operator()(ast::matrix matrix);
	expr operator()(ast::variable variable);
	expr operator()(ast::function function);
	expr operator()(ast::expression function);

	void print_variables() const;
	void print_functions() const;
};

}