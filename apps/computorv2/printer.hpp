#pragma once
#include "ast.hpp"

namespace client
{
namespace ast
{

struct printer
{
	void operator()(rational const &rational_) const;
	void operator()(imaginary const &imaginary_) const;
	void operator()(matrix const &matrix_) const;
	void operator()(variable const &variable_) const;
	void operator()(coef_variable const &coef_variable_) const;
	void operator()(function const &function_) const;
	void operator()(operation const &x) const;
	void operator()(expression const &x) const;
	void operator()(variable_assignation const &x) const;
	void operator()(function_assignation const &x) const;
	void operator()(value_resolution const &x) const;
	void operator()(polynomial_resolution const &x) const;
	void operator()(input const &input_) const;
};

} // namespace ast
} // namespace client