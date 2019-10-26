#include "printer.hpp"
#include <iostream>

namespace client
{
namespace ast
{

void printer::operator()(rational const &rational_) const
{
	std::cout << rational_;
}

void printer::operator()(imaginary const &imaginary_) const
{
	std::cout << imaginary_;
}

void printer::operator()(matrix const &matrix_) const
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

void printer::operator()(variable const &variable_) const
{
	std::cout << variable_;
}

void printer::operator()(coef_variable const &coef_variable_) const
{
	std::cout << coef_variable_.coef << " * " << coef_variable_.variable_;
}

void printer::operator()(function const &function_) const
{
	std::cout << function_.function_ << '(' << function_.variable_ << ')';
}

void printer::operator()(operation const &x) const
{
	boost::apply_visitor(*this, x.operand_);
	std::cout << ' ' << x.operator_;
}

void printer::operator()(expression const &x) const
{
	boost::apply_visitor(*this, x.first);
	for (auto const &oper : x.rest)
	{
		std::cout << ' ';
		(*this)(oper);
	}
}

void printer::operator()(variable_assignation const &x) const
{
	(*this)(x.variable_);
	std::cout << " = ";
	(*this)(x.expression_);
}

void printer::operator()(function_assignation const &x) const
{
	(*this)(x.function_);
	std::cout << " = ";
	(*this)(x.expression_);
}

void printer::operator()(value_resolution const &x) const
{
	(*this)(x.expression_);
	std::cout << " = ?";
}

void printer::operator()(polynomial_resolution const &x) const
{
	(*this)(x.left_expression);
	std::cout << " = ";
	(*this)(x.right_expression);
	std::cout << " ?";
}

void printer::operator()(input const &input_) const
{
	boost::apply_visitor(*this, input_);
	std::cout << std::endl;
}

} // namespace ast
} // namespace client