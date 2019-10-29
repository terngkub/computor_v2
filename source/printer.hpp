#pragma once
#include "ast.hpp"

namespace computorv2
{

struct printer
{
	void operator()(client::ast::rational const &rational_) const;
	void operator()(client::ast::imaginary const &imaginary_) const;
	void operator()(client::ast::matrix const &matrix_) const;
	void operator()(client::ast::variable const &variable_) const;
	void operator()(client::ast::coef_variable const &coef_variable_) const;
	void operator()(client::ast::function const &function_) const;
	void operator()(client::ast::operation const &x) const;
	void operator()(client::ast::expression const &x) const;
	void operator()(client::ast::variable_assignation const &x) const;
	void operator()(client::ast::function_assignation const &x) const;
	void operator()(client::ast::value_resolution const &x) const;
	void operator()(client::ast::polynomial_resolution const &x) const;
	void operator()(client::ast::input const &input_) const;
};

} // namespace computorv2