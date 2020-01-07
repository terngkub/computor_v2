#include "expr.hpp"
#include <functional>
#include <iostream>
#include <sstream>
#include <variant>

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

expr::expr(std::vector<std::vector<complex>> matrix)
: _term_map{}
{
	_term_map[0] = term{matrix};
}

expr::expr(std::string variable)
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

	return var;
}

int expr::degree() const
{
	if (_term_map.size() != 1 || _term_map.find(0) == _term_map.end())
		throw std::runtime_error("degree can't contain unassigned variable");

	return _term_map.at(0).degree();
}

std::string expr::str() const
{
	std::stringstream ss;

	bool not_first = false;

	for (auto it = _term_map.crbegin(); it != _term_map.crend(); ++it)
	{
		if (not_first)
		{
			if (it->second.is_complex() && std::get<complex>(it->second.coef()).real() < 0)
			{
				ss << " - ";
				ss << -it->second;
			}
			else
			{
				ss << " + ";
				ss << it->second;
			}
		}
		else
		{
			not_first = true;
			ss << it->second;
		}
		
		if (it->first != 0 && it->first != 1)
			ss << "^" << it->first;
	}
	return ss.str();
}


// Checkers

bool expr::is_coef() const
{
	return _term_map.size() == 1 && _term_map.find(0) != _term_map.end();
}

bool expr::is_matrix() const
{
	return is_coef() && _term_map.at(0).is_matrix();
}

bool expr::is_zero() const
{
	return is_coef() && _term_map.at(0).is_zero();
}


// Operations

static expr operation_add_sub(expr const & lhs, expr const & rhs, std::function<term (term const &, term const &)> add_or_sub)
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
		term value;
		if (lhs_term_map.find(degree) == lhs_term_map.end())
			value = add_or_sub(term{}, rhs_term_map.at(degree));
		else if (rhs_term_map.find(degree) == rhs_term_map.end())
			value = lhs_term_map.at(degree);
		else
			value = add_or_sub(lhs_term_map.at(degree), rhs_term_map.at(degree));

		if (!value.is_zero())
			new_term_map[degree] = value;
	}

	if (new_term_map.size() == 0)
		new_term_map[0] = term{};

	return expr{std::move(new_term_map)};
}

expr operator+(expr const & lhs, expr const & rhs)
{
	return operation_add_sub(lhs, rhs, [](term const & a, term const & b){ return a + b; });
}

expr operator-(expr const & lhs, expr const & rhs)
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
			auto degree = left.first + right.first;
			auto value = left.second * right.second;

			auto new_term = (value.is_zero() || (degree == 0 && value.variable() != "")) ? term{value.coef()} : value;
			
			if (new_term_map.find(degree) == new_term_map.end())
				new_term_map[degree] = new_term;
			else
				new_term_map[degree] = new_term_map[degree] + new_term;
		}
	}

	return expr{std::move(new_term_map)};
}

static expr operation_div_mod(expr const & lhs, expr const & rhs, std::function<term (term const &, term const &)> func)
{
	auto rhs_term_map = rhs.term_map();

	if (rhs_term_map.size() > 1)
		throw std::runtime_error("expression can't be denominator");

	auto lhs_term_map = lhs.term_map();
	std::map<int, term> new_term_map{};
	auto const & denom = *rhs_term_map.begin();

	for (auto const & left : lhs_term_map)
	{
		auto degree = left.first - denom.first;
		auto value = func(left.second, denom.second);
		new_term_map[degree] = (degree == 0 && value.variable() != "") ? term{value.coef()} : value;
	}

	return expr{std::move(new_term_map)};
}

expr operator/(expr const & lhs, expr const & rhs)
{
	return operation_div_mod(lhs, rhs, [](term const & a, term const & b){ return a / b; });
}

expr operator%(expr const & lhs, expr const & rhs)
{
	return operation_div_mod(lhs, rhs, [](term const & a, term const & b){ return a % b; });
}

// note test 0^10
expr operator^(expr const & lhs, expr const & rhs)
{
	int deg = rhs.degree();
	
	if (deg == 1)
		return lhs;

	if (deg == 0)
	{
		if (lhs.is_matrix())
		{
			auto m = std::get<matrix>(lhs.term_map().at(0).coef());
			auto row_nb = m.row_nb();
			auto col_nb = m.col_nb();
			return expr{std::vector<std::vector<complex>>(row_nb, std::vector<complex>(col_nb, 1))};
		}
		return expr{complex{1, 0}};
	}

	std::map<int, term> new_term_map{};

	for (int i = 1; i < deg; ++i)
	{
		for (auto const & left : lhs.term_map())
		{
			for (auto const & right : lhs.term_map())
			{
				auto new_degree = left.first + right.first;
				auto mul_result = left.second * right.second;

				if (new_term_map.find(new_degree) == new_term_map.end())
					new_term_map[new_degree] = mul_result;
				else
					new_term_map[new_degree] = new_term_map[new_degree] + mul_result;
			}
		}
	}

	return expr{std::move(new_term_map)};
}

expr expr_matrix_mul(expr const & lhs, expr const & rhs)
{
	if (!lhs.is_matrix() || !rhs.is_matrix())
		throw std::runtime_error("expr matrix multiplication: both side isn't matrix");

	std::map<int, term> new_term_map{};

	new_term_map[0] = term_matrix_mul(lhs.term_map().at(0), rhs.term_map().at(0));

	return expr{std::move(new_term_map)};
}

std::ostream & operator<<(std::ostream &os, expr const &rhs)
{
	os << rhs.str();
	return os;
}


} // namespace computorv2