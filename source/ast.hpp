#pragma once
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <list>
#include <vector>

namespace ast
{
    namespace x3 = boost::spirit::x3;

	struct expression;
	struct parenthesis;
	struct used_function;

	struct assigned_function
	{
		std::string function_;
		std::string variable_;
	};

	struct operand : x3::variant<
		double
		, char
		, std::string
		, std::vector<std::vector<double>>
		, x3::forward_ast<expression>
		, x3::forward_ast<parenthesis>
		, x3::forward_ast<used_function>
	>
	{
		using base_type::base_type;
		using base_type::operator=;
	};

	struct operation
	{
		std::string operator_;
		operand operand_;
	};

	struct expression
	{
		expression() = default;
		expression(expression const &) = default;
		operand first;
		std::list<operation> rest;
	};

	struct parenthesis
	{
		expression expression_;
	};

	struct used_function
	{
		std::string function_;
		expression expression_;
	};

	struct variable_assignation
	{
		std::string variable_;
		expression expression_;
	};

	struct function_assignation
	{
		assigned_function function_;
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
		std::string
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

BOOST_FUSION_ADAPT_STRUCT(ast::used_function, function_, expression_)
BOOST_FUSION_ADAPT_STRUCT(ast::assigned_function, function_, variable_)
BOOST_FUSION_ADAPT_STRUCT(ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(ast::parenthesis, expression_)
BOOST_FUSION_ADAPT_STRUCT(ast::expression, first, rest)
BOOST_FUSION_ADAPT_STRUCT(ast::variable_assignation, variable_, expression_)
BOOST_FUSION_ADAPT_STRUCT(ast::function_assignation, function_, expression_)
BOOST_FUSION_ADAPT_STRUCT(ast::value_resolution, expression_)
BOOST_FUSION_ADAPT_STRUCT(ast::polynomial_resolution, left_expression, right_expression)