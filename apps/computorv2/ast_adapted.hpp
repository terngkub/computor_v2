#pragma once
#include <boost/fusion/include/adapt_struct.hpp>
#include "ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(client::ast::coef_variable, coef, variable_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::function, function_, variable_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::expression, first, rest)
BOOST_FUSION_ADAPT_STRUCT(client::ast::variable_assignation, variable_, expression_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::function_assignation, function_, expression_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::value_resolution, expression_)
BOOST_FUSION_ADAPT_STRUCT(client::ast::polynomial_resolution, left_expression, right_expression)