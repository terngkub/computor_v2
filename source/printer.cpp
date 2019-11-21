#include "printer.hpp"
#include <iostream>

namespace computorv2
{

void printer::operator()(ast::rational const &rational_) const
{
	std::cout << rational_;
}

void printer::operator()(ast::imaginary const &imaginary_) const
{
	std::cout << imaginary_;
}

void printer::operator()(ast::matrix const &matrix_) const
{
	for (auto itr = matrix_.begin(); itr < matrix_.end(); ++itr)
	{
		std::cout << '|';
		for (auto itc = itr->begin(); itc < itr->end(); ++itc)
		{
			std::cout << *itc;
			if (itc != itr->end() - 1)
				std::cout << ' ';
		}
		std::cout << '|';
		if (itr != matrix_.end() - 1)
			std::cout << '\n';
	}
}

void printer::operator()(ast::variable const &variable_) const
{
	std::cout << variable_;
}

void printer::operator()(ast::coef_variable const &coef_variable_) const
{
	std::cout << coef_variable_.coef << " * " << coef_variable_.variable_;
}

void printer::operator()(ast::function const &function_) const
{
	std::cout << function_.function_ << '(' << function_.variable_ << ')';
}

void printer::operator()(ast::operation const &x) const
{
	boost::apply_visitor(*this, x.operand_);
	std::cout << ' ' << x.operator_;
}

void printer::operator()(ast::expression const &x) const
{
	boost::apply_visitor(*this, x.first);
	for (auto const &oper : x.rest)
	{
		std::cout << ' ';
		(*this)(oper);
	}
}

void printer::operator()(ast::variable_assignation const &x) const
{
	(*this)(x.variable_);
	std::cout << " = ";
	(*this)(x.expression_);
}

void printer::operator()(ast::function_assignation const &x) const
{
	(*this)(x.function_);
	std::cout << " = ";
	(*this)(x.expression_);
}

void printer::operator()(ast::value_resolution const &x) const
{
	(*this)(x.expression_);
	std::cout << " = ?";
}

void printer::operator()(ast::polynomial_resolution const &x) const
{
	(*this)(x.left_expression);
	std::cout << " = ";
	(*this)(x.right_expression);
	std::cout << " ?";
}

void printer::operator()(ast::input const &input_) const
{
	boost::apply_visitor(*this, input_);
	std::cout << std::endl;
}

} // namespace computor