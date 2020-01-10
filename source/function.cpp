#include "function.hpp"
#include <sstream>
#include <iostream>

namespace computorv2
{

function::function(std::string param,
	ast::expression tree,
	std::map<std::string, expr> variable_map,
	std::map<std::string, function> function_map)
	: param(param)
	, tree(tree)
	, variable_map(variable_map)
	, function_map(function_map)
	, input("")
{
	this->variable_map.erase(param);
}

function::function(function const & func, std::string input)
	: param(func.param)
	, tree(func.tree)
	, variable_map(func.variable_map)
	, function_map(func.function_map)
	, input(input)
{}

std::string function::operator()(double const & nb) const
{
    std::stringstream ss;
    ss << nb;
    return ss.str();
}

std::string function::operator()(char const & c) const
{
    std::stringstream ss;
    ss << c;
    return ss.str();
}

std::string function::operator()(std::string const & str) const
{
    std::stringstream ss;
	if (str == param && input != "")
		ss << '(' << input << ')';
    else if (auto it = variable_map.find(str); it != variable_map.end())
        ss << it->second;
    else
        ss << str;
    return ss.str();
}

std::string function::operator()(std::vector<std::vector<ast::expression>> const & mtx) const
{
  	std::stringstream ss;

	ss << '[';

	for (auto rit = mtx.cbegin(); rit < mtx.cend(); ++rit)
	{
		ss << '[';

		for (auto cit = rit->cbegin(); cit < rit->cend(); ++cit)
		{
			ss << (*this)(*cit);

			if (cit != rit->cend() - 1)
				ss << ", ";
		}

		ss << ']';

		if (rit != mtx.cend() - 1)
			ss << "; ";
	}

	ss << ']';

	return ss.str();
}

std::string function::operator()(ast::used_function const & func) const
{
    if (auto it = function_map.find(func.function_); it != function_map.end())
    {
		std::string input = (*this)(func.expression_);
		function used_func{it->second, input};

        std::stringstream ss;
        ss << '(' << used_func << ')';
        return ss.str();
    }
    throw std::runtime_error("call to undefined function");
}

std::string function::operator()(ast::operation const & op) const
{
	std::stringstream ss;
	ss << (op.operator_ == "" ? "*" : op.operator_) << ' ';
	ss << boost::apply_visitor(*this, op.operand_);
	return ss.str();
}

std::string function::operator()(ast::parenthesis const & paren) const
{
    std::stringstream ss;
	ss << '(' << (*this)(paren.expression_) << ')';
	return ss.str();
}

std::string function::operator()(ast::negate const & neg) const
{
   	std::stringstream ss;
	ss << '-' << boost::apply_visitor(*this, neg.operand_);
	return ss.str();
}

std::string function::operator()(ast::expression const & ex) const
{
	std::stringstream ss;
	ss << boost::apply_visitor(*this, ex.first);
	for (auto const &oper : ex.rest)
		ss << ' ' << (*this)(oper);
	return ss.str();
}

std::string function::str() const
{
    return (*this)(tree);
}

std::ostream & operator<<(std::ostream & os, function const & rhs)
{
    os << rhs.str();
    return os;
}

}