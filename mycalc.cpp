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
    using matrix_row = std::list<double>;
    using matrix = std::list<matrix_row>;
    using name = std::string;
    using variable = name;

    struct function
    {
        name function_name;
        name variable_name;
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

    // struct variable_assignation
    // {
    //     variable variable_;
    //     expression expression_;
    // };

    // struct function_assignation
    // {
    //     function function_;
    //     expression expression_;
    // };

    // struct value_resolution
    // {
    //     expression expression_;
    // };

    // struct polynomial_resolution
    // {
    //     expression left_expression;
    //     expression right_expression;
    // };

    // struct input : x3::variant<
    //     variable_assignation
    //     , function_assignation
    //     , value_resolution
    //     , polynomial_resolution
    // >
    // {
    //     using base_type::base_type;
    //     using base_type::operator=;
    // };
}}

BOOST_FUSION_ADAPT_STRUCT(client::ast::function, function_name, variable_name)
BOOST_FUSION_ADAPT_STRUCT(client::ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::expression, first, rest)
// BOOST_FUSION_ADAPT_STRUCT(client::ast::variable_assignation, variable_, expression_)
// BOOST_FUSION_ADAPT_STRUCT(client::ast::function_assignation, function_, expression_)
// BOOST_FUSION_ADAPT_STRUCT(client::ast::value_resolution, expression_)
// BOOST_FUSION_ADAPT_STRUCT(client::ast::polynomial_resolution, left_expression, right_expression)

namespace client
{
    namespace grammar
    {
        using x3::char_;
        using x3::double_;
        using x3::lexeme;
        using x3::ascii::alpha;
        using x3::ascii::alnum;

        x3::rule<class rational, ast::rational> const rational("rational");
        x3::rule<class imaginary, ast::imaginary> const imaginary("imaginary");

        x3::rule<class matrix, ast::matrix> const matrix("matrix");
        x3::rule<class matrix_row, ast::matrix_row> const matrix_row("matrix_row");

        x3::rule<class name, ast::name> const name("name");
        x3::rule<class variable, ast::variable> const variable("variable");
        x3::rule<class function, ast::function> const function("function");

        x3::rule<class expression, ast::expression> const expression("expression");
        x3::rule<class term, ast::expression> const term("term");
        x3::rule<class power, ast::expression> const power("power");
        x3::rule<class factor, ast::operand> const factor("factor");

        // x3::rule<class variable_assignation, ast::variable_assignation> const variable_assignation("variable_assignation");
        // x3::rule<class function_assignation, ast::function_assignation> const function_assignation("function_assignation");

        // x3::rule<class value_resolution, ast::value_resolution> const value_resolution("value_resolution");
        // x3::rule<class polynomial_resolution, ast::polynomial_resolution> const polynomial_resolution("polynomial_resolution");

        // x3::rule<class input, ast::input> const input("input");


        auto const rational_def = double_;
        auto const imaginary_def = char_('i');

        auto const matrix_def = '[' >> matrix_row % ';' >> ']';
        auto const matrix_row_def = '[' >> double_ % ',' >> ']';

        // auto const name_def = alpha >> *(alnum);
        auto const name_def = *alpha;
        auto const variable_def = name;
        auto const function_def = name >> '(' >> name >> ')';

        auto const expression_def = term >> *((char_('+') >> term | char_('-') >> term));
        auto const term_def = power >> *((char_('*') >> power) | (char_('/') >> power) | (char_('%') >> power));
        auto const power_def = factor >> *(char_('^') >> factor);
        auto const factor_def = rational | imaginary | ('(' >> expression >> ')') | function | variable | matrix | ('-' >> factor);

        // auto const variable_assignation_def = variable >> '=' >> expression;
        // auto const function_assignation_def = function >> '=' >> expression;

        // auto const value_resolution_def = expression >> '=' >> '?';
        // auto const polynomial_resolution_def = expression >> '=' >> expression >> '?';

        // auto const input_def = variable_assignation | function_assignation | value_resolution | polynomial_resolution | expression;

        BOOST_SPIRIT_DEFINE(rational);
        BOOST_SPIRIT_DEFINE(imaginary);
        BOOST_SPIRIT_DEFINE(matrix);
        BOOST_SPIRIT_DEFINE(matrix_row);
        BOOST_SPIRIT_DEFINE(name);
        BOOST_SPIRIT_DEFINE(variable);
        BOOST_SPIRIT_DEFINE(function);
        BOOST_SPIRIT_DEFINE(expression);
        BOOST_SPIRIT_DEFINE(term);
        BOOST_SPIRIT_DEFINE(power);
        BOOST_SPIRIT_DEFINE(factor);
        // BOOST_SPIRIT_DEFINE(variable_assignation);
        // BOOST_SPIRIT_DEFINE(function_assignation);
        // BOOST_SPIRIT_DEFINE(value_resolution);
        // BOOST_SPIRIT_DEFINE(polynomial_resolution);
        // BOOST_SPIRIT_DEFINE(input);

        // auto calculator = input;
        auto calculator = expression;
    }

    using grammar::calculator;
}

int main()
{
    std::string str;
    while (std::getline(std::cin, str))
    {
        if (str == "q") break;

        auto & grammar = client::calculator;

        auto iter = str.cbegin();
        auto const end = str.cend();
        boost::spirit::x3::ascii::space_type space;
        // client::ast::input res;
        client::ast::expression res;
        bool r = phrase_parse(iter, end, grammar, space, res);

        if (r && iter == end)
        {
            std::cout << "success\n";
        }
        else
        {
            std::string rest(iter, end);
            std::cout << "Failed at " << rest << "\n";
        }
    }
}