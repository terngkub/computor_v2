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
	using x3::lit;
	using x3::ascii::alpha;
	using x3::ascii::alnum;
	using x3::ascii::space;

	x3::rule<class rational, double> const rational("rational");
	x3::rule<class imaginary, char> const imaginary("imaginary");
	x3::rule<class matrix, std::vector<std::vector<ast::expression>>> const matrix("matrix");
	x3::rule<class matrix_row, std::vector<ast::expression>> const matrix_row("matrix_row");

	x3::rule<class name, std::string> const name("name");
	x3::rule<class variable, std::string> const variable("variable");
	x3::rule<class assigned_function, ast::assigned_function> const assigned_function("assigned function");
	x3::rule<class used_function, ast::used_function> const used_function("used function");

	x3::rule<class expression, ast::expression> const expression("expression");
	x3::rule<class term, ast::expression> const term("term");
	x3::rule<class power, ast::expression> const power("power");
	x3::rule<class coef, ast::expression> const coef("coef");
	x3::rule<class all_factor, ast::operand> const all_factor("all_factor");
	x3::rule<class pos_factor, ast::operand> const pos_factor("pos_factor");
	x3::rule<class factor, ast::operand> const factor("factor");
	x3::rule<class parenthesis, ast::parenthesis> const parenthesis("parenthesis");
	x3::rule<class negate, ast::negate> const negate("negate");

	x3::rule<class variable_assignation, ast::variable_assignation> const variable_assignation("variable_assignation");
	x3::rule<class function_assignation, ast::function_assignation> const function_assignation("function_assignation");
	x3::rule<class value_resolution, ast::value_resolution> const value_resolution("value_resolution");
	x3::rule<class polynomial_resolution, ast::polynomial_resolution> const polynomial_resolution("polynomial_resolution");
	x3::rule<class command, std::string> const command("command");
	x3::rule<class input, ast::input> const input("input");


	auto const rational_def = !lit("+") >> double_;
	auto const imaginary_def = !lit("ii") >> (char_('i') | char_('I'));

	auto const matrix_def = '[' >> matrix_row % ';' >> ']';
	auto const matrix_row_def = '[' >> expression % ',' >> ']';

	auto const name_def = alpha >> *(alnum);
	auto const variable_def = name;
	auto const assigned_function_def = name >> '(' >> variable >> ')';
	auto const used_function_def = name >> '(' >> expression >> ')';

	auto const parenthesis_def = '(' >> expression >> ')';
	auto const expression_def = term >> *((string("+") >> term | string("-") >> term));
	auto const term_def = coef >> *(((string("**") >> coef) | string("*") >> coef) | (string("/") >> coef) | (string("%") >> coef));
	auto const coef_def = all_factor >> *(string("") >> pos_factor);

	auto const all_factor_def = power | negate;
	auto const pos_factor_def = !lit('-') >> power;

	auto const negate_def = '-' >> all_factor;

	auto const power_def = factor >> *(string("^") >> factor);
	auto const factor_def = rational | parenthesis | used_function | imaginary | variable | matrix;

	auto const variable_assignation_def = variable >> '=' >> expression;
	auto const function_assignation_def = assigned_function >> '=' >> expression;
	auto const value_resolution_def = expression >> '=' >> '?';
	auto const polynomial_resolution_def = expression >> '=' >> expression >> '?';
	auto const command_def = string("help") | string("list_variables") | string("list_functions") | string("history") | string("exit");
	auto const input_def = command | polynomial_resolution | variable_assignation | function_assignation | value_resolution;

	BOOST_SPIRIT_DEFINE(rational);
	BOOST_SPIRIT_DEFINE(imaginary);
	BOOST_SPIRIT_DEFINE(matrix);
	BOOST_SPIRIT_DEFINE(matrix_row);

	BOOST_SPIRIT_DEFINE(name);
	BOOST_SPIRIT_DEFINE(variable);
	BOOST_SPIRIT_DEFINE(assigned_function);
	BOOST_SPIRIT_DEFINE(used_function);

	BOOST_SPIRIT_DEFINE(expression);
	BOOST_SPIRIT_DEFINE(term);
	BOOST_SPIRIT_DEFINE(power);
	BOOST_SPIRIT_DEFINE(coef);
	BOOST_SPIRIT_DEFINE(all_factor);
	BOOST_SPIRIT_DEFINE(pos_factor);
	BOOST_SPIRIT_DEFINE(factor);
	BOOST_SPIRIT_DEFINE(parenthesis);
	BOOST_SPIRIT_DEFINE(negate);

	BOOST_SPIRIT_DEFINE(variable_assignation);
	BOOST_SPIRIT_DEFINE(function_assignation);
	BOOST_SPIRIT_DEFINE(value_resolution);
	BOOST_SPIRIT_DEFINE(polynomial_resolution);
	BOOST_SPIRIT_DEFINE(command);
	BOOST_SPIRIT_DEFINE(input);
}