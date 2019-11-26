#include "evaluator.hpp"
#include <iostream>

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
		// factory.print_variables();
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
	/*
	auto rhs = evaluate(input.expression_);
	factory.variable_map[input.variable_] = rhs;
	*/
}

void evaluator::operator()(ast::function_assignation input)
{
	std::cout << "function_assignation\n";
	/*
	auto rhs = evaluate(input.expression_);
	factory.function_map[input.function_.function_] = std::pair<ast::variable, expr>{input.function_.variable_, rhs};
	*/
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
		[this](ast::variable const & variable)	{ return expr{}; },
		[this](ast::function const & function)	{ return expr{}; },
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
		switch (operation.operator_)
		{
		case '+': ret = ret + rhs; break;
		case '-': ret = ret - rhs; break;
		case '*': ret = ret * rhs; break;
		// case '/': ret = ret / rhs; break;
		// case '%': ret = ret % rhs; break;
		default: throw std::runtime_error("invalid operation");
		}
	}
	return ret;
}

}