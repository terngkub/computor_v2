#include "expr.hpp"
#include <boost/test/unit_test.hpp>
#include <map>

using expr = computorv2::expr;
using term = computorv2::term;
using complex = computorv2::complex;

BOOST_AUTO_TEST_SUITE(ts_expr)

/*
// 0
expr ex0{};

// x
expr ex1{"x"};

// 2
expr ex2{std::map<int, term>{
	{0, term{2}}
}};

// 3x
expr ex3{std::map<int, term>{
	{1, term{3, "x"}}
}};

// 5x + 4
expr ex4{std::map<int, term>{
	{0, term{4}},
	{1, term{5, "x"}}
}};

// 4x^2 + (1 - i)x - 2 + i
expr ex5{std::map<int, term>{
	{0, term{complex{-2, 1}}},
	{1, term{complex{1, -1}, "x"}},
	{2, term{4, "x"}}
}};

// 4x - 2 - 3i
expr ex6{std::map<int, term>{
	{0, term{complex{-2, -3}}},
	{1, term{4, "x"}}
}};
*/



BOOST_AUTO_TEST_CASE(tc_print)
{
	/*
	BOOST_TEST(ex0.str() == "0");
	BOOST_TEST(ex1.str() == "x");
	BOOST_TEST(ex2.str() == "2");
	BOOST_TEST(ex3.str() == "3x");
	BOOST_TEST(ex4.str() == "5x + 4");
	BOOST_TEST(ex5.str() == "4x^2 + (1 - i)x - 2 + i");
	BOOST_TEST(ex6.str() == "4x - 2 - 3i");
	*/
}

// 0
expr ex0{};

// x
expr ex1{"x"};

// x^2
expr ex2{std::map<int, term>{
	{2, term{1, "x"}}
}};

// 3.14x^2
expr ex3{std::map<int, term>{
	{2, term{3.14, "x"}}
}};

// 3.14
expr ex4{3.14};

// -3.14
expr ex5{-3.14};

// 4i
expr ex6{complex{0, 4}};

// 6i
expr ex7{complex{0, 6}};

// 1 + i
expr ex8{complex{1, 1}};

// [[1, 2]; [3, 4]]
expr ex9{std::vector<std::vector<complex>>{
	{1, 2},
	{3, 4}
}};

// [[1, 0]; [0, 1]]
expr ex10{std::vector<std::vector<complex>>{
	{1, 0},
	{0, 1}
}};

// x^2 + 2x + 1
expr ex11{std::map<int, term>{
	{0, term{1}},
	{1, term{2, "x"}},
	{2, term{1, "x"}}
}};

// -1 + i
expr ex12{complex{-1, 1}};

// x + 1
expr ex13{std::map<int, term>{
	{0, term{1}},
	{1, term{1, "x"}}
}};


BOOST_AUTO_TEST_CASE(tc_add)
{
	// x + 0 = x
	BOOST_TEST((ex1 + ex0).str() == "x");

	// x^2 + 3.14x^2 = 4.14x^2
	BOOST_TEST((ex2 + ex3).str() == "4.14x^2");

	// 3.14 + -3.14 = 0
	BOOST_TEST((ex4 + ex5).str() == "0");

	// 4i + 6i = 10i
	BOOST_TEST((ex6 + ex7).str() == "10i");

	// [[1, 2]; [3, 4]] + (1 + i) = [[2 + i, 3 + i]; [4 + i, 5 + i]]
	BOOST_TEST((ex9 + ex8).str() == "[[2 + i, 3 + i]; [4 + i, 5 + i]]");

	// [[1, 2]; [3, 4]] + [[1, 0]; [0, 1]] = [[2, 2]; [3, 5]]
	BOOST_TEST((ex9 + ex10).str() == "[[2, 2]; [3, 5]]");
}


BOOST_AUTO_TEST_CASE(tc_sub)
{
	// x - 0 = x
	BOOST_TEST((ex1 - ex0).str() == "x");

	// x^2 - 3.14x^2 = -2.14x^2
	BOOST_TEST((ex2 - ex3).str() == "-2.14x^2");

	// 3.14 - -3.14 = 6.28
	BOOST_TEST((ex4 - ex5).str() == "6.28");

	// 4i - 6i = -2i
	BOOST_TEST((ex6 - ex7).str() == "-2i");

	// [[1, 2]; [3, 4]] - (1 + i) = [[-i, 1 - i]; [2 - i, 3 - i]] 
	BOOST_TEST((ex9 - ex8).str() == "[[-i, 1 - i]; [2 - i, 3 - i]]");

	// [[1, 2]; [3, 4]] - [[1, 0]; [0, 1]] = [[0, 2]; [3, 3]]
	BOOST_TEST((ex9 - ex10).str() == "[[0, 2]; [3, 3]]");
}


BOOST_AUTO_TEST_CASE(tc_mul)
{
	// x * 0 = 0
	BOOST_TEST((ex1 * ex0).str() == "0");

	// x^2 * 3.14x^2 = 3.14x^4
	BOOST_TEST((ex2 * ex3).str() == "3.14x^4");

	// 3.14 * -3.14 = -9.8596
	BOOST_TEST((ex4 * ex5).str() == "-9.8596");

	// 4i * 6i = -24
	BOOST_TEST((ex6 * ex7).str() == "-24");

	// [[1, 2]; [3, 4]] * (1 + i) = [[1 + i, 2 + 2i]; [3 + 3i, 4 + 4i]]
	BOOST_TEST((ex9 * ex8).str() == "[[1 + i, 2 + 2i]; [3 + 3i, 4 + 4i]]");

	// [[1, 2]; [3, 4]] * [[1, 0]; [0, 1]] = [[1, 0]; [0, 4]]
	BOOST_TEST((ex9 * ex10).str() == "[[1, 0]; [0, 4]]");

	// (x^2 + 2x + 1) * 0 = 0
	BOOST_TEST((ex11 * ex0).str() == "0");

	// (-1 + i) * -1 = 1 - i
	BOOST_TEST((ex12 * expr{-1}).str() == "1 - i");

	// (x + 1)(x + 1)(x + 1) = x^3 + 3x^2 + 3x + 1
	BOOST_TEST((ex13 * ex13 * ex13).str() == "x^3 + 3x^2 + 3x + 1");
}


BOOST_AUTO_TEST_CASE(tc_div)
{
	// x / 0 = throw
	BOOST_CHECK_THROW((ex1 / ex0), std::runtime_error);

	// 3.14x^2 / x^2 = 3.14
	BOOST_TEST((ex3 / ex2).str() == "3.14");

	// 3.14 / -3.14 = -1
	BOOST_TEST((ex4 / ex5).str() == "-1");

	// 6i / 4i = 1.5
	BOOST_TEST((ex7 / ex6).str() == "1.5");

	// [[1, 2]; [3, 4]] / (1 + i) = [[0.5 - 0.5i, 1 - i]; [1.5 - 1.5i, 2 - 2i]]
	BOOST_TEST((ex9 / ex8).str() == "[[0.5 - 0.5i, 1 - i]; [1.5 - 1.5i, 2 - 2i]]");

	// [[1, 2]; [3, 4]] / [[1, 0]; [0, 1]] = throw
	BOOST_CHECK_THROW((ex9 / ex10), std::runtime_error);

	// (x^2 + 2x + 1) / 0 = throw
	BOOST_CHECK_THROW(((ex13 * ex13) / ex0), std::runtime_error);

	// (-1 + i) / -1 = 1 - i
	BOOST_TEST((ex12 / expr{-1}).str() == "1 - i");
}

BOOST_AUTO_TEST_CASE(tc_mod)
{

}

BOOST_AUTO_TEST_CASE(tc_power)
{
	/*
	// Integer degree
	(1 + i) ^ 3 = ?
	-2 + 2i

	[[1, 2]; [3, 4]] ^ 3 = ?
	[[1, 8]; [27, 64]]

	(x + 1) ^ 3 = ?
	x^3 + 3x^2 + 3x + 1

	// Zero degree

	(1 + i) ^ 0 = ?
	1

	[[1, 2]; [3, 4]] ^ 0 = ?
	[[1, 1]; [1, 1]]


	// Negative degree

	x ^ -2 = ?
	x^-2

	(1 + i) ^ -2 = ?
	-0.5i

	[[1, 2]; [4, 8]] ^ -2 = ?
	[[1, 0.25]; [0.0625, 0.015625]]

	*/

	// Invalid degree
	BOOST_CHECK_THROW((expr{"x"} ^ expr{-2}), std::runtime_error);
	BOOST_CHECK_THROW((expr{"x"} ^ expr{3.14}), std::runtime_error);
	BOOST_CHECK_THROW((expr{"x"} ^ expr{complex{1, 1}}), std::runtime_error);
	BOOST_CHECK_THROW((expr{"x"} ^ expr{"x"}), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(tc_matrix_mul)
{
	/*
	[[1, 2]; [3, 4]] ** [[1, 0]; [0, 1]] = ?
	[[1, 2]; [3, 4]]

	[[1 + i, 2 + i]; [2 - i, -1 + 3i]] ** [1 + i, 2 + i]; [2 - i, -1 + 3i] = ?
	[[5 + 2i, -4 + 8i]; [4 + 8i, -3 - 6i]]

	[[1, 2]; [3, 4]] ** [[1]] = ?
	throw

	[[1, 2]; [3, 4]] ** 1 = ?
	throw

	[[1, 2]; [3, 4]] ** x = ?
	throw
	*/
}

BOOST_AUTO_TEST_SUITE_END()
