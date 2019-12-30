#include "evaluator.hpp"
#include "math.hpp"
#include <functional>
#include <iostream>
#include <sstream>

namespace computorv2
{

// Function Object

std::string evaluator::operator()(ast::input x)
{
	return boost::apply_visitor(*this, x);
}

std::string evaluator::operator()(std::string x)
{
	if (x == "list_variables")
		return print_variables();
	else if (x == "list_functions")
		return print_functions();
	else if (x == "exit")
		exit(EXIT_SUCCESS);
	throw std::runtime_error("unknown command");
}

std::string evaluator::operator()(ast::variable_assignation input)
{
	if (input.variable_ == "i")
		throw std::runtime_error("can't use i as a variable name");

	auto rhs = evaluate(input.expression_);

	if (rhs.variable() != "")
		throw std::runtime_error("assigned value contain unassigned variable(s)");

	variable_map[input.variable_] = rhs;

	std::stringstream ss;
	ss << rhs;
	return ss.str();
}

std::string evaluator::operator()(ast::function_assignation input)
{
	// TODO check if function have extra unkown variable
	function_map[input.function_.function_] = std::pair<std::string, ast::expression>{input.function_.variable_, input.expression_};
	return print(input.expression_);
}

std::string evaluator::operator()(ast::value_resolution x)
{
	auto ret = evaluate(x.expression_);
	std::stringstream ss;
	ss << ret;
	return ss.str();
}

std::string evaluator::operator()(ast::polynomial_resolution x)
{
	auto left = evaluate(x.left_expression);
	auto right = evaluate(x.right_expression);
	auto equation = left - right;
	return polynomial_resolution(equation);
}


// Private Methods

expr evaluator::create_expr(ast::operand const & operand)
{
	return boost::apply_visitor(overloaded
	{
		[this](double const & rational)	{ return expr{complex{rational, 0}}; },
		[this](char const & imaginary)	{ return expr{complex{0, 1}}; },
		[this](std::vector<std::vector<double>> const & matrix)		{ return expr{matrix}; },
		[this](std::string const & variable)	{ return variable_map.find(variable) != variable_map.end() ? variable_map[variable] : expr{variable}; },
		[this](ast::used_function const & function)
		{
			auto it = function_map.find(function.function_);
			if (it == function_map.end())
				throw std::runtime_error("unknown function");

			expr input_expr = evaluate(function.expression_);

			variable_map[it->second.first] = input_expr;
			expr ret_expr = evaluate(it->second.second);
			variable_map.erase(it->first);

			return ret_expr;
		},
		[this](ast::parenthesis const & parenthesis) { return evaluate(parenthesis.expression_); },
		[this](ast::negate const & negate) { return create_expr(negate.operand_) * expr{complex{-1}}; },
		[this](ast::expression const & expression)	{ return evaluate(expression); }
	}
	, operand);
}

expr evaluator::evaluate(ast::expression expression)
{
	auto ret = create_expr(expression.first);
	for (auto const & operation : expression.rest)
	{
		auto rhs = create_expr(operation.operand_);

		static std::map<std::string, std::function<expr(expr const &, expr const &)>> operation_map
		{
			{"", [](expr const & ret, expr const & rhs){ return ret * rhs; }},
			{"+", [](expr const & ret, expr const & rhs){ return ret + rhs; }},
			{"-", [](expr const & ret, expr const & rhs){ return ret - rhs; }},
			{"*", [](expr const & ret, expr const & rhs){ return ret * rhs; }},
			{"/", [](expr const & ret, expr const & rhs){ return ret / rhs; }},
			{"%", [](expr const & ret, expr const & rhs){ return ret % rhs; }},
			{"^", [](expr const & ret, expr const & rhs){ return ret ^ rhs; }},
			{"**", [](expr const & ret, expr const & rhs){ return expr_matrix_mul(ret, rhs); }}
		};
		if (operation_map.find(operation.operator_) == operation_map.end())
			throw std::runtime_error("invalid operator " + operation.operator_);
		ret = operation_map[operation.operator_](ret, rhs);
	}
	return ret;
}

std::string evaluator::print_variables() const
{
	if (variable_map.size() == 0)
		return "no assigned variable";

	std::stringstream ss;
	for (auto it = variable_map.cbegin(); it != variable_map.cend(); ++it)
	{
		if (it != variable_map.cbegin())
			ss << "  ";
		ss << it->first << " = " << it->second;
		if (it != --variable_map.cend())
			ss << '\n';
	}
	return ss.str();
}

std::string evaluator::print_functions() const
{
	if (function_map.size() == 0)
		return "no assigned function";

	std::stringstream ss;
	for (auto it = function_map.cbegin(); it != function_map.cend(); ++it)
	{
		if (it != function_map.cbegin())
			ss << "  ";
		ss << it->first << "(" << it->second.first << ") = " << print(it->second.second);
		if (it != --function_map.cend())
			ss << '\n';
	}
	return ss.str();
}


// Polynomial resolution

std::string evaluator::polynomial_resolution(expr const & equation) const
{
	auto max_degree = equation.term_map().crbegin()->first;
	if (max_degree == 1)
		return solve_equation(equation);
	else if (max_degree == 2)
		return solve_polynomial(equation);
	else if (max_degree == 0)
		throw std::runtime_error("no variable in the equation");
	else if (max_degree < 0)
		throw std::runtime_error("polynomial with negative degree");
	else
		throw std::runtime_error("polynomial with degree more than two");
	return "";
}

std::string evaluator::solve_equation(expr const & equation) const
{
	auto b = equation.term_map().find(1) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(1).coef()) : complex{};
	auto c = equation.term_map().find(0) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(0).coef()) : complex{};

	auto result = -c / b;

	std::stringstream ss;
	ss << equation.variable() << " = " << result;
	return ss.str();
}

std::string evaluator::solve_polynomial(expr const & equation) const
{
	if (equation.term_map().find(2) == equation.term_map().cend())
		throw std::runtime_error("can't find term with degree two");

	auto a = std::get<complex>(equation.term_map().at(2).coef());
	auto b = equation.term_map().find(1) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(1).coef()) : complex{};
	auto c = equation.term_map().find(0) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(0).coef()) : complex{};

	auto sqrt_part = complex_sqrt(b * b - 4 * a * c);
	auto result_one = (-b + sqrt_part) / (2 * a);
	auto result_two = (-b - sqrt_part) / (2 * a);

	std::stringstream ss;
	ss << equation.variable() << " = ";
	if (result_one == result_two)
		ss << result_one;
	else
		ss << result_one << ", " << result_two;
	return ss.str();
}

}