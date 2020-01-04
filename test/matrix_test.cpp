#include "matrix.hpp"
#include <boost/test/unit_test.hpp>

using matrix = computorv2::matrix;
using complex = computorv2::complex;
using vvd = std::vector<std::vector<complex>>;

BOOST_AUTO_TEST_SUITE(ts_matrix)

BOOST_AUTO_TEST_CASE(tc_printing)
{
	BOOST_TEST((matrix{}.str() == "[]"));
	BOOST_TEST((matrix{1, 1}.str() == "[[0]]"));
	BOOST_TEST((matrix{2, 3}.str() == "[[0, 0, 0]; [0, 0, 0]]"));
	BOOST_TEST((matrix{vvd{{1, 2, 3}, {4, 5, 6}}}.str() == "[[1, 2, 3]; [4, 5, 6]]"));
}

// Term-to-term Operations
BOOST_AUTO_TEST_SUITE(ts_term_to_term)

BOOST_AUTO_TEST_CASE(tc_plus)
{
	BOOST_CHECK_THROW((matrix{vvd{{1,2,3}, {4,5}}} + matrix{vvd{{1,2}, {4, 5, 6}}}), std::runtime_error);

	BOOST_TEST(( matrix({{complex(1, 2)}}) + matrix({{complex(3, 4)}}) == matrix({{complex(4, 6)}}) ));
	BOOST_TEST(( matrix({{complex(1, 2), complex(3, 4)}}) + matrix({{complex(5, 6), complex(7, 8)}}) == matrix({{complex(6, 8), complex(10, 12)}}) ));
	BOOST_TEST(( matrix({{complex(1, 2)}, {complex(3, 4)}}) + matrix({{complex(5, 6)}, {complex(7, 8)}}) == matrix({{complex(6, 8)}, {complex(10, 12)}}) ));

	BOOST_TEST((
		matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) +
		matrix({{complex(8, 7), complex(6, 5), complex(4, 3)}, {complex(2, 1), complex(0, 12), complex(11, 10)}}) ==
		matrix({{complex(9, 9), complex(9, 9), complex(9, 9)}, {complex(9, 9), complex(9, 22), complex(22, 22)}})
	));
}

BOOST_AUTO_TEST_CASE(tc_minus)
{
	BOOST_CHECK_THROW((matrix{vvd{{1,2,3}, {4,5}}} - matrix{vvd{{1,2}, {4, 5, 6}}}), std::runtime_error);

	BOOST_TEST(( matrix({{complex(1, 2)}}) - matrix({{complex(3, 4)}}) == matrix({{complex(-2, -2)}}) ));
	BOOST_TEST(( matrix({{complex(1, 2), complex(3, 4)}}) - matrix({{complex(5, 6), complex(7, 8)}}) == matrix({{complex(-4, -4), complex(-4, -4)}}) ));
	BOOST_TEST(( matrix({{complex(1, 2)}, {complex(3, 4)}}) - matrix({{complex(5, 6)}, {complex(7, 8)}}) == matrix({{complex(-4, -4)}, {complex(-4, -4)}}) ));

	BOOST_TEST((
		matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) -
		matrix({{complex(8, 7), complex(6, 5), complex(4, 3)}, {complex(2, 1), complex(0, 12), complex(11, 10)}}) ==
		matrix({{complex(-7, -5), complex(-3, -1), complex(1, 3)}, {complex(5, 7), complex(9, -2), complex(0, 2)}})
	));
}

BOOST_AUTO_TEST_CASE(tc_multiply)
{
	BOOST_CHECK_THROW((matrix{vvd{{1,2,3}, {4,5}}} * matrix{vvd{{1,2}, {4, 5, 6}}}), std::runtime_error);

	BOOST_TEST(( matrix({{complex(1, 2)}}) * matrix({{complex(3, 4)}}) == matrix({{complex(-5, 10)}}) ));
	BOOST_TEST(( matrix({{complex(1, 2), complex(3, 4)}}) * matrix({{complex(5, 6), complex(7, 8)}}) == matrix({{complex(-7, 16), complex(-11, 52)}}) ));
	BOOST_TEST(( matrix({{complex(1, 2)}, {complex(3, 4)}}) * matrix({{complex(5, 6)}, {complex(7, 8)}}) == matrix({{complex(-7, 16)}, {complex(-11, 52)}}) ));

	BOOST_TEST((
		matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) *
		matrix({{complex(8, 7), complex(6, 5), complex(4, 3)}, {complex(2, 1), complex(0, 12), complex(11, 10)}}) ==
		matrix({{complex(-6, 23), complex(-2, 39), complex(2, 39)}, {complex(6, 23), complex(-120, 108), complex(1, 242)}})
	));
}

BOOST_AUTO_TEST_CASE(tc_divide)
{
	BOOST_CHECK_THROW((matrix{vvd{{1,2,3}, {4,5}}} / matrix{vvd{{1,2}, {4, 5, 6}}}), std::runtime_error);
	BOOST_CHECK_THROW(( matrix{vvd{{1, 2, 3}}} / matrix{vvd{{1, 2, 0}}} ), std::runtime_error);

	BOOST_TEST(( matrix({{complex(1, 2)}}) / matrix({{complex(3, 4)}}) == matrix({{complex(0.44, 0.08)}}) ));
	BOOST_TEST(( matrix({{complex(1, 2), complex(3, 4)}}) / matrix({{complex(5, 6), complex(7, 8)}}) == matrix({{complex(17./61, 4./61), complex(53./113, 4./113)}}) ));
	BOOST_TEST(( matrix({{complex(1, 2)}, {complex(3, 4)}}) / matrix({{complex(5, 6)}, {complex(7, 8)}}) == matrix({{complex(17./61, 4./61)}, {complex(53./113, 4./113)}}) ));

	BOOST_TEST((
		matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) /
		matrix({{complex(8, 7), complex(6, 5), complex(4, 3)}, {complex(2, 1), complex(0, 12), complex(11, 10)}}) ==
		matrix({{complex(22./113, 9./113), complex(38./61, 9./61), complex(1.52, 0.36)}, {complex(4.4, 1.8), complex(5./6, -0.75), complex(241./221, 22./221)}})
	));

}

BOOST_AUTO_TEST_CASE(tc_modulo)
{
	BOOST_CHECK_THROW((matrix{vvd{{1,2,3}, {4,5}}} % matrix{vvd{{1,2}, {4, 5, 6}}}), std::runtime_error);
	BOOST_CHECK_THROW(( matrix{vvd{{1, 2, 3}}} % matrix{vvd{{1, 2, 0}}} ), std::runtime_error);

	BOOST_TEST(( matrix({{complex(1, 2)}}) % matrix({{complex(3, 4)}}) == matrix({{complex(1, 2)}}) ));
	BOOST_TEST(( matrix({{complex(1, 2), complex(3, 4)}}) % matrix({{complex(5, 6), complex(7, 8)}}) == matrix({{complex(1, 2), complex(3, 4)}}) ));
	BOOST_TEST(( matrix({{complex(1, 2)}, {complex(3, 4)}}) % matrix({{complex(5, 6)}, {complex(7, 8)}}) == matrix({{complex(1, 2)}, {complex(3, 4)}}) ));

	BOOST_TEST((
		matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) %
		matrix({{complex(8, 7), complex(6, 5), complex(4, 3)}, {complex(2, 1), complex(0, 12), complex(11, 10)}}) ==
		matrix({{complex(1, 2), complex(-3, -1), complex(-3)}, {complex(1), complex(-3, -2), complex(0, 2)}})
	));
}

BOOST_AUTO_TEST_SUITE_END() // ts_term_to_term


// Scalar Operations
BOOST_AUTO_TEST_SUITE(ts_scalar)

BOOST_AUTO_TEST_CASE(tc_plus)
{
	BOOST_TEST((
		matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) + complex(6, 6) ==
		matrix({{complex(7, 8), complex(9, 10), complex(11, 12)}, {complex(13, 14), complex(15, 16), complex(17, 18)}})
	));
	BOOST_TEST((
		complex(6, 6) + matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) ==
		matrix({{complex(7, 8), complex(9, 10), complex(11, 12)}, {complex(13, 14), complex(15, 16), complex(17, 18)}})
	));
}

BOOST_AUTO_TEST_CASE(tc_minus)
{
	BOOST_TEST((
		matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) - complex(6, 6) ==
		matrix({{complex(-5, -4), complex(-3, -2), complex(-1, 0)}, {complex(1, 2), complex(3, 4), complex(5, 6)}})
	));
	BOOST_TEST((
		complex(6, 6) - matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) ==
		matrix({{complex(5, 4), complex(3, 2), complex(1, 0)}, {complex(-1, -2), complex(-3, -4), complex(-5, -6)}})
	));
}

BOOST_AUTO_TEST_CASE(tc_multiply)
{
	BOOST_TEST((
		matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) * complex(6, 6) ==
		matrix({{complex(-6, 18), complex(-6, 42), complex(-6, 66)}, {complex(-6, 90), complex(-6, 114), complex(-6, 138)}})
	));

	BOOST_TEST((
		complex(6, 6) * matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) ==
		matrix({{complex(-6, 18), complex(-6, 42), complex(-6, 66)}, {complex(-6, 90), complex(-6, 114), complex(-6, 138)}})
	));
}

BOOST_AUTO_TEST_CASE(tc_divide)
{
	BOOST_CHECK_THROW((matrix{vvd{{1, 2}}} / complex{}), std::runtime_error);

	BOOST_TEST((
		matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) / complex(6, 6) ==
		matrix({{complex(1./4, 1./12), complex(7./12, 1./12), complex(11./12, 1./12)}, {complex(5./4, 1./12), complex(19./12, 1./12), complex(23./12, 1./12)}})
	));

	BOOST_TEST((
		complex(6, 6) / matrix({{complex(1, 2), complex(3, 4)}, {complex(5, 6), complex(7, 8)}}) ==
		matrix({{complex(18./5, -6./5), complex(42./25, -6./25)}, {complex(66./61, -6./61), complex(90./113, -6./113)}})
	));
}

BOOST_AUTO_TEST_CASE(tc_modulo)
{
	BOOST_CHECK_THROW((matrix{vvd{{1, 2}}} % complex{}), std::runtime_error);

	BOOST_TEST((
		matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) % complex(6, 6) ==
		matrix({{complex(1, 2), complex(-3, -2), complex(-1)}, {complex(1, 2), complex(-3, -2), complex(-1)}})
	));

	BOOST_TEST((
		complex(6, 6) % matrix({{complex(1, 2), complex(3, 4)}, {complex(5, 6), complex(7, 8)}}) ==
		matrix({{complex(0, -1), complex(0, -2)}, {complex(1), complex(-1, -2)}})
	));
}

BOOST_AUTO_TEST_CASE(tc_negate)
{
	BOOST_TEST((
		-matrix({{complex(1, 2), complex(3, 4), complex(5, 6)}, {complex(7, 8), complex(9, 10), complex(11, 12)}}) ==
		matrix({{complex(-1, -2), complex(-3, -4), complex(-5, -6)}, {complex(-7, -8), complex(-9, -10), complex(-11, -12)}})
	));
}

BOOST_AUTO_TEST_SUITE_END() // ts_term_to_term


BOOST_AUTO_TEST_CASE(ts_matrix_mul)
{
	BOOST_CHECK_THROW((
		mt_mul(
			matrix(vvd{{1, 2, 3}, {4, 5, 6}}),
			matrix(vvd{{1, 2}, {3, 4}})
		)
	), std::runtime_error);

	BOOST_TEST((
		mt_mul(
			matrix(vvd{{1, 2, 3}, {4, 5, 6}}),
			matrix(vvd{{1, 2}, {3, 4}, {5, 6}})
		) == matrix(vvd{{22, 28}, {49, 64}})
	));
}

BOOST_AUTO_TEST_SUITE_END() // ts_matrix