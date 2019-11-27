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


// Checker

bool expr::is_matrix() const
{
	if (term_map.size() != 1)
		return false;
	if (term_map.find(0) == term_map.end())
		return false;
	if (!term_map.at(0).is_matrix())
		return false;
	return true;
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

expr expr::operator*(expr const & rhs) const
{
	expr new_expr{};

	for (auto const & left : term_map)
	{
		for (auto const & right : rhs.term_map)
		{
			auto new_degree = left.first + right.first;
			auto new_value = left.second * right.second;
			
			if (new_expr.term_map.find(new_degree) == new_expr.term_map.end() || (new_degree == 0 && new_expr.term_map[new_degree].coef.is_zero()))
				new_expr.term_map[new_degree] = new_value;
			else
				new_expr.term_map[new_degree] = new_expr.term_map[new_degree] + new_value;
		}
	}

	return new_expr;
}

expr expr::operator/(expr const & rhs) const
{

	if (rhs.term_map.size() > 1)
		throw std::runtime_error("expression can't be denominator");

	for (auto const & right : rhs.term_map)
	{
		if (!right.second.mt.empty())
			throw std::runtime_error("matrix can't be denominator");
	}
	
	expr new_expr{};

	if (rhs.term_map.find(0) != rhs.term_map.end())
	{
		for (auto const & left : term_map)
		{
			new_expr.term_map[left.first] = left.second / rhs.term_map.at(0);
		}
	}
	else
	{
		auto const & denom = *rhs.term_map.begin();
		for (auto const & left : term_map)
		{
			new_expr.term_map[left.first - denom.first] = left.second / denom.second;
		}
	}
	
	return new_expr;
}

expr expr::matrix_mul(expr const & rhs) const
{
	if (!is_matrix() || !rhs.is_matrix())
		throw std::runtime_error("expr matrix multiplication: both side isn't matrix");

	expr new_expr{};
	new_expr.term_map[0] = term_map.at(0).matrix_mul(rhs.term_map.at(0));
	return new_expr;
}

expr expr::substitute_variable(expr const & value)
{
	expr new_expr{};

	for (auto & term : term_map)
	{
		if (term.second.is_variable())
		{
			// TODO implement power function
			// power with degree
			term.second.variable = "";
			auto new_value = term.second * value;
			new_expr.push_back

			// power the input
			// time input with term
			// push_back to the return
		}
		new_expr.term_map[term.first] = term.second;
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