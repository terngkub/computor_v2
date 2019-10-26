#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <iostream>
#include <list>
#include <string>

namespace x3 = boost::spirit::x3;

namespace client { namespace ast
{
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
		coef_variable
		, rational
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
		variable_assignation
		, function_assignation
		, value_resolution
		, polynomial_resolution
		, expression
	>
	{
		using base_type::base_type;
		using base_type::operator=;
	};
}}

BOOST_FUSION_ADAPT_STRUCT(client::ast::coef_variable, coef, variable_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::function, function_, variable_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::expression, first, rest)
BOOST_FUSION_ADAPT_STRUCT(client::ast::variable_assignation, variable_, expression_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::function_assignation, function_, expression_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::value_resolution, expression_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::polynomial_resolution, left_expression, right_expression)

namespace client { namespace ast
{
	struct printer
	{
		void operator()(rational const & rational_) const
		{
			std::cout << rational_;
		}

		void operator()(imaginary const & imaginary_) const
		{
			std::cout << imaginary_;
		}

		void operator()(matrix const & matrix_) const
		{
			for (auto itr = matrix_.begin(); itr < matrix_.end(); ++itr)
			{
				std::cout << '|';
				for (auto itc = itr->begin(); itc < itr->end(); ++itc)
				{
					std::cout << *itc;
					if (itc != itr->end() - 1)
						std::cout << ' ';
				}
				std::cout << '|';
				if (itr != matrix_.end() - 1)
					std::cout << '\n';
			}
		}

		void operator()(variable const & variable_) const
		{
			std::cout << variable_;
		}

		void operator()(coef_variable const & coef_variable_) const
		{
			std::cout << coef_variable_.coef << " * " << coef_variable_.variable_;
		}

		void operator()(function const & function_) const
		{
			std::cout << function_.function_ << '(' << function_.variable_ << ')';
		}


		void operator()(operation const & x) const
		{
			boost::apply_visitor(*this, x.operand_);
			std::cout << ' ' << x.operator_;
		}

		void operator()(expression const & x) const
		{
			boost::apply_visitor(*this, x.first);
			for(auto const & oper: x.rest)
			{
				std::cout << ' ';
				(*this)(oper);
			}
		}

		void operator()(variable_assignation const & x) const
		{
			(*this)(x.variable_);
			std::cout << " = ";
			(*this)(x.expression_);
		}

		void operator()(function_assignation const & x) const
		{
			(*this)(x.function_);
			std::cout << " = ";
			(*this)(x.expression_);
		}

		void operator()(value_resolution const & x) const
		{
			(*this)(x.expression_);
			std::cout << " = ?";
		}

		void operator()(polynomial_resolution const & x) const
		{
			(*this)(x.left_expression);
			std::cout << " = ";
			(*this)(x.right_expression);
			std::cout << " ?";
		}

		void operator()(input const & input_) const
		{
			boost::apply_visitor(*this, input_);
			std::cout << std::endl;
		} 
	};
}}

namespace client
{
	namespace grammar
	{
		using x3::char_;
		using x3::double_;
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

		x3::rule<class input, ast::input> const input("input");


		auto const rational_def = double_;
		auto const imaginary_def = char_('i');

		auto const matrix_def = '[' >> matrix_row % ';' >> ']';
		auto const matrix_row_def = '[' >> double_ % ',' >> ']';

		auto const name_def = alpha >> *(alnum);
		auto const variable_def = name;
		auto const coef_variable_def = double_ >> variable;
		auto const function_def = name >> '(' >> variable >> ')';

		auto const expression_def = term >> *((char_('+') >> term | char_('-') >> term));
		auto const term_def = power >> *((char_('*') >> power) | (char_('/') >> power) | (char_('%') >> power));
		auto const power_def = factor >> *(char_('^') >> factor);
		auto const factor_def = coef_variable | rational | imaginary | ('(' >> expression >> ')') | function | variable | matrix | ('-' >> factor);

		auto const variable_assignation_def = variable >> '=' >> expression;
		auto const function_assignation_def = function >> '=' >> expression;

		auto const value_resolution_def = expression >> '=' >> '?';
		auto const polynomial_resolution_def = expression >> '=' >> expression >> '?';

		auto const input_def = variable_assignation | function_assignation | value_resolution | polynomial_resolution | expression;

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
		BOOST_SPIRIT_DEFINE(input);

		auto calculator = input;
	}

	using grammar::calculator;
}

int main()
{
	std::string str;

	client::ast::printer print;

	while (std::getline(std::cin, str))
	{
		if (str == "q") break;

		auto & grammar = client::calculator;

		auto iter = str.cbegin();
		auto const end = str.cend();
		boost::spirit::x3::ascii::space_type space;
		client::ast::input res;
		bool r = phrase_parse(iter, end, grammar, space, res);

		if (r && iter == end)
		{
			print(res);
		}
		else
		{
			std::string rest(iter, end);
			std::cout << "Failed at " << rest << "\n";
		}
	}
}