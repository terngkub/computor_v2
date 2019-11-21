#pragma once
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <list>
#include <vector>

namespace ast
{
    namespace x3 = boost::spirit::x3;

	using command = std::string;
	using rational = double;
	using imaginary = char;
	using matrix_row = std::vector<double>;
	using matrix = std::vector<matrix_row>;
	using name = std::string;
	using variable = name;

	struct coef_variable
	{
		double coef;
		variable variable_;
	};

	struct function
	{
		name function_;
		variable variable_;
	};

	struct expression;

	struct operand : x3::variant<
		rational
		, imaginary
		, function
		, variable
		, matrix
		, x3::forward_ast<expression>
	>
	{
		using base_type::base_type;
		using base_type::operator=;
	};

	struct operation
	{
		char operator_;
		operand operand_;
	};

	struct expression
	{
		operand first;
		std::list<operation> rest;
	};

	struct variable_assignation
	{
		variable variable_;
		expression expression_;
	};

	struct function_assignation
	{
		function function_;
		expression expression_;
	};

	struct value_resolution
	{
		expression expression_;
	};

	struct polynomial_resolution
	{
		expression left_expression;
		expression right_expression;
	};

	struct input : x3::variant<
		command
		, variable_assignation
		, function_assignation
		, value_resolution
		, polynomial_resolution
	>
	{
		using base_type::base_type;
		using base_type::operator=;
	};
}

BOOST_FUSION_ADAPT_STRUCT(ast::coef_variable, coef, variable_)
BOOST_FUSION_ADAPT_STRUCT(ast::function, function_, variable_)
BOOST_FUSION_ADAPT_STRUCT(ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(ast::expression, first, rest)
BOOST_FUSION_ADAPT_STRUCT(ast::variable_assignation, variable_, expression_)
BOOST_FUSION_ADAPT_STRUCT(ast::function_assignation, function_, expression_)
BOOST_FUSION_ADAPT_STRUCT(ast::value_resolution, expression_)
BOOST_FUSION_ADAPT_STRUCT(ast::polynomial_resolution, left_expression, right_expression)