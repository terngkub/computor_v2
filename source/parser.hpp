#pragma once
#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

namespace grammar
{
	namespace x3 = boost::spirit::x3;

	using x3::char_;
	using x3::double_;
	using x3::string;
	using x3::lexeme;
	using x3::ascii::alpha;
	using x3::ascii::alnum;
	using x3::ascii::space;

	x3::rule<class rational, ast::rational> const rational("rational");
	x3::rule<class imaginary, ast::imaginary> const imaginary("imaginary");

	x3::rule<class matrix, ast::matrix> const matrix("matrix");
	x3::rule<class matrix_row, ast::matrix_row> const matrix_row("matrix_row");

	x3::rule<class name, ast::name> const name("name");
	x3::rule<class variable, ast::variable> const variable("variable");
	x3::rule<class coef_variable, ast::coef_variable> const coef_variable("coef_variable");
	x3::rule<class function, ast::function> const function("function");

	x3::rule<class expression, ast::expression> const expression("expression");
	x3::rule<class term, ast::expression> const term("term");
	x3::rule<class power, ast::expression> const power("power");
	x3::rule<class factor, ast::operand> const factor("factor");

	x3::rule<class variable_assignation, ast::variable_assignation> const variable_assignation("variable_assignation");
	x3::rule<class function_assignation, ast::function_assignation> const function_assignation("function_assignation");

	x3::rule<class value_resolution, ast::value_resolution> const value_resolution("value_resolution");
	x3::rule<class polynomial_resolution, ast::polynomial_resolution> const polynomial_resolution("polynomial_resolution");

	x3::rule<class command, ast::command> const command("command");
	x3::rule<class input, ast::input> const input("input");


	auto const rational_def = double_;
	auto const imaginary_def = char_('i');

	auto const matrix_def = '[' >> matrix_row % ';' >> ']';
	auto const matrix_row_def = '[' >> double_ % ',' >> ']';

	auto const name_def = alpha >> *(alnum);
	auto const variable_def = name;
	auto const coef_variable_def = double_ >> variable;
	auto const function_def = name >> '(' >> variable >> ')';

	auto const expression_def = term >> *((string("+") >> term | string("-") >> term));
	auto const term_def = power >> *((string("*") >> power) | (string("/") >> power) | (string("%") >> power));
	auto const power_def = factor >> *(string("^") >> factor);
	auto const factor_def = rational | imaginary | ('(' >> expression >> ')') | function | variable | matrix | ('-' >> factor);

	auto const variable_assignation_def = variable >> '=' >> expression;
	auto const function_assignation_def = function >> '=' >> expression;

	auto const value_resolution_def = expression >> '=' >> '?';
	auto const polynomial_resolution_def = expression >> '=' >> expression >> '?';

	auto const command_def = string("list variables") | string("exit");

	auto const input_def = command | variable_assignation | function_assignation | value_resolution | polynomial_resolution;

	BOOST_SPIRIT_DEFINE(rational);
	BOOST_SPIRIT_DEFINE(imaginary);
	BOOST_SPIRIT_DEFINE(matrix);
	BOOST_SPIRIT_DEFINE(matrix_row);
	BOOST_SPIRIT_DEFINE(name);
	BOOST_SPIRIT_DEFINE(variable);
	BOOST_SPIRIT_DEFINE(coef_variable);
	BOOST_SPIRIT_DEFINE(function);
	BOOST_SPIRIT_DEFINE(expression);
	BOOST_SPIRIT_DEFINE(term);
	BOOST_SPIRIT_DEFINE(power);
	BOOST_SPIRIT_DEFINE(factor);
	BOOST_SPIRIT_DEFINE(variable_assignation);
	BOOST_SPIRIT_DEFINE(function_assignation);
	BOOST_SPIRIT_DEFINE(value_resolution);
	BOOST_SPIRIT_DEFINE(polynomial_resolution);
	BOOST_SPIRIT_DEFINE(command);
	BOOST_SPIRIT_DEFINE(input);
}