#include "evaluator.hpp"
#include <iostream>
#include <map>
#include <functional>

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
	// expression = expression ?
	// evaluate left
	// evaluate right
	// evaluate (left - right)
	// check validity
	// solve polynomial
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
			auto it = function_map.find(function.function_);
			if (it == function_map.end())
				throw std::runtime_error("unknown function");

			// this is a bit hard since the expr eval isn't stable yet

			/*
			expr contain term
			term contain variable
			so for each term that have variable
				change it to new_expr and then time it to the old value
			*/
			
			return expr{};
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
			{"**", [](expr const & ret, expr const & rhs){ return ret.matrix_mul(rhs); }}
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

}