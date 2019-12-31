#pragma once
#include "ast.hpp"
#include "expr.hpp"
#include "printer.hpp"
#include "utils.hpp"
#include <map>
#include <vector>

namespace computorv2
{

class evaluator : public boost::static_visitor<std::string>
{
public:
	// Function Object 
	std::string operator()(ast::input x, std::string);
	std::string operator()(std::string x);
	std::string operator()(ast::variable_assignation x);
	std::string operator()(ast::function_assignation x);
	std::string operator()(ast::value_resolution x);
	std::string operator()(ast::polynomial_resolution x);

private:

	struct function
	{
		std::string param;
		ast::expression tree;
		std::map<std::string, expr> variable_map;
		std::map<std::string, function> function_map;
	};

	struct function_checker
	{
		std::string const & _function_name;
		std::string const & _parameter;
		std::map<std::string, expr> const & variable_map;
		std::map<std::string, function> const & function_map;

		void operator()(ast::expression const &);
		void operator()(ast::operation const &);
		void operator()(ast::operand const &);
		void operator()(double);
		void operator()(char);
		void operator()(std::string const &);
		void operator()(std::vector<std::vector<double>> const &);
		void operator()(ast::parenthesis const &);
		void operator()(ast::used_function const &);
		void operator()(ast::negate const &);
	};
	
	printer print;

	// Private attributes
	std::map<std::string, expr> variable_map;
	std::map<std::string, function> function_map;
	std::vector<std::string> history_list;

	// Private Methods
	expr create_expr(ast::operand const & operand);
	expr evaluate(ast::expression expression);
	std::string print_variables() const;
	std::string print_functions() const;
	std::string print_history() const;

	std::string polynomial_resolution(expr const & equation) const;
	std::string solve_equation(expr const & equation) const;
	std::string solve_polynomial(expr const & equation) const;
};

}