#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <iostream>
#include <list>

namespace x3 = boost::spirit::x3;

namespace client { namespace ast
{
	using matrix_row = std::list<double>;
	using matrix = std::list<matrix_row>;
}}

namespace client
{
    namespace calculator_grammar
    {
        using x3::double_;

        x3::rule<class matrix, client::ast::matrix> const matrix("matrix");
        x3::rule<class matrix_row, client::ast::matrix_row> const matrix_row("matrix_row");

        auto const matrix_def = '[' >> matrix_row % ';' >> ']';
        auto const matrix_row_def = '[' >> double_ % ',' >> ']';

        BOOST_SPIRIT_DEFINE(matrix, matrix_row);

        auto calculator = matrix;
    }

    using calculator_grammar::calculator;
}

int main()
{
	std::string str;
	while (std::getline(std::cin, str))
	{
		if (str == "q") break;

		auto & grammar = client::calculator;
		client::ast::matrix res;

		auto iter = str.cbegin();
		auto const end = str.cend();
		x3::ascii::space_type space;


		bool r = phrase_parse(iter, end, grammar, space, res);

		if (r && iter == end)
		{
			for (auto itr = res.begin(); itr != res.end(); ++itr)
			{
				for (auto itc = itr->begin(); itc != itr->end(); ++itc)
				{
					std::cout << *itc << ' ';
				}
				std::cout << '\n';
			}
		}
		else
		{
			std::string rest(iter, end);
			std::cout << "Failed at " << rest << '\n';
		}
	}
}