#include "term.hpp"

namespace computorv2
{

// Constructors

term::term()
	: coef{complex{0, 0}}
    , variable{""}
    , mt{}
{}

term::term(complex nb)
	: coef{nb}
    , variable{""}
    , mt{}
{}

term::term(ast::variable variable)
	: coef{complex{1, 0}}
    , variable{variable}
    , mt{}
{}

term::term(matrix mt)
	: coef{complex{1, 0}}
    , variable{""}
    , mt{mt}
{}

term::term(complex const & nb, ast::variable const & variable, matrix const & mt)
	: coef{nb}
    , variable{variable}
    , mt{mt}
{}


// Checker

bool term::is_matrix() const
{
    return !mt.empty();
}

// Operations

term term::operator+(term const & rhs) const
{
    // both matrix
    if (!mt.empty() && !rhs.mt.empty())
        return term{mt.matrix_add(rhs.mt)};

    // one matrix, on scalar
    if (!mt.empty() || !rhs.mt.empty())
        throw std::runtime_error("invalid operation between scalar and matrix");

    // both scalar
    auto new_coef = coef + rhs.coef;
    return term{new_coef, variable, mt};
}

term term::operator-(term const & rhs) const
{
    // both matrix
    if (!mt.empty() && !rhs.mt.empty())
        return term{mt.matrix_sub(rhs.mt)};

    // one matrix, on scalar
    if (!mt.empty() || !rhs.mt.empty())
        throw std::runtime_error("invalid operation between scalar and matrix");

    // both scalar
    auto new_coef = coef - rhs.coef;
    return term{new_coef, variable, mt};
}

term term::operator*(term const & rhs) const
{
    // both matrix
    if (!mt.empty() && !rhs.mt.empty())
        throw std::runtime_error("term multiplication: both sides are matrix");

    // left = matrix, right = scalar
    if (!mt.empty() && rhs.mt.empty())
        return term{mt.scalar_mul(rhs.coef)};

    // left = scalar, right = matrix
    if (!mt.empty() && rhs.mt.empty())
        return term{rhs.mt.scalar_mul(coef)};

    // both scalar
    auto new_coef = coef * rhs.coef;
    return term{new_coef, variable, mt};
}

term term::operator/(term const & rhs) const
{
    // right = matrix
    if (!rhs.mt.empty())
        throw std::runtime_error("matrix can't be denominator");

    // left = matrix, right = scalar
    if (!mt.empty())
        return term{mt.scalar_div(rhs.coef)};

    // both scalar
    auto new_coef = coef / rhs.coef;
    return term{new_coef, variable, mt};
}

/*
term term::operator%(term const & rhs) const
{
    // TODO
    return term{};
}
*/

term term::operator-() const
{
    // TODOo
    // matrix
    if (!mt.empty())
        return term{mt.scalar_mul(complex{-1, 0})};

    return term{-coef};
}

term term::matrix_mul(term const & rhs) const
{
	if (mt.empty() || rhs.mt.empty())
		throw std::runtime_error("matrix multiplication: both side isn't matrix");
	return term{mt.matrix_mul(rhs.mt)};
}



// Printing

std::ostream &operator<<(std::ostream & os, term const & rhs)
{
    if (rhs.coef.is_zero())
    {
        os << '0';
        return os;
    }

    if (rhs.coef.is_complex() && (!rhs.mt.empty() || !rhs.variable.empty()))
        os << '(' << rhs.coef << ')';
    else
	    os << rhs.coef;

    if (!rhs.mt.empty())
        os << " * " << rhs.mt;

    if (!rhs.variable.empty())
        os << " * " << rhs.variable;

	return os;
}

}