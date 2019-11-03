#include "expr.hpp"
#include <iostream>

namespace computorv2
{

// term

term::term()
    : coef{number{0, 0}}
    , variable{""}
    , matrix{}
    , has_matrix{false}
{}

term::term(number nb)
    : coef{nb}
    , variable{""}
    , matrix{}
    , has_matrix{false}
{}

term::term(client::ast::matrix matrix)
    : coef{number{1, 0}}
    , variable{""}
    , matrix{matrix}
    , has_matrix{true}
{}

term::term(client::ast::variable variable)
    : coef{number{1, 0}}
    , variable{variable}
    , matrix{}
    , has_matrix{false}
{}

term term::operator+(term const & rhs) const
{
    number nb{this->coef.rational + rhs.coef.rational, this->coef.imaginary + rhs.coef.imaginary};
    return term{nb};
}

std::ostream & operator<<(std::ostream & os, term const & rhs)
{
    std::cout << rhs.coef.rational << " + " << rhs.coef.imaginary;
    return os;
}


// epxr

expr::expr() : term_map()
{
    term_map[0] = term{};
}

expr::expr(number nb) : term_map()
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

expr expr::operator+(expr const & rhs) const
{
    auto new_term = this->term_map.at(0) + rhs.term_map.at(0);
    return expr{new_term};
}

std::ostream & operator<<(std::ostream & os, expr const & rhs)
{
    for(auto const & t : rhs.term_map)
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
    number nb{rational, 0};
    return expr{nb};
}

expr expr_factory::operator()(client::ast::imaginary imaginary)
{
    number nb{0, 1};
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
	for(auto it = variable_map.cbegin(); it != variable_map.cend(); ++it)
	{
		std::cout << it->first << " = " << it->second << '\n';
	}
}

void expr_factory::print_functions() const
{
    // TODO add this
}

}