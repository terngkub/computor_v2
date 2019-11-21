#pragma once
#include "ast.hpp"

namespace computorv2
{

struct printer
{
	void operator()(ast::rational const &rational_) const;
	void operator()(ast::imaginary const &imaginary_) const;
	void operator()(ast::matrix const &matrix_) const;
	void operator()(ast::variable const &variable_) const;
	void operator()(ast::coef_variable const &coef_variable_) const;
	void operator()(ast::function const &function_) const;
	void operator()(ast::operation const &x) const;
	void operator()(ast::expression const &x) const;
	void operator()(ast::variable_assignation const &x) const;
	void operator()(ast::function_assignation const &x) const;
	void operator()(ast::value_resolution const &x) const;
	void operator()(ast::polynomial_resolution const &x) const;
	void operator()(ast::input const &input_) const;
};

} // namespace computorv2