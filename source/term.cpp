#include "math.hpp"
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
    : _coef{complex{1, 0}}
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

    if (is_complex())
    {
        complex c_coef = std::get<complex>(_coef);
        if (has_variable())
        {
            if (std::get<complex>(_coef) != 1)
                ss << c_coef;
            ss << _variable;
        }
        else
        {
            ss << c_coef;
        }
    }
    else
    {
        ss << std::get<matrix>(_coef);
        if (has_variable())
            ss << " * " << _variable;
    }

	return ss.str();
}

int term::degree() const
{
    if (is_matrix())
        throw std::runtime_error("degree can't be matrix");

    if (has_variable())
        throw std::runtime_error("degree can't be variable");
    
    auto complex_deg = std::get<complex>(_coef);

    if (complex_deg.is_complex())
        throw std::runtime_error("degree can't be complex number");
        
    auto deg = complex_deg.real();
    
    if (math::fmod(deg, 1.0) != 0.0)
        throw std::runtime_error("degree isn't an integer");

    if (deg < 0)
        throw std::runtime_error("degree is negative");

    return deg;
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