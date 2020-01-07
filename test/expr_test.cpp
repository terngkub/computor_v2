#include "expr.hpp"
#include <boost/test/unit_test.hpp>
#include <map>

using expr = computorv2::expr;
using term = computorv2::term;
using complex = computorv2::complex;

BOOST_AUTO_TEST_SUITE(ts_expr)

expr ex0{};
expr ex1{"x"};

expr ex2{std::map<int, term>{
    {0, term{2}}
}};

expr ex3{std::map<int, term>{
    {1, term{3, "x"}}
}};

expr ex4{std::map<int, term>{
    {0, term{4}},
    {1, term{5, "x"}}
}};

expr ex5{std::map<int, term>{
    {0, term{complex{-2, 1}}},
    {1, term{complex{1, -1}, "x"}},
    {2, term{4, "x"}}
}};

expr ex6{std::map<int, term>{
    {0, term{complex{-2, -3}}},
    {1, term{4, "x"}}
}};



BOOST_AUTO_TEST_CASE(tc_print)
{
    BOOST_TEST(ex0.str() == "0");
    BOOST_TEST(ex1.str() == "x");
    BOOST_TEST(ex2.str() == "2");
    BOOST_TEST(ex3.str() == "3x");
    BOOST_TEST(ex4.str() == "5x + 4");
    BOOST_TEST(ex5.str() == "4x^2 + (1 - i)x - 2 + i");
    BOOST_TEST(ex6.str() == "4x - 2 - 3i");
}

BOOST_AUTO_TEST_CASE(tc_add)
{
}

BOOST_AUTO_TEST_CASE(tc_sub)
{
}

BOOST_AUTO_TEST_CASE(tc_mul)
{
    BOOST_TEST((ex1 * ex0).str() == "0");
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
