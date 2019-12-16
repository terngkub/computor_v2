#include "expr.hpp"

namespace computorv2
{

// Constructors

expr::expr()
: _term_map{}
{
	_term_map[0] = term{};
}

expr::expr(complex nb)
: _term_map{}
{
	_term_map[0] = term{nb};
}

expr::expr(ast::matrix matrix)
: _term_map{}
{
	_term_map[0] = term{matrix};
}

expr::expr(ast::variable variable)
: _term_map{}
{
	_term_map[1] = term{variable};
}

expr::expr(term t)
: _term_map{}
{
	_term_map[0] = term{t};
}

expr::expr(std::map<int, term> && tm)
: _term_map{tm}
{}


// Getters

std::map<int, term> const & expr::term_map() const
{
	return _term_map;
}

// TODO double check
std::string expr::variable() const
{
	std::string var{};
	for (auto const & term : _term_map)
	{
		if (term.second.variable() != "")
		{
			if (var != "")
			{
				if (term.second.variable() != var)
					throw std::runtime_error("get_variable: multiple variables in one expression");
			}
			else
				var = term.second.variable();
		}
	}

	if (var == "")
		throw std::runtime_error("get_variable: no variable in the expression");

	return var;
}

std::string expr::str() const
{
	std::stringstream ss;

	for (auto const &t : _term_map)
	{
		ss << t.second << ' ';
	}
	return ss.str();
}


// Checkers

bool expr::is_value() const
{
	return _term_map.size() == 1 && _term_map.find(0) != _term_map.end();
}

bool expr::is_matrix() const
{
	return is_value() && _term_map.at(0).is_matrix();
}

bool expr::is_valid_degree() const
{
	return is_value() && _term_map.at(0).is_valid_degree();
}

bool expr::is_zero() const
{
	return is_value() && _term_map.at(0).is_zero();
}


// Operations

expr operation_add_sub(expr const & lhs, expr const & rhs, std::function<term (term const &, term const &)> add_or_sub)
{
	auto const & lhs_term_map = lhs.term_map();
	auto const & rhs_term_map = rhs.term_map();

	std::set<int> degree_set{};

	for (auto const & elem : lhs_term_map)
		degree_set.insert(elem.first);
	for (auto const & elem : rhs_term_map)
		degree_set.insert(elem.first);

	std::map<int, term> new_term_map{};

	for (auto const degree : degree_set)
	{
		if (lhs_term_map.find(degree) == lhs_term_map.end())
			new_term_map[degree] = add_or_sub(term{}, rhs_term_map.at(degree));
		else if (rhs_term_map.find(degree) == rhs_term_map.end())
			new_term_map[degree] = lhs_term_map.at(degree);
		else
			new_term_map[degree] = add_or_sub(lhs_term_map.at(degree), rhs_term_map.at(degree));
	}

	return expr{std::move(new_term_map)};
}

expr operator+(expr const & lhs, expr const & rhs)
{
	return operation_add_sub(lhs, rhs, [](term const & a, term const & b){ return a + b; });
}

expr operator+(expr const & lhs, expr const & rhs)
{
	return operation_add_sub(lhs, rhs, [](term const & a, term const & b){ return a - b; });
}

expr operator*(expr const & lhs, expr const & rhs)
{
	std::map<int, term> new_term_map;

	for (auto const & left : lhs.term_map())
	{
		for (auto const & right : rhs.term_map())
		{
			auto new_degree = left.first + right.first;
			auto new_value = left.second * right.second;
			
			if (new_term_map.find(new_degree) == new_term_map.end() || (new_degree == 0 && new_term_map[new_degree].is_zero()))
				new_term_map[new_degree] = new_value;
			else
				new_term_map[new_degree] = new_term_map[new_degree] + new_value;
		}
	}

	return expr{std::move(new_term_map)};
}

expr operator/(expr const & lhs, expr const & rhs)
{
	auto rhs_term_map = rhs.term_map();

	if (rhs_term_map.size() > 1)
		throw std::runtime_error("expression can't be denominator");

	auto lhs_term_map = lhs.term_map();
	std::map<int, term> new_term_map{};
	auto const & denom = *rhs_term_map.begin();

	for (auto const & left : lhs_term_map)
		new_term_map[left.first - denom.first] = left.second / denom.second;

	return expr{std::move(new_term_map)};
}

// TODO
expr operator%(expr const & lhs, expr const & rhs)
{
	auto rhs_term_map = rhs.term_map();

	if (rhs_term_map.size() > 1)
		throw std::runtime_error("expression can't be denominator");

	auto lhs_term_map = lhs.term_map();
	std::map<int, term> new_term_map{};
	auto const & denom = *rhs_term_map.begin();

	for (auto const & left : lhs_term_map)
		new_term_map[left.first - denom.first] = left.second / denom.second;

	return expr{std::move(new_term_map)};
}

// TODO
expr expr::operator^(expr const & rhs) const
{
	if (!rhs.is_valid_degree())
		throw std::runtime_error("degree isn't positive number");

	if (rhs.is_zero())
		return expr{complex{1, 0}};

	int degree = std::get<complex>(rhs.term_map.at(0).value()).real();

	if (degree == 1)
		return *this;

	expr ret_expr{*this};
	expr tmp_expr{};

	for (int i = 1; i < degree; ++i)
	{
		for (auto const & left : ret_expr.term_map)
		{
			for (auto const & right : term_map)
			{
				auto degree = left.first + right.first;
				if (tmp_expr.term_map.find(degree) == tmp_expr.term_map.end())
					tmp_expr.term_map[degree] = left.second * right.second;
				else
					tmp_expr.term_map[degree] = tmp_expr.term_map[degree] + (left.second * right.second);
			}
		}
		ret_expr = tmp_expr;
		tmp_expr = expr{};
	}

	return ret_expr;
}

// TODO
expr expr::matrix_mul(expr const & rhs) const
{
	if (!is_matrix() || !rhs.is_matrix())
		throw std::runtime_error("expr matrix multiplication: both side isn't matrix");

	expr new_expr{};
	new_expr.term_map[0] = term_matrix_mul(term_map.at(0), rhs.term_map.at(0));
	return new_expr;
}

std::ostream & operator<<(std::ostream &os, expr const &rhs)
{
	os << rhs.str();
	return os;
}


} // namespace computorv2