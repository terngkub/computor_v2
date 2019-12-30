#include <boost/test/unit_test.hpp>
#include "ast.hpp"
#include "evaluator.hpp"
#include "parser.hpp"

BOOST_AUTO_TEST_SUITE(ts_evaluator)

std::string get_result(std::vector<std::string> v)
{
    computorv2::evaluator evaluate;

    std::string result_str;
    for (auto & str : v)
    {
        ast::input res;
        if (!phrase_parse(str.begin(), str.end(), grammar::input, boost::spirit::x3::ascii::space, res))
            throw std::runtime_error("invalid syntax");
        result_str = evaluate(res, str);
    }
    return result_str;
}

BOOST_AUTO_TEST_SUITE(ts_value_resolution)

BOOST_AUTO_TEST_CASE(tc_correction)
{
    // Partie assignation

    // Test erreur élémentaire
    BOOST_CHECK_THROW(get_result({"x == 2"}), std::runtime_error);
    BOOST_CHECK_THROW(get_result({"x = 23edd23-+-+"}), std::runtime_error);

    // Test erreur semi-avancé
    BOOST_CHECK_THROW(get_result({"= 2"}), std::runtime_error);
    BOOST_CHECK_THROW(get_result({"3 = 4"}), std::runtime_error);
    BOOST_CHECK_THROW(get_result({"x = g"}), std::runtime_error);
    BOOST_CHECK_THROW(get_result({"f(x = 2"}), std::runtime_error);
    BOOST_CHECK_THROW(get_result({"x = [[4,2]"}), std::runtime_error);

    // Test erreur avancé
    BOOST_TEST(get_result({"x = --2", "x = ?"}) == "2");
    BOOST_CHECK_THROW(get_result({"f(x) = x * 2", "t = f(x)"}), std::runtime_error);
    BOOST_CHECK_THROW(get_result({"i = 2"}), std::runtime_error);

    // Test valide élémentaire
    BOOST_TEST(get_result({"x = 2", "x = ?"}) == "2");
    BOOST_TEST(get_result({"y = 4i", "y = ?"}) == "4i");
    BOOST_TEST(get_result({"z = [[2,3];[3,5]]", "z = ?"}) == "[[2, 3]; [3, 5]]");

    // Test valide semi-avancé
    BOOST_TEST(get_result({"x = 2", "y = x", "y = ?"}) == "2");
    BOOST_TEST(get_result({"x = 2", "x = 5", "x = ?"}) == "5");
    BOOST_TEST(get_result({"A = [[2,3]]", "B = A", "B = ?"}) == ("[[2, 3]]"));

    // Test valide avancé
    BOOST_TEST(get_result({"x = 2", "y = x * [[4,2]]", "f(z) = z * y", "f(z) = ?"}) == "[[8, 4]] * z");
    BOOST_TEST(get_result({"x = 2", "f(x) = x * 5", "f(x) = ?"}) == "10");

    // Partie calculatoire

    // Test valide élémentaire
    BOOST_TEST(get_result({"2 + 2 = ?"}) == "4");
    BOOST_TEST(get_result({"3 * 4 = ?"}) == "12");
    BOOST_TEST(get_result({"x = 2", "x + 2 = ?"}) == "4");
    BOOST_CHECK_THROW(get_result({"2 / 0 = ?"}), std::runtime_error);
    BOOST_TEST(get_result({"1 + 1.5 = ?"}) == "2.5");

    // Test valide semi-avancé
    BOOST_TEST(get_result({"x = 2 * i", "x ^ 2 = ?"}) == "-4");
    BOOST_TEST(get_result({"A = [[2,3];[3,4]]", "B = [[1,0];[0,1]]", "A ** B = ?"}) == "[[2, 3]; [3, 4]]");
    BOOST_TEST(get_result({"f(x) = x + 2", "p = 4", "f(p) = ?"}) == "6");

    // Test valide avancé
    BOOST_TEST(get_result({"4 - 3 - ( 2 * 3 ) ^ 2 * ( 2 - 4 ) + 4 = ?"}) == "77");
    BOOST_TEST(get_result({"f(x) = 2*(x + 3*(x-4))", "p = 2", "f(3) - f(p) + 2 = ?"}) == "10");
    BOOST_TEST(get_result({"f(x) = 2*x*i", "f(2) = ?"}) == "4i");
}

BOOST_AUTO_TEST_SUITE_END() // ts_value_resolution

BOOST_AUTO_TEST_SUITE_END() // ts_evaluator
