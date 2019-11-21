#pragma once
#include "ast.hpp"
#include "expr_factory.hpp"
#include <unordered_map>

namespace computorv2
{

class evaluator : public boost::static_visitor<>
{
public:
	// Public operator()
	void operator()(ast::input x);
	void operator()(ast::command x);
	void operator()(ast::variable_assignation x);
	void operator()(ast::function_assignation x);
	void operator()(ast::value_resolution x);
	void operator()(ast::polynomial_resolution x);

private:
	// Private attributes
	expr_factory factory;

    void print_variable_list() const;

	expr get_expr(ast::operand operand);
	expr evaluate(ast::expression expression);
};

}