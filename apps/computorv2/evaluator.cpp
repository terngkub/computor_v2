#include "evaluator.hpp"
#include <iostream>

namespace computorv2
{

void evaluator::operator()(client::ast::input x)
{
    boost::apply_visitor(*this, x);
}

void evaluator::operator()(client::ast::variable_assignation x)
{
    std::cout << "variable_assignation\n";
}

void evaluator::operator()(client::ast::function_assignation x)
{
    std::cout << "function_assignation\n";
}

void evaluator::operator()(client::ast::value_resolution x)
{
    std::cout << "value_resolution\n";
}

void evaluator::operator()(client::ast::polynomial_resolution x)
{
    std::cout << "polynomial_resolution\n";
}

void evaluator::operator()(client::ast::expression x)
{
    std::cout << "expression\n";
}

}