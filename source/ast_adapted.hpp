#pragma once
#include <boost/fusion/include/adapt_struct.hpp>
#include "ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(ast::coef_variable, coef, variable_)
BOOST_FUSION_ADAPT_STRUCT(ast::function, function_, variable_)
BOOST_FUSION_ADAPT_STRUCT(ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(ast::expression, first, rest)
BOOST_FUSION_ADAPT_STRUCT(ast::variable_assignation, variable_, expression_)
BOOST_FUSION_ADAPT_STRUCT(ast::function_assignation, function_, expression_)
BOOST_FUSION_ADAPT_STRUCT(ast::value_resolution, expression_)
BOOST_FUSION_ADAPT_STRUCT(ast::polynomial_resolution, left_expression, right_expression)