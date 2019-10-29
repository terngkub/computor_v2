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
		print_variable_list();
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

void evaluator::operator()(client::ast::variable_assignation x)
{
	std::cout << "variable_assignation\n";
	if (variable_map.find(x.variable_) != variable_map.end())
	{
		std::cout << "warning: reassign variable " << x.variable_;
	}
	variable_map[x.variable_] = x.expression_;
}

void evaluator::operator()(client::ast::function_assignation x)
{
	std::cout << "function_assignation\n";
}

void evaluator::operator()(client::ast::value_resolution x)
{
	std::cout << "value_resolution\n";
}

void evaluator::operator()(client::ast::polynomial_resolution x)
{
	std::cout << "polynomial_resolution\n";
}

void evaluator::operator()(client::ast::expression x)
{
	std::cout << "expression\n";
}

void evaluator::print_variable_list() const
{
	for(auto it = variable_map.cbegin(); it != variable_map.cend(); ++it)
	{
		std::cout << it->first << '\n';
	}
}

}