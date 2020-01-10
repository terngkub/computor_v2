#include "evaluator.hpp"
#include "math.hpp"
#include <functional>
#include <iostream>
#include <sstream>

namespace computorv2
{

// Function Object

std::string evaluator::operator()(ast::input inp, std::string str)
{
	auto ret = boost::apply_visitor(*this, inp);
	if (boost::get<std::string>(&inp) == nullptr)
	{
		history_list.push_back("> " + str + "\n    " + ret);
	}
	return ret; 
}

std::string evaluator::operator()(std::string x)
{
	if (x == "help")
		return print_help();
	if (x == "list_variables")
		return print_variables();
	else if (x == "list_functions")
		return print_functions();
	else if (x == "history")
		return print_history();
	else if (x == "exit")
		exit(EXIT_SUCCESS);

	throw std::runtime_error("unknown command");
}

std::string evaluator::operator()(ast::variable_assignation input)
{
	if (input.variable_ == "i")
		throw std::runtime_error("can't use i as a variable name");

	auto rhs = evaluate(input.expression_);

	if (rhs.variable() != "")
		throw std::runtime_error("the expression contains unassigned variable");

	variable_map[input.variable_] = rhs;

	std::stringstream ss;
	ss << rhs;
	return ss.str();
}

std::string evaluator::operator()(ast::function_assignation input)
{
	evaluator::function_checker{input.function_.function_, input.function_.variable_, variable_map, function_map, false}(input.expression_);


	function func{input.function_.variable_, input.expression_, variable_map, function_map};
	function_map[input.function_.function_] = func;

	return func.str();
}

std::string evaluator::operator()(ast::value_resolution x)
{
	auto ret = evaluate(x.expression_);
	std::stringstream ss;
	ss << ret;
	return ss.str();
}

std::string evaluator::operator()(ast::polynomial_resolution x)
{
	auto left = evaluate(x.left_expression);
	auto right = evaluate(x.right_expression);
	auto equation = left - right;
	return polynomial_resolution(equation);
}


// Private Methods

expr evaluator::create_expr(ast::operand const & operand)
{
	return boost::apply_visitor(overloaded
	{
		[](double const & rational)	{ return expr{complex{rational, 0}}; },
		[](char const & imaginary)	{ (void)imaginary; return expr{complex{0, 1}}; },
		[this](std::vector<std::vector<ast::expression>> const & input_matrix)
		{
			auto row_nb = input_matrix.size();
			if (row_nb <= 0)
				throw std::runtime_error("matrix row is zero");

			auto col_nb = input_matrix[0].size();
			if (col_nb <= 0)
				throw std::runtime_error("matrix col is zero");

			std::vector<std::vector<complex>> complex_matrix(row_nb, std::vector<complex>(col_nb, complex{}));

			for (size_t ri = 0; ri < row_nb; ++ri)
			{
				if (input_matrix[ri].size() != col_nb)
					throw std::runtime_error("matrix don't have consistent column number");

				for (size_t ci = 0; ci < col_nb; ++ci)
				{
					auto elem_expr = evaluate(input_matrix[ri][ci]);

					if (elem_expr.variable() != "")
						throw std::runtime_error("unassigned variable in matrix");

					if (elem_expr.is_matrix())
						throw std::runtime_error("matrix inside matrix");

					complex_matrix[ri][ci] = std::get<complex>(elem_expr.term_map().at(0).coef());
				}
			}
			return expr{complex_matrix};
		},
		[this](std::string const & variable)	{ return variable_map.find(variable) != variable_map.end() ? variable_map[variable] : expr{variable}; },
		[this](ast::used_function const & input_function)
		{
			auto it = function_map.find(input_function.function_);
			if (it == function_map.end())
				throw std::runtime_error("the expression contains undefined function");

			function stored_function = it->second;

			expr input_expr = evaluate(input_function.expression_);

			// swap maps
			auto tmp_variable_map = variable_map;
			auto tmp_function_map = function_map;
			variable_map = stored_function.variable_map;
			function_map = stored_function.function_map;

			// set variable value
			variable_map[stored_function.param] = input_expr;

			try
			{
				expr ret_expr = evaluate(stored_function.tree);

				// reset variable value
				variable_map.erase(input_function.function_);

				// swap maps back
				variable_map = tmp_variable_map;
				function_map = tmp_function_map;

				return ret_expr;
			}
			catch(std::runtime_error & e)
			{
				// reset variable value
				variable_map.erase(input_function.function_);

				// swap maps back
				variable_map = tmp_variable_map;
				function_map = tmp_function_map;
				throw e;
			}
		},
		[this](ast::parenthesis const & parenthesis) { return evaluate(parenthesis.expression_); },
		[this](ast::negate const & negate) { return create_expr(negate.operand_) * expr{complex{-1}}; },
		[this](ast::expression const & expression)	{ return evaluate(expression); }
	}
	, operand);
}

expr evaluator::evaluate(ast::expression expression)
{
	auto ret = create_expr(expression.first);
	for (auto const & operation : expression.rest)
	{
		auto rhs = create_expr(operation.operand_);

		static std::map<std::string, std::function<expr(expr const &, expr const &)>> operation_map
		{
			{"", [](expr const & ret, expr const & rhs){ return ret * rhs; }},
			{"+", [](expr const & ret, expr const & rhs){ return ret + rhs; }},
			{"-", [](expr const & ret, expr const & rhs){ return ret - rhs; }},
			{"*", [](expr const & ret, expr const & rhs){ return ret * rhs; }},
			{"/", [](expr const & ret, expr const & rhs){ return ret / rhs; }},
			{"%", [](expr const & ret, expr const & rhs){ return ret % rhs; }},
			{"^", [](expr const & ret, expr const & rhs){ return ret ^ rhs; }},
			{"**", [](expr const & ret, expr const & rhs){ return expr_matrix_mul(ret, rhs); }}
		};
		if (operation_map.find(operation.operator_) == operation_map.end())
			throw std::runtime_error("invalid operator " + operation.operator_);
		ret = operation_map[operation.operator_](ret, rhs);
	}
	return ret;
}


// function_checker

void evaluator::function_checker::operator()(ast::expression const & x)
{
	(*this)(x.first);
	for (auto op : x.rest)
		(*this)(op.operand_);
}

void evaluator::function_checker::operator()(ast::operand const & x)
{
	boost::apply_visitor(*this, x);
}

void evaluator::function_checker::operator()(double x)
{
	// do nothing
	(void)x;
}

void evaluator::function_checker::operator()(char x)
{
	// do nothing
	(void)x;
}

void evaluator::function_checker::operator()(std::string const & x)
{
	if (x == _parameter)
		return ;
	
	auto var = variable_map.find(x);

	if (var == variable_map.end())
		throw std::runtime_error("the expression contain unassigned variable");
}

void evaluator::function_checker::operator()(std::vector<std::vector<ast::expression>> const & x)
{
	if (_checking_matrix)
		throw std::runtime_error("matrix inside matrix");

	_checking_matrix = true;
	for (auto const & row : x)
	{
		for (auto const & elem : row)
		{
			(*this)(elem);
		}
	}
	_checking_matrix = false;
}

void evaluator::function_checker::operator()(ast::parenthesis const & x)
{
	(*this)(x.expression_);
}

void evaluator::function_checker::operator()(ast::used_function const & x)
{
	if (x.function_ == _function_name)
		throw std::runtime_error("the expression contains defining function");

	if (function_map.find(x.function_) == function_map.end())
		throw std::runtime_error("the expression contains undefined function");

	(*this)(x.expression_);
}

void evaluator::function_checker::operator()(ast::negate const & x)
{
	(*this)(x.operand_);
}


// Commands

std::string evaluator::print_help() const
{
	std::string message{
		"Valid Inputs:\n\n"

		"  Value Resolution\n"
		"    expression = ?\n\n"

		"  Polynomial Resolution\n"
		"    expression = expression ?\n\n"

		"  Variable Assignation\n"
		"    variable = expression\n\n"

		"  Function Definition\n"
		"    function(variable) = expression\n\n"

		"  Commands\n"
		"    list_variables - list assigned variables\n"
		"    list_functions - list defined functions\n"
		"    history        - show history of inputs and results\n"
		"    exit           - exit the program"
	};
	return message;
}

std::string evaluator::print_variables() const
{
	if (variable_map.size() == 0)
		return "no assigned variable";

	std::stringstream ss;
	for (auto it = variable_map.cbegin(); it != variable_map.cend(); ++it)
	{
		if (it != variable_map.cbegin())
			ss << "  ";
		ss << it->first << " = " << it->second;
		if (it != --variable_map.cend())
			ss << '\n';
	}
	return ss.str();
}

std::string evaluator::print_functions() const {
	if (function_map.size() == 0)
		return "no defined function";

	std::stringstream ss;
	for (auto it = function_map.cbegin(); it != function_map.cend(); ++it)
	{
		if (it != function_map.cbegin())
			ss << "  ";
		ss << it->first << "(" << it->second.param << ") = " << it->second;
		if (it != --function_map.cend())
			ss << '\n';
	}
	return ss.str();
}

std::string evaluator::print_history() const
{
	if (history_list.size() == 0)
		return "no history";

	std::stringstream ss;
	for (auto it = history_list.cbegin(); it != history_list.cend(); ++it)
	{
		if (it != history_list.cbegin())
			ss << "  ";
		ss << *it;
		if (it != --history_list.cend())
			ss << '\n';
	}
	return ss.str();
}


// Polynomial resolution

std::string evaluator::polynomial_resolution(expr const & equation) const
{
	auto max_degree = equation.term_map().crbegin()->first;
	if (max_degree == 1)
		return solve_equation(equation);
	else if (max_degree == 2)
		return solve_polynomial(equation);
	else if (max_degree == 0)
		throw std::runtime_error("no variable in the equation");
	else if (max_degree < 0)
		throw std::runtime_error("polynomial with negative degree");
	else
		throw std::runtime_error("polynomial with degree more than two");
	return "";
}

std::string evaluator::solve_equation(expr const & equation) const
{
	if (equation.term_map().find(1) == equation.term_map().cend())
		throw std::runtime_error("can't find term with degree one");

	auto b = equation.term_map().find(1) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(1).coef()) : complex{};
	auto c = equation.term_map().find(0) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(0).coef()) : complex{};

	auto result = -c / b;

	std::stringstream ss;
	ss << equation.variable() << " = " << result;
	return ss.str();
}

std::string evaluator::solve_polynomial(expr const & equation) const
{
	if (equation.term_map().find(2) == equation.term_map().cend())
		throw std::runtime_error("can't find term with degree two");

	auto a = std::get<complex>(equation.term_map().at(2).coef());
	auto b = equation.term_map().find(1) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(1).coef()) : complex{};
	auto c = equation.term_map().find(0) != equation.term_map().cend() ? std::get<complex>(equation.term_map().at(0).coef()) : complex{};

	auto sqrt_part = complex_sqrt(b * b - 4 * a * c);
	auto result_one = (-b + sqrt_part) / (2 * a);
	auto result_two = (-b - sqrt_part) / (2 * a);

	std::stringstream ss;
	ss << equation.variable() << " = ";
	if (result_one == result_two)
		ss << result_one;
	else
		ss << result_one << ", " << result_two;
	return ss.str();
}

}