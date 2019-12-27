#include "ast.hpp"
#include "computor.hpp"
#include "parser.hpp"
#include <iostream>

namespace computorv2
{

void computor::operator()()
{
    using boost::spirit::x3::ascii::space;

    constexpr auto & grammar = grammar::input;
    std::string str;

    while (std::cout << "> " && std::getline(std::cin, str))
    {
        auto iter = str.cbegin();
        auto const end = str.cend();
        ast::input res;

        bool r = phrase_parse(iter, end, grammar, space, res);

        if (r && iter == end)
        {
            try
            {
                evaluate(res);
            }
            catch(const std::exception & e)
            {
                std::cout << "  error: " << e.what() << '\n';
            }
        }
        else
        {
            std::string rest(iter, end);
            std::cout << "  error: invalid syntax\n";
        }
    }
}

} // namespace computorv2