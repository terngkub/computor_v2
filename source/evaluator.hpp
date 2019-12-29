#pragma once
#include "ast.hpp"
#include "expr.hpp"
#include "printer.hpp"
#include "utils.hpp"
#include <unordered_map>

namespace computorv2
{

class evaluator : public boost::static_visitor<std::string>
{
public:
	// Function Object 
	std::string operator()(ast::input x);
	std::string operator()(std::string x);
	std::string operator()(ast::variable_assignation x);
	std::string operator()(ast::function_assignation x);
	std::string operator()(ast::value_resolution x);
	std::string operator()(ast::polynomial_resolution x);

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

	std::string polynomial_resolution(expr const & equation) const;
	std::string solve_equation(expr const & equation) const;
	std::string solve_polynomial(expr const & equation) const;
};

}