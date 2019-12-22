#pragma once
#include "ast.hpp"

namespace computorv2
{

struct printer
{
	void operator()(double const &rational_) const;
	void operator()(char const &imaginary_) const;
	void operator()(std::vector<std::vector<double>> const &matrix_) const;
	void operator()(std::string const &variable_) const;
	void operator()(ast::coef_variable const &coef_variable_) const;
	void operator()(ast::assigned_function const &function_) const;
	void operator()(ast::used_function const &function_) const;
	void operator()(ast::operation const &x) const;
	void operator()(ast::expression const &x) const;
	void operator()(ast::variable_assignation const &x) const;
	void operator()(ast::function_assignation const &x) const;
	void operator()(ast::value_resolution const &x) const;
	void operator()(ast::polynomial_resolution const &x) const;
	void operator()(ast::input const &input_) const;
};

} // namespace computorv2