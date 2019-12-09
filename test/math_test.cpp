#include "math.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <cmath>

namespace utf = boost::unit_test;
namespace bdata = boost::unit_test::data;
namespace math = computorv2::math;

BOOST_AUTO_TEST_SUITE(ts_math)

BOOST_AUTO_TEST_CASE(tc_fequal)
{
    BOOST_TEST((math::fequal(1.2 * 3.4 * 5.6, 22.848)));
    BOOST_TEST((math::fequal(5.6 * 7.8 * 9.0, 393.12)));
}

BOOST_DATA_TEST_CASE(tc_floor,
    bdata::make({0.0, 0.01, -0.01, 3.1416, -3.1416}),
    nb)
{
    BOOST_TEST(math::floor(nb) == std::floor(nb));
}

BOOST_DATA_TEST_CASE(tc_ceil,
    bdata::make({0.0, 0.01, -0.01, 3.1416, -3.1416}),
    nb)
{
    BOOST_TEST(math::ceil(nb) == std::ceil(nb));
}

BOOST_DATA_TEST_CASE(tc_round,
    bdata::make({-3.7, -3.5, -3.1, -3.0, -1.0, -0.8, -0.5, -0.3, 0.0, 0.3, 0.5, 0.8, 1.0, 3.1, 3.5, 3.7}),
    nb)
{
    BOOST_TEST(math::round(nb) == std::round(nb));
}

BOOST_AUTO_TEST_CASE(tc_fmod_exception)
{
    BOOST_CHECK_THROW(math::fmod(0, 0), std::exception);
}

BOOST_DATA_TEST_CASE(tc_fmod_normal,
    bdata::make({0.0, 3.1, -3.1}) * bdata::make({1.2, -1.2}),
    lhs, rhs)
{
    BOOST_TEST(math::fmod(lhs, rhs) == std::fmod(lhs, rhs));
}

BOOST_DATA_TEST_CASE(tc_power,
    bdata::make({0, 2, -2}) * bdata::make({0, 1, 3, -1, -3}),
    base, degree)
{
    BOOST_TEST((math::power(base, degree) == std::pow(base, degree)));
}

BOOST_AUTO_TEST_CASE(tc_sqrt_exception)
{
    BOOST_CHECK_THROW(math::sqrt(-1), std::exception);
}

BOOST_TEST_DECORATOR(* utf::tolerance(0.0000001))
BOOST_DATA_TEST_CASE(tc_sqrt_normal,
    bdata::make({0, 1, 4, 10}),
    nb)
{
    BOOST_TEST(math::sqrt(nb) == std::sqrt(nb));
}

BOOST_AUTO_TEST_SUITE_END()