#include "expr_factory.hpp"

namespace computorv2
{

expr expr_factory::operator()(client::ast::operand operand)
{
	return boost::apply_visitor(*this, operand);
}

expr expr_factory::operator()(client::ast::rational rational)
{
	complex nb{rational, 0};
	return expr{nb};
}

expr expr_factory::operator()(client::ast::imaginary imaginary)
{
	complex nb{0, 1};
	return expr{nb};
}

expr expr_factory::operator()(client::ast::matrix matrix)
{
	return expr{matrix};
}

expr expr_factory::operator()(client::ast::variable variable)
{
	if (variable_map.find(variable) != variable_map.end())
	{
		return variable_map[variable];
	}
	return expr{variable};
}

expr expr_factory::operator()(client::ast::function function)
{
	(void)function;
	return expr{};
}

expr expr_factory::operator()(client::ast::expression expression)
{
	return (*this)(expression.first);
}

void expr_factory::print_variables() const
{
	for (auto it = variable_map.cbegin(); it != variable_map.cend(); ++it)
	{
		std::cout << it->first << " = " << it->second << '\n';
	}
}

void expr_factory::print_functions() const
{
	// TODO add this
}

}