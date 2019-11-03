#include "evaluator.hpp"
#include <iostream>

namespace computorv2
{

void evaluator::operator()(client::ast::input x)
{
	boost::apply_visitor(*this, x);
}

void evaluator::operator()(client::ast::command x)
{
	if (x == "list variables")
	{
		factory.print_variables();
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

void evaluator::operator()(client::ast::variable_assignation input)
{
	std::cout << "variable_assignation\n";
	auto rhs = evaluate(input.expression_);
	factory.variable_map[input.variable_] = rhs;
}

void evaluator::operator()(client::ast::function_assignation input)
{
	std::cout << "function_assignation\n";
	auto rhs = evaluate(input.expression_);
	factory.function_map[input.function_.function_] = std::pair<client::ast::variable, expr>{input.function_.variable_, rhs};
}

void evaluator::operator()(client::ast::value_resolution x)
{
	std::cout << "value_resolution\n";
	auto ret = evaluate(x.expression_);
	std::cout << ret << '\n';
}

void evaluator::operator()(client::ast::polynomial_resolution x)
{
	std::cout << "polynomial_resolution\n";
	// expression = expression ?
	// evaluate left
	// evaluate right
	// evaluate (left - right)
	// check validity
	// solve polynomial
}

expr evaluator::get_expr(client::ast::operand operand)
{
	return factory(operand);
}

expr evaluator::evaluate(client::ast::expression expression)
{
	auto ret = get_expr(expression.first);
	for (auto const & operation : expression.rest)
	{
		auto rhs = get_expr(operation.operand_);
		switch (operation.operator_)
		{
		case '+': ret = ret + rhs; break;
		case '-': ret = ret - rhs; break;
		/*
		case '*': ret = ret * rhs; break;
		case '/': ret = ret / rhs; break;
		case '%': ret = ret % rhs; break;
		*/
		default: throw std::runtime_error("invalid operation");
		}
	}
	return ret;
}

}