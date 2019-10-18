#include <boost/spirit/home/x3.hpp>

#include <iostream>
#include <string>

namespace x3 = boost::spirit::x3;

namespace client
{
    namespace calculator_grammar
    {
        using x3::ascii::alpha;
        using x3::ascii::alnum;

        x3::rule<class variable> const variable("variable");

        auto const variable_def = alpha >> +(alnum);

        BOOST_SPIRIT_DEFINE(variable);

        auto calculator = variable;
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