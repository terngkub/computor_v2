#include "expr.hpp"
#include <boost/test/unit_test.hpp>
#include <map>

using expr = computorv2::expr;
using term = computorv2::term;

BOOST_AUTO_TEST_SUITE(ts_expr)

expr ex1{"x"};

BOOST_AUTO_TEST_CASE(tc_print)
{
}

BOOST_AUTO_TEST_CASE(tc_add)
{
}

BOOST_AUTO_TEST_CASE(tc_sub)
{
}

BOOST_AUTO_TEST_CASE(tc_mul)
{
}

BOOST_AUTO_TEST_CASE(tc_div)
{
    BOOST_TEST((ex1 / ex1).str() == "1");
}

BOOST_AUTO_TEST_CASE(tc_mod)
{
    BOOST_TEST((ex1 % ex1).str() == "0");
}

BOOST_AUTO_TEST_CASE(tc_power)
{
}

BOOST_AUTO_TEST_CASE(tc_matrix_mul)
{
}

BOOST_AUTO_TEST_SUITE_END()
