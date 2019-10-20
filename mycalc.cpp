#include <boost/spirit/home/x3.hpp>

#include <iostream>
#include <string>

namespace x3 = boost::spirit::x3;

namespace client
{
    namespace calculator_grammar
    {
        using x3::double_;
        using x3::ascii::alpha;
        using x3::ascii::alnum;

        x3::rule<class rational> const rational("rational");
        x3::rule<class imaginary> const imaginary("imaginary");

        x3::rule<class matrix> const matrix("matrix");
        x3::rule<class matrix_row> const matrix_row("matrix_row");

        x3::rule<class name> const name("name");
        x3::rule<class variable> const variable("variable");
        x3::rule<class function> const function("function");

        x3::rule<class expression> const expression("expression");
        x3::rule<class term> const term("term");
        x3::rule<class power> const power("power");
        x3::rule<class factor> const factor("factor");

        x3::rule<class variable_assignation> const variable_assignation("variable_assignation");
        x3::rule<class function_assignation> const function_assignation("function_assignation");

        x3::rule<class value_resolution> const value_resolution("value_resolution");
        x3::rule<class polynomial_resolution> const polynomial_resolution("polynomial_resolution");

        x3::rule<class input> const input("input");


        // Number
        auto const rational_def = double_;
        auto const imaginary_def = 'i';

        // Matrix
        auto const matrix_def = '[' >> matrix_row >> *(';' >> matrix_row) >> ']';
        auto const matrix_row_def = '[' >> double_ >> *(',' >> double_) >> ']';

        // Variable and Function
        auto const name_def = alpha >> *(alnum);
        auto const variable_def = name;
        auto const function_def = name >> '(' >> name >> ')';

        // Expression
        auto const expression_def = term >> *(('+' >> term | '-' >> term));
        auto const term_def = power >> *(('*' >> power) | ('/' >> power) | ('%' >> power));
        auto const power_def = factor >> *('^' >> factor);
        auto const factor_def = rational | imaginary | function | variable | matrix | ('(' >> expression >> ')')  | ('-' >> factor);

        // Assignation
        auto const variable_assignation_def = variable >> '=' >> expression;
        auto const function_assignation_def = function >> '=' >> expression;

        // Resolution
        auto const value_resolution_def = expression >> '=' >> '?';
        auto const polynomial_resolution_def = expression >> '=' >> expression >> '?';

        // Input
        auto const input_def = variable_assignation | function_assignation | value_resolution | polynomial_resolution | expression;

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
        BOOST_SPIRIT_DEFINE(variable_assignation);
        BOOST_SPIRIT_DEFINE(function_assignation);
        BOOST_SPIRIT_DEFINE(value_resolution);
        BOOST_SPIRIT_DEFINE(polynomial_resolution);
        BOOST_SPIRIT_DEFINE(input);

        auto calculator = input;
    }

    using calculator_grammar::calculator;
}

int main()
{
    std::string str;
    while (std::getline(std::cin, str))
    {
        if (str == "q") break;

        auto & calc = client::calculator;

        auto iter = str.cbegin();
        auto const end = str.cend();
        boost::spirit::x3::ascii::space_type space;
        bool r = phrase_parse(iter, end, calc, space);

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