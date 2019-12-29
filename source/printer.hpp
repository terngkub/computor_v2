#pragma once
#include "ast.hpp"

namespace computorv2
{

struct printer : public boost::static_visitor<std::string>
{
	std::string operator()(double const &rational_) const;
	std::string operator()(char const &imaginary_) const;
	std::string operator()(std::vector<std::vector<double>> const &matrix_) const;
	std::string operator()(std::string const &variable_) const;
	std::string operator()(ast::assigned_function const &function_) const;
	std::string operator()(ast::used_function const &function_) const;
	std::string operator()(ast::operation const &x) const;
	std::string operator()(ast::parenthesis const &x) const;
	std::string operator()(ast::negate const & x) const;
	std::string operator()(ast::expression const &x) const;
	std::string operator()(ast::variable_assignation const &x) const;
	std::string operator()(ast::function_assignation const &x) const;
	std::string operator()(ast::value_resolution const &x) const;
	std::string operator()(ast::polynomial_resolution const &x) const;
	std::string operator()(ast::input const &input_) const;
};

} // namespace computorv2