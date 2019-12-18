#include "term.hpp"
#include <sstream>

namespace computorv2
{

// Constructors

term::term()
	: _coef{complex{}}
    , _variable{""}
{}

term::term(std::variant<complex, matrix> const & val)
    : _coef{val}
    , _variable{""}
{}

term::term(std::string var)
    : _coef{complex{}}
    , _variable{var}
{}

term::term(std::variant<complex, matrix> const & val, std::string var)
    : _coef{val}
    , _variable{var}
{}


// Getters

std::variant<complex, matrix> term::coef() const
{
    return _coef;
}

std::string term::variable() const
{
    return _variable;
}

std::string term::str() const
{
    std::stringstream ss;

    std::visit([&ss](auto const & val){ ss << val; }, _coef);
    
    if (has_variable())
        ss << " * " << _variable;

	return ss.str();
}


// Checkers

bool term::is_complex() const
{
    return std::holds_alternative<complex>(_coef);
}

bool term::is_matrix() const
{
    return std::holds_alternative<matrix>(_coef);
}

bool term::has_variable() const
{
    return !_variable.empty();
}

bool term::is_zero() const
{
    return is_complex() && std::get<complex>(_coef) == 0;
}

bool term::is_valid_degree() const
{
    return !has_variable() && is_complex() && std::get<complex>(_coef) % 1 == 0.0;
    // TODO check if more than zero
}


// Operations

static term term_operation(term const & lhs, term const & rhs, std::variant<complex, matrix> && new_coef)
{
    if (lhs.has_variable() && rhs.has_variable() && lhs.variable() != rhs.variable())
        throw std::runtime_error("more than one unassigned variables");

    auto new_variable = lhs.has_variable() ? lhs.variable() : (rhs.has_variable() ? rhs.variable() : "");

    return term{new_coef, new_variable};
}

term operator+(term const & lhs, term const & rhs)
{
    auto new_coef = std::visit( [](auto const & a, auto const & b)->std::variant<complex, matrix>{ return a + b; }, lhs.coef(), rhs.coef());
    return term_operation(lhs, rhs, std::move(new_coef));
}

term operator-(term const & lhs, term const & rhs)
{
    auto new_coef = std::visit( [](auto const & a, auto const & b)->std::variant<complex, matrix>{ return a - b; }, lhs.coef(), rhs.coef());
    return term_operation(lhs, rhs, std::move(new_coef));
}

term operator*(term const & lhs, term const & rhs)
{
    auto new_coef = std::visit( [](auto const & a, auto const & b)->std::variant<complex, matrix>{ return a * b; }, lhs.coef(), rhs.coef());
    return term_operation(lhs, rhs, std::move(new_coef));
}

term operator/(term const & lhs, term const & rhs)
{
    auto new_coef = std::visit( [](auto const & a, auto const & b)->std::variant<complex, matrix>{ return a / b; }, lhs.coef(), rhs.coef());
    return term_operation(lhs, rhs, std::move(new_coef));
}

term operator%(term const & lhs, term const & rhs)
{
    auto new_coef = std::visit( [](auto const & a, auto const & b)->std::variant<complex, matrix>{ return a % b; }, lhs.coef(), rhs.coef());
    return term_operation(lhs, rhs, std::move(new_coef));
}

term operator-(term const & rhs)
{
    std::variant<complex, matrix> new_coef = std::visit([](auto const & a)->std::variant<complex, matrix>{ return -a; }, rhs.coef());
    return term{new_coef, rhs.variable()};
}

std::ostream &operator<<(std::ostream & os, term const & rhs)
{
    os << rhs.str();
    return os;
}


// Other Operations

term term_matrix_mul(term const & lhs, term const & rhs)
{
	if (lhs.is_matrix() || rhs.is_matrix())
		throw std::runtime_error("matrix multiplication: both side isn't matrix");
	return term{mt_mul(std::get<matrix>(lhs.coef()), std::get<matrix>(rhs.coef()))};
}

}