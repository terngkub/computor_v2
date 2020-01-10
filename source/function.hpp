#pragma once
#include "ast.hpp"
#include "expr.hpp"
#include <iostream>
#include <map>

namespace computorv2
{

// name(param) = tree
struct function
{
    std::string param;
    ast::expression tree;
    std::map<std::string, expr> variable_map;
    std::map<std::string, function> function_map;

	function() = default;
	function(std::string param,
		ast::expression tree,
		std::map<std::string, expr> variable_map,
		std::map<std::string, function> function_map);

	function(function const & f) = default;
	function(function && f) = default;

	function & operator=(function const & rhs) = default;
	function & operator=(function && rhs) = default;

	std::string operator()(double const & nb) const;
	std::string operator()(char const & c) const;
	std::string operator()(std::vector<std::vector<ast::expression>> const & mtx) const;
	std::string operator()(std::string const & str) const;
	std::string operator()(ast::assigned_function const & func) const;
	std::string operator()(ast::used_function const & func) const;
	std::string operator()(ast::operation const & op) const;
	std::string operator()(ast::parenthesis const & paren) const;
	std::string operator()(ast::negate const & neg) const;
	std::string operator()(ast::expression const & ex) const;

    std::string str() const;
};

std::ostream & operator<<(std::ostream & os, function const & rhs);

}