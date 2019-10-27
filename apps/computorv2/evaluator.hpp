#pragma once
#include "ast.hpp"

namespace computorv2
{

class evaluator
{
public:
	void operator()(client::ast::input x);
	void operator()(client::ast::variable_assignation x);
	void operator()(client::ast::function_assignation x);
	void operator()(client::ast::value_resolution x);
	void operator()(client::ast::polynomial_resolution x);
	void operator()(client::ast::expression x);

private:

};

}