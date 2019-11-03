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
	void operator()(client::ast::input x);
	void operator()(client::ast::command x);
	void operator()(client::ast::variable_assignation x);
	void operator()(client::ast::function_assignation x);
	void operator()(client::ast::value_resolution x);
	void operator()(client::ast::polynomial_resolution x);

private:
	// Private attributes
	expr_factory factory;

    void print_variable_list() const;

	expr get_expr(client::ast::operand operand);
	expr evaluate(client::ast::expression expression);
};

}