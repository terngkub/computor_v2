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
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });

        ast::input res;
        if (!phrase_parse(str.begin(), str.end(), grammar::input, boost::spirit::x3::ascii::space, res))
            throw std::runtime_error("invalid syntax");
        result_str = evaluate(res, str);
    }
    return result_str;
}

BOOST_AUTO_TEST_SUITE(ts_correction)

BOOST_AUTO_TEST_CASE(tc_assignation)
{
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
}

BOOST_AUTO_TEST_CASE(tc_calculatoire)
{
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

BOOST_AUTO_TEST_SUITE_END() // ts_correction


// Value Resolution
BOOST_AUTO_TEST_SUITE(ts_value_resolution)

BOOST_AUTO_TEST_CASE(tc_complex)
{
    BOOST_TEST(get_result({"3 + 4i = ?"}) == "3 + 4i");
    BOOST_TEST(get_result({"(3 + 4i) * (5 + 6i) = ?"}) == "-9 + 38i");
}

BOOST_AUTO_TEST_CASE(tc_matrix)
{

}

BOOST_AUTO_TEST_CASE(tc_expression)
{
    // plus sign in front of double
    BOOST_CHECK_THROW(get_result({"+3 = ?"}), std::runtime_error);
    BOOST_TEST(get_result({"x +3 = ?"}) == "x + 3");
}

BOOST_AUTO_TEST_SUITE_END() // ts_value_resolution


// Variable Assignation
BOOST_AUTO_TEST_SUITE(ts_varialbe_assignation)

BOOST_AUTO_TEST_CASE(tc_complex)
{
    BOOST_TEST(get_result({"a = 3"}) == "3");
    BOOST_TEST(get_result({"a = 3", "a = ?"}) == "3");

    BOOST_TEST(get_result({"a = 2.13i"}) == "2.13i");
    BOOST_TEST(get_result({"a = 2.13i", "a = ?"}) == "2.13i");

    BOOST_TEST(get_result({"a = 3.14 + 4.15i"}) == "3.14 + 4.15i");
    BOOST_TEST(get_result({"a = 3.14 + 4.15i", "a = ?"}) == "3.14 + 4.15i");
}

BOOST_AUTO_TEST_CASE(tc_matrix)
{
    BOOST_TEST(get_result({"m = [[1,2]]"}) == "[[1, 2]]");
    BOOST_TEST(get_result({"m = [[1,2];[3,4]]"}) == "[[1, 2]; [3, 4]]");
    BOOST_CHECK_THROW(get_result({"m = [[1,2];[3,4,5]]"}), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(tc_variable)
{
    BOOST_TEST(get_result({"a = 3.14", "b = a", "b = ?"}) == "3.14");
    BOOST_TEST(get_result({"a = 3.14", "a = a"}) == "3.14");
    BOOST_TEST(get_result({"a = 3.14", "b = a", "a = b", "a = ?"}) == "3.14");
    BOOST_TEST(get_result({"a = 3.14", "b = a", "a = b", "b = ?"}) == "3.14");

    BOOST_CHECK_THROW(get_result({"a = a"}), std::runtime_error);
    BOOST_CHECK_THROW(get_result({"a = b"}), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(tc_function)
{
    BOOST_TEST(get_result({"f(x) = x * 3", "a = f(10)", "a = ?"}) == "30");
    BOOST_TEST(get_result({"f(x) = x * 3", "a = -f(10)", "a = ?"}) == "-30");
    BOOST_TEST(get_result({"f(x) = x ** [[1,0];[0,1]]", "a = f([[1,2];[3,4]])", "a = ?"}) == "[[1, 2]; [3, 4]]");
}

BOOST_AUTO_TEST_CASE(tc_expression)
{
    BOOST_TEST(get_result({"a = (12 * 3) / 4", "a = ?"}) == "9");
    BOOST_TEST(get_result({"a = 1 + i", "b = 1 - i", "c = a * b", "c = ?"}) == "2");
}

BOOST_AUTO_TEST_CASE(tc_reassign)
{
    BOOST_TEST(get_result({"a = 3.14", "a = 42", "a = ?"}) == "42");
    BOOST_TEST(get_result({"a = 3.14", "b = a", "a = 42", "a = ?"}) == "42");
    BOOST_TEST(get_result({"a = 3.14", "b = a", "a = 42", "b = ?"}) == "3.14");
}

BOOST_AUTO_TEST_CASE(tc_double_letters)
{
    BOOST_TEST(get_result({"xx = ?"}) == "xx");
    BOOST_TEST(get_result({"ii = ?"}) == "ii");

    BOOST_TEST(get_result({"xx = 3", "xx = ?"}) == "3");
    BOOST_TEST(get_result({"ii = 3", "ii = ?"}) == "3");
}

BOOST_AUTO_TEST_CASE(tc_uppercase)
{
    BOOST_TEST(get_result({"varA = 3", "varA = ?"}) == "3");
    BOOST_TEST(get_result({"varA = 3", "vara = ?"}) == "3");
    BOOST_TEST(get_result({"vara = 3", "varA = ?"}) == "3");

    BOOST_TEST(get_result({"2I = ?"}) == "2i");
    BOOST_TEST(get_result({"iI = ?"}) == "ii");
    BOOST_TEST(get_result({"iI = 3", "ii = ?"}) == "3");
}

BOOST_AUTO_TEST_SUITE_END() // ts_variable_assignation


// Function Assignation
BOOST_AUTO_TEST_SUITE(ts_function_assignation)

BOOST_AUTO_TEST_CASE(tc_printing)
{
    BOOST_TEST(get_result({"f(x) = x"}) == "x");
    BOOST_TEST(get_result({"f(x) = 3x"}) == "3 * x");
    BOOST_TEST(get_result({"f(x) = x + 1"}) == "x + 1");
    BOOST_TEST(get_result({"f(x) = x * -1"}) == "x * -1");
    BOOST_TEST(get_result({"f(x) = 3x + 2i"}) == "3 * x + 2 * i");
    BOOST_TEST(get_result({"f(x) = (x + 1)^3"}) == "(x + 1) ^ 3");

    BOOST_TEST(get_result({"f(x) = x ** x"}) == "x ** x");
    BOOST_TEST(get_result({"f(x) = x ** [[1,2];[3,4]] * 3"}) == "x ** [[1, 2]; [3, 4]] * 3");

    BOOST_TEST(get_result({"f(x) = 3x", "g(x) = f(x)"}) == "(3 * x)");
    BOOST_TEST(get_result({"f(x) = 3x", "g(x) = 4f(x)"}) == "4 * (3 * x)");
}

BOOST_AUTO_TEST_CASE(tc_basic)
{
    BOOST_TEST(get_result({"f(x) = (x + 3)^2 * 4 % 5"}) == "(x + 3) ^ 2 * 4 % 5");
    BOOST_TEST(get_result({"functionName(x) = x + 1", "functionname(x) = ?"}) == "x + 1");
    BOOST_TEST(get_result({"functionname(x) = x + 1", "functionName(x) = ?"}) == "x + 1");
    BOOST_TEST(get_result({"i(x) = x + 1", "i(x) = ?"}) == "x + 1");
}

BOOST_AUTO_TEST_CASE(tc_unassigned_value)
{
    BOOST_CHECK_THROW(get_result({"f(x) = y"}), std::runtime_error);
    BOOST_CHECK_THROW(get_result({"f(x) = x + y"}), std::runtime_error);
    BOOST_CHECK_THROW(get_result({"f(x) = g(x)"}), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(tc_reassign)
{
    BOOST_TEST(get_result({"f(x) = x + 1", "f(x) = x + 2", "f(x) = ?"}) == "x + 2");
    BOOST_TEST(get_result({"f(x) = x + 1", "g(x) = x + 2", "f(x) = g(x)", "f(x) = ?"}) == "x + 2");

    // change f
    BOOST_TEST(get_result({"f(x) = x + 1", "g(x) = f(x)", "f(x) = x + 2", "g(x) = ?"}) == "x + 1");
    BOOST_TEST(get_result({"f(x) = x + 1", "g(x) = f(x)", "f(x) = x + 2", "f(x) = ?"}) == "x + 2");

    // change g
    BOOST_TEST(get_result({"f(x) = x + 1", "g(x) = f(x)", "g(x) = x + 2", "g(x) = ?"}) == "x + 2");
    BOOST_TEST(get_result({"f(x) = x + 1", "g(x) = f(x)", "g(x) = x + 2", "f(x) = ?"}) == "x + 1");
}

BOOST_AUTO_TEST_CASE(tc_loop)
{
    BOOST_CHECK_THROW(get_result({"f(x) = f(x)"}), std::runtime_error);
    BOOST_TEST(get_result({"f(x) = x", "g(x) = f(x)", "f(x) = g(x)", "f(x) = ?"}) == "x");
    BOOST_TEST(get_result({"f(x) = x", "g(x) = f(x)", "f(x) = g(x)", "g(x) = ?"}) == "x");
}

BOOST_AUTO_TEST_CASE(tc_composite)
{
    BOOST_TEST(get_result({"f(x) = x + 1", "g(x) = x ^ 2", "g(f(x)) = ?"}) == "x^2 + 2x + 1");
    BOOST_TEST(get_result({"f(x) = 1 / x", "g(x) = x ^ 2", "g(f(x)) = ?"}) == "x^-2");
    BOOST_TEST(get_result({"f(x) = x + 1", "g(x) = x ^ 2", "h(x) = x * 3", "h(f(x) + g(x)) = ?"}) == "3x^2 + 3x + 3");

    BOOST_CHECK_THROW(get_result({"f(x) = 1 / (x + 1)", "g(x) = x ^ 2", "g(f(x)) = ?"}), std::runtime_error);
    BOOST_CHECK_THROW(get_result({"f(x) = x + 1", "g(f(x)) = x ^ 2"}), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END() // ts_variable_assignation


// Polynomial Resolution
BOOST_AUTO_TEST_SUITE(ts_polynomial_resolution)

BOOST_AUTO_TEST_CASE(tc_zero_degree)
{
    // BOOST_TEST(get_result({""}) == "");
}

BOOST_AUTO_TEST_CASE(tc_one_degree)
{
    BOOST_TEST(get_result({"x = 0 ?"}) == "x = 0");
    BOOST_TEST(get_result({"x + 3.14 = 0 ?"}) == "x = -3.14");
    BOOST_TEST(get_result({"x + 3.14 + 2.56i = 0 ?"}) == "x = -3.14 - 2.56i");
    BOOST_TEST(get_result({"x^2 - x^2 + x + 3.14 = 0 ?"}) == "x = -3.14");
    BOOST_TEST(get_result({"x^2 / x + 3.14 = 0 ?"}) == "x = -3.14");

    // TODO fix
    // BOOST_TEST(get_result({"x^2 / x = 0 ?"}) == "no answer");
}

BOOST_AUTO_TEST_CASE(tc_two_degree)
{
    BOOST_TEST(get_result({"x^2 + 2x + 1 = 0 ?"}) == "x = -1");
    BOOST_TEST(get_result({"x^2 + 3x + 2 = 0 ?"}) == "x = -1, -2");
    BOOST_TEST(get_result({"2x^2 + 5x + 3 = 0 ?"}) == "x = -1, -1.5");
    BOOST_TEST(get_result({"x^2 - 4 = 0 ?"}) == "x = 2, -2");
    BOOST_TEST(get_result({"4x^2 - 4 = 0 ?"}) == "x = 1, -1");
}

BOOST_AUTO_TEST_CASE(tc_invalid_input)
{
    BOOST_CHECK_THROW(get_result({"x ** 3 = 0 ?"}), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(tc_invalid_degree)
{
    // BOOST_TEST(get_result({""}) == "");
}

BOOST_AUTO_TEST_SUITE_END() // ts_polynomial_resolution


BOOST_AUTO_TEST_SUITE_END() // ts_evaluator
