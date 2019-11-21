#pragma once
#include "ast.hpp"
#include "expr.hpp"
#include "utils.hpp"
#include <unordered_map>

namespace computorv2
{

class evaluator : public boost::static_visitor<>
{
public:
	// Function Object 
	void operator()(ast::input x);
	void operator()(ast::command x);
	void operator()(ast::variable_assignation x);
	void operator()(ast::function_assignation x);
	void operator()(ast::value_resolution x);
	void operator()(ast::polynomial_resolution x);

private:
	// Private attributes
	std::unordered_map<ast::variable, expr> variable_map;
	std::unordered_map<ast::name, std::pair<ast::variable, expr>> function_map;

	// Private Methods
	expr create_expr(ast::operand const & operand);
	expr evaluate(ast::expression expression);
};

}