#include "evaluator.hpp"
#include <iostream>
#include <map>
#include <functional>
#include <cmath>

namespace computorv2
{

// Function Object

void evaluator::operator()(ast::input x)
{
	boost::apply_visitor(*this, x);
}

void evaluator::operator()(ast::command x)
{
	if (x == "list variables")
	{
		print_variables();
	}
	else if (x == "exit")
	{
		exit(EXIT_SUCCESS);
	}
	else
	{
		std::cout << "unknown command\n";
	}
}

void evaluator::operator()(ast::variable_assignation input)
{
	std::cout << "variable_assignation\n";
	auto rhs = evaluate(input.expression_);
	variable_map[input.variable_] = rhs;
}

void evaluator::operator()(ast::function_assignation input)
{
	// TODO check if function have extra unkown variable
	std::cout << "function_assignation\n";
	auto rhs = evaluate(input.expression_);
	function_map[input.function_.function_] = std::pair<ast::variable, expr>{input.function_.variable_, rhs};
}

void evaluator::operator()(ast::value_resolution x)
{
	std::cout << "value_resolution\n";
	auto ret = evaluate(x.expression_);
	std::cout << ret << '\n';
}

void evaluator::operator()(ast::polynomial_resolution x)
{
	std::cout << "polynomial_resolution\n";

	auto left = evaluate(x.left_expression);
	auto right = evaluate(x.right_expression);
	auto equation = left - right;
	polynomial_resolution(equation);
}


// Private Methods

expr evaluator::create_expr(ast::operand const & operand)
{
	return boost::apply_visitor(overloaded
	{
		[this](ast::rational const & rational)	{ return expr{complex{rational, 0}}; },
		[this](ast::imaginary const & imaginary)	{ return expr{complex{0, 1}}; },
		[this](ast::matrix const & matrix)		{ return expr{matrix}; },
		[this](ast::variable const & variable)	{ return variable_map.find(variable) != variable_map.end() ? variable_map[variable] : expr{variable}; },
		[this](ast::used_function const & function)
		{
			// used_function contain name and expression
			// assigned function contain name and pair(variable, expression)

			// error if function isn't assigned
			auto it = function_map.find(function.function_);
			if (it == function_map.end())
				throw std::runtime_error("unknown function");

			expr ret_expr{};
			expr input_expr = evaluate(function.expression_);

			// for each elem in assigned function expression
			for (auto const & elem : it->second.second.term_map())
			{
				// if the term contain variable
				if (elem.second.has_variable())
				{
					// remove variable
					term new_term{elem.second.coef(), ""};

					// power used_function's expression
					expr new_expr{input_expr};

					for (auto i = 1; i < elem.first; ++i)
						new_expr = new_expr * input_expr;

					// time it with the assigned_function's expression
					new_expr = new_expr * expr{new_term};

					// add the result to ret_expr;
					ret_expr = ret_expr + new_expr;
				}
				else
				{
					ret_expr = ret_expr + expr{elem.second};
				}
			}

			return ret_expr;
		},
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
			{"+", [](expr const & ret, expr const & rhs){ return ret + rhs; }},
			{"-", [](expr const & ret, expr const & rhs){ return ret - rhs; }},
			{"*", [](expr const & ret, expr const & rhs){ return ret * rhs; }},
			{"/", [](expr const & ret, expr const & rhs){ return ret / rhs; }},
			{"%", [](expr const & ret, expr const & rhs){ return ret % rhs; }},
			{"^", [](expr const & ret, expr const & rhs){ return ret ^ rhs; }},
			// {"**", [](expr const & ret, expr const & rhs){ return ret.matrix_mul(rhs); }}
		};
		if (operation_map.find(operation.operator_) == operation_map.end())
			throw std::runtime_error("invalid operator " + operation.operator_);
		ret = operation_map[operation.operator_](ret, rhs);
	}
	return ret;
}

void evaluator::print_variables() const
{
	for (auto const & elem : variable_map)
	{
		std::cout << elem.first << " = " << elem.second << '\n';
	}
}


// Polynomial resolution

void evaluator::polynomial_resolution(expr const & equation) const
{
	auto max_degree = equation.term_map().crbegin()->first;
	if (max_degree == 1)
		solve_equation(equation);
	else if (max_degree == 2)
		solve_polynomial(equation);
	else if (max_degree == 0)
		std::cout << "x can be any numbers\n";
	else if (max_degree < 0)
		throw std::runtime_error("polynomial with negative degree");
	else
		throw std::runtime_error("polynomial with degree more than two");
}

void evaluator::solve_equation(expr const & equation) const
{
	auto b = equation.term_map().find(1) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(1).coef()) : complex{};
	auto c = equation.term_map().find(0) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(0).coef()) : complex{};

	if (c == 0)
	{
		std::cout << equation.variable() << " = 0\n";
		return ;
	}

	auto result = b / -c;

	std::cout << equation.variable() << " = " << result << '\n';
}

void evaluator::solve_polynomial(expr const & equation) const
{
	// TODO handle when b or c isn't in the map
	// TODO handle complex
	auto a = equation.term_map().find(2) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(2).coef()).real() : 0;
	auto b = equation.term_map().find(1) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(1).coef()).real() : 0;
	auto c = equation.term_map().find(0) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(0).coef()).real() : 0;

	// TODO handle negative b^2 - 4ac

	// TODO implement pow
	auto result_one = (-b + pow(pow(b, 2) - 4 * a * c, 0.5)) / (2 * a);
	auto result_two = (-b - pow(pow(b, 2) - 4 * a * c, 0.5)) / (2 * a);

	std::cout << equation.variable() << " = ";
	if (result_one == result_two)
		std::cout << result_one << '\n';
	else
		std::cout << result_one << ", " << result_two << '\n';
}

}