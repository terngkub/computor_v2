#include "ast_adapted.hpp"
#include "computor.hpp"
#include "parser.hpp"
#include <iostream>

namespace computorv2
{

void computor::operator()()
{
    using boost::spirit::x3::ascii::space;

    constexpr auto & grammar = client::calculator;
    std::string str;

    while (std::getline(std::cin, str))
    {
        auto iter = str.cbegin();
        auto const end = str.cend();
        client::ast::input res;

        bool r = phrase_parse(iter, end, grammar, space, res);

        if (r && iter == end)
        {
            evaluate(res);
            print(res);
        }
        else
        {
            std::string rest(iter, end);
            std::cout << "Failed at " << rest << "\n";
        }
    }
}

} // namespace computorv2