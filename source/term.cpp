#include "term.hpp"

namespace computorv2
{

// Constructors

term::term()
	: coef{complex{0, 0}}
    , variable{""}
    , matrix{}
{}

term::term(complex nb)
	: coef{nb}
    , variable{""}
    , matrix{}
{}

term::term(ast::variable variable)
	: coef{complex{1, 0}}
    , variable{variable}
    , matrix{}
{}

term::term(Matrix matrix)
	: coef{complex{1, 0}}
    , variable{""}
    , matrix{matrix}
{}

term::term(complex const & nb, ast::variable const & variable, Matrix const & matrix)
	: coef{nb}
    , variable{variable}
    , matrix{matrix}
{}


void term::check_validity(term const & rhs) const
{
    if (variable != rhs.variable)
        throw std::runtime_error("multiple variables");

    if (!matrix.empty() || !rhs.matrix.empty())
        throw std::runtime_error("invalid operation between scalar and matrix");
}


// Operations

term term::operator+(term const & rhs) const
{
    if (!matrix.empty() && !rhs.matrix.empty())
        return term{matrix + rhs.matrix};

    check_validity(rhs);
    auto new_coef = coef + rhs.coef;
    return term{new_coef, variable, matrix};
}

term term::operator-(term const & rhs) const
{
    if (!matrix.empty() && !rhs.matrix.empty())
        return term{matrix - rhs.matrix};

    check_validity(rhs);
    auto new_coef = coef - rhs.coef;
    return term{new_coef, variable, matrix};
}

term term::operator*(term const & rhs) const
{
    if (!matrix.empty() && !rhs.matrix.empty())
        throw std::runtime_error("scalar multiplication with matrix on left and right side");

    // m * m error
    // m * c ok
    // c * m ok
    // c * c ok
    
    return term{};
}

term term::operator/(term const & rhs) const
{
    // m / m error
    // m / c ok
    // c / m error
    // c / c ok

    return term{};
}

term term::operator%(term const & rhs) const
{
    return term{};
}

term term::operator-() const
{
    return term{};
}



// Printing

std::ostream &operator<<(std::ostream & os, term const & rhs)
{
    if (rhs.coef.is_zero())
    {
        os << '0';
        return os;
    }

    if (rhs.coef.is_complex() && (!rhs.matrix.empty() || !rhs.variable.empty()))
        os << '(' << rhs.coef << ')';
    else
	    os << rhs.coef;

    if (!rhs.matrix.empty())
        os << " * " << rhs.matrix;

    if (!rhs.variable.empty())
        os << " * " << rhs.variable;

	return os;
}

}