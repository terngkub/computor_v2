#pragma once
#include "ast.hpp"
#include <unordered_map>

namespace computorv2
{

class evaluator
{
public:
	void operator()(client::ast::input x);
	void operator()(client::ast::command x);
	void operator()(client::ast::variable_assignation x);
	void operator()(client::ast::function_assignation x);
	void operator()(client::ast::value_resolution x);
	void operator()(client::ast::polynomial_resolution x);
	void operator()(client::ast::expression x);

    void print_variable_list() const;

private:
    std::unordered_map<client::ast::variable, client::ast::expression> variable_map;

};

}