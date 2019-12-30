#include <boost/test/unit_test.hpp>
#include "ast.hpp"
#include "evaluator.hpp"
#include "parser.hpp"

BOOST_AUTO_TEST_SUITE(ts_evaluator)

computorv2::evaluator evaluate;

std::string get_result(std::string str)
{
    ast::input res;
    phrase_parse(str.begin(), str.end(), grammar::input, boost::spirit::x3::ascii::space, res);
    return evaluate(res, str);
}

BOOST_AUTO_TEST_SUITE(ts_value_resolution)

BOOST_AUTO_TEST_CASE(tc_basic)
{
    BOOST_TEST(get_result("1 + 2 = ?") == "3");
}

BOOST_AUTO_TEST_SUITE_END() // ts_value_resolution

BOOST_AUTO_TEST_SUITE_END() // ts_evaluator
