#pragma once
#include "ast.hpp"
#include "expr.hpp"
#include "printer.hpp"
#include "utils.hpp"
#include <unordered_map>

namespace computorv2
{

class evaluator : public boost::static_visitor<>
{
public:
	// Function Object 
	void operator()(ast::input x);
	void operator()(std::string x);
	void operator()(ast::variable_assignation x);
	void operator()(ast::function_assignation x);
	void operator()(ast::value_resolution x);
	void operator()(ast::polynomial_resolution x);

private:
	printer print;

	// Private attributes
	std::unordered_map<std::string, expr> variable_map;
	std::unordered_map<std::string, std::pair<std::string, ast::expression>> function_map;

	// Private Methods
	expr create_expr(ast::operand const & operand);
	expr evaluate(ast::expression expression);
	void print_variables() const;
	void print_functions() const;

	void polynomial_resolution(expr const & equation) const;
	void solve_equation(expr const & equation) const;
	void solve_polynomial(expr const & equation) const;
};

}