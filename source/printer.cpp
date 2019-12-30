#include "printer.hpp"
#include <iostream>
#include <sstream>

namespace computorv2
{

std::string printer::operator()(double const & rational_) const
{
	std::stringstream ss;
	ss << rational_;
	return ss.str();
}

std::string printer::operator()(char const & imaginary_) const
{
	std::stringstream ss;
	ss << imaginary_;
	return ss.str();
}

std::string printer::operator()(std::vector<std::vector<double>> const & matrix_) const
{
	std::stringstream ss;

	ss << '[';

	for (auto rit = matrix_.cbegin(); rit < matrix_.cend(); ++rit)
	{
		ss << '[';

		for (auto cit = rit->cbegin(); cit < rit->cend(); ++cit)
		{
			ss << *cit;

			if (cit != rit->cend() - 1)
				ss << ", ";
		}

		ss << ']';

		if (rit != matrix_.cend() - 1)
			ss << "; ";
	}

	ss << ']';

	return ss.str();
}

std::string printer::operator()(std::string const & variable_) const
{
	std::stringstream ss;
	ss << variable_;
	return ss.str();
}

std::string printer::operator()(ast::assigned_function const & function_) const
{
	std::stringstream ss;
	ss << function_.function_ << '(' << function_.variable_ << ')';
	return ss.str();
}

std::string printer::operator()(ast::used_function const & function_) const
{
	std::stringstream ss;
	ss << function_.function_ << '(' << (*this)(function_.expression_) << ')';
	return ss.str();
}

std::string printer::operator()(ast::operation const & x) const
{
	std::stringstream ss;
	ss << (x.operator_ == "" ? "*" : x.operator_) << ' ';
	ss << boost::apply_visitor(*this, x.operand_);
	return ss.str();
}

std::string printer::operator()(ast::parenthesis const & x) const
{
	std::stringstream ss;
	ss << '(' << (*this)(x.expression_) << ')';
	return ss.str();
}

std::string printer::operator()(ast::negate const & x) const
{
	std::stringstream ss;
	ss << '-' << boost::apply_visitor(*this, x.operand_);
	return ss.str();
}

std::string printer::operator()(ast::expression const & x) const
{
	std::stringstream ss;
	ss << boost::apply_visitor(*this, x.first);
	for (auto const &oper : x.rest)
		ss << ' ' << (*this)(oper);
	return ss.str();
}

std::string printer::operator()(ast::variable_assignation const &x) const
{
	std::stringstream ss;
	ss << (*this)(x.variable_) << " = " << (*this)(x.expression_);
	return ss.str();
}

std::string printer::operator()(ast::function_assignation const & x) const
{
	std::stringstream ss;
	ss << (*this)(x.function_) << " = " << (*this)(x.expression_);
	return ss.str();
}

std::string printer::operator()(ast::value_resolution const & x) const
{
	std::stringstream ss;
	ss << (*this)(x.expression_) << " = ?";
	return ss.str();
}

std::string printer::operator()(ast::polynomial_resolution const & x) const
{
	std::stringstream ss;
	ss << (*this)(x.left_expression) << " = " << (*this)(x.right_expression) << " ?";
	return ss.str();
}

std::string printer::operator()(ast::input const & input_) const
{
	std::stringstream ss;
	ss << boost::apply_visitor(*this, input_) << std::endl;
	return ss.str();
}

} // namespace computor