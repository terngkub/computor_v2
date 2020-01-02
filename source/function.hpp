#pragma once
#include "ast.hpp"
#include "expr.hpp"
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
};

}