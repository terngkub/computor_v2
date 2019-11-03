#include "expr.hpp"
#include <iostream>
#include <set>

namespace computorv2
{

expr::expr() : term_map()
{
	term_map[0] = term{};
}

expr::expr(complex nb) : term_map()
{
	term_map[0] = term{nb};
}

expr::expr(client::ast::matrix matrix) : term_map()
{
	term_map[0] = term{matrix};
}

expr::expr(client::ast::variable variable) : term_map()
{
	term_map[0] = term{variable};
}

expr::expr(term t) : term_map()
{
	term_map[0] = term{t};
}



expr & expr::operator+(expr & rhs)
{
	std::set<int> degree_set{};
	for (auto const & elem : this->term_map)
		degree_set.insert(elem.first);
	for (auto const & elem : rhs.term_map)
		degree_set.insert(elem.first);

	for (auto const degree : degree_set)
	{
		if (this->term_map.find(degree) == this->term_map.end())
			this->term_map[degree] = rhs.term_map.at(degree);
		else if (rhs.term_map.find(degree) == rhs.term_map.end())
			continue;
		else
			this->term_map[degree] = this->term_map[degree] + rhs.term_map.at(degree);
	}

	return *this;
}

expr & expr::operator-(expr & rhs)
{
	std::set<int> degree_set{};
	for (auto const & elem : this->term_map)
		degree_set.insert(elem.first);
	for (auto const & elem : rhs.term_map)
		degree_set.insert(elem.first);

	for (auto const degree : degree_set)
	{
		if (this->term_map.find(degree) == this->term_map.end())
			this->term_map[degree] = -rhs.term_map.at(degree);
		else if (rhs.term_map.find(degree) == rhs.term_map.end())
			continue;
		else
			this->term_map[degree] = this->term_map[degree] - rhs.term_map.at(degree);
	}

	return *this;
}

std::ostream &operator<<(std::ostream &os, expr const &rhs)
{
	for (auto const &t : rhs.term_map)
	{
		os << t.second << ' ';
	}
	return os;
}

// expr_factory

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

} // namespace computorv2