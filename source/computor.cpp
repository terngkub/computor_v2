#include "ast.hpp"
#include "computor.hpp"
#include "parser.hpp"
#include <algorithm>
#include <iostream>
#include <string>

namespace computorv2
{

void computor::operator()()
{
    using boost::spirit::x3::ascii::space;

    constexpr auto & grammar = grammar::input;
    std::string str;

    while (std::cout << "> " && std::getline(std::cin, str))
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });

        auto iter = str.cbegin();
        auto const end = str.cend();
        ast::input res;

        bool r = phrase_parse(iter, end, grammar, space, res);

        try
        {
            if (r && iter == end)
                evaluate(res);
            else
                throw std::runtime_error("invalid syntax");
        }
        catch(const std::exception& e)
        {
            std::cout << "  error: " << e.what() << '\n';
        }
    }
}

} // namespace computorv2