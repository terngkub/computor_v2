#include "ast.hpp"
#include "ast_adapted.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include <iostream>

int main()
{
	using boost::spirit::x3::ascii::space;

	std::string str;

	client::ast::printer print;

	while (std::getline(std::cin, str))
	{
		if (str == "q") break;

		auto & grammar = client::calculator;

		auto iter = str.cbegin();
		auto const end = str.cend();
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