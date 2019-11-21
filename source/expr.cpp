#include "expr.hpp"

namespace computorv2
{

// Constructors

expr::expr()
: term_map{}
{
	term_map[0] = term{};
}

expr::expr(complex nb)
: term_map{}
{
	term_map[0] = term{nb};
}

expr::expr(ast::matrix matrix)
: term_map{}
{
	term_map[0] = term{matrix};
}

expr::expr(ast::variable variable)
: term_map{}
{
	term_map[0] = term{variable};
}

expr::expr(term t)
: term_map{}
{
	term_map[0] = term{t};
}


// Operations

expr expr::operator+(expr const & rhs) const
{
	auto degree_set = get_all_degrees(rhs);

	expr new_expr{};

	for (auto const degree : degree_set)
	{
		if (term_map.find(degree) == term_map.end())
			new_expr.term_map[degree] = rhs.term_map.at(degree);
		else if (rhs.term_map.find(degree) == rhs.term_map.end())
			new_expr.term_map[degree] = term_map.at(degree);
		else
			new_expr.term_map[degree] = term_map.at(degree) + rhs.term_map.at(degree);
	}

	return new_expr;
}

expr expr::operator-(expr const & rhs) const
{
	auto degree_set = get_all_degrees(rhs);

	expr new_expr{};

	for (auto const degree : degree_set)
	{
		if (term_map.find(degree) == term_map.end())
			new_expr.term_map[degree] = -rhs.term_map.at(degree);
		else if (rhs.term_map.find(degree) == rhs.term_map.end())
			new_expr.term_map[degree] = term_map.at(degree);
		else
			new_expr.term_map[degree] = term_map.at(degree) - rhs.term_map.at(degree);
	}

	return new_expr;
}


// Private Methods

std::set<int> expr::get_all_degrees(expr const & rhs) const
{
	std::set<int> degree_set{};

	for (auto const & elem : term_map)
		degree_set.insert(elem.first);

	for (auto const & elem : rhs.term_map)
		degree_set.insert(elem.first);

	return degree_set;
}


// Printing

std::ostream & operator<<(std::ostream &os, expr const &rhs)
{
	for (auto const &t : rhs.term_map)
	{
		os << t.second << ' ';
	}
	return os;
}


} // namespace computorv2