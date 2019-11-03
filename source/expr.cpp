#include "expr.hpp"

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
	auto degree_set = get_all_degrees(rhs);

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
	auto degree_set = get_all_degrees(rhs);

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

std::set<int> expr::get_all_degrees(expr const & rhs)
{
	std::set<int> degree_set{};
	for (auto const & elem : this->term_map)
		degree_set.insert(elem.first);
	for (auto const & elem : rhs.term_map)
		degree_set.insert(elem.first);
	return degree_set;
}

} // namespace computorv2