#include "complex.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <cmath>

namespace utf = boost::unit_test;
using complex = computorv2::complex;

BOOST_AUTO_TEST_SUITE(ts_complex)

BOOST_AUTO_TEST_CASE(tc_addition)
{
    BOOST_TEST((complex{} + complex{} == complex{}));
    BOOST_TEST((complex{4} + complex{-4} == complex{}));
    BOOST_TEST((complex{4} + complex{0, -4} == complex{4, -4}));
    BOOST_TEST((complex{2, 3} + complex{4, -5} == complex{6, -2}));
}

BOOST_AUTO_TEST_CASE(tc_subtraction)
{
    BOOST_TEST((complex{} - complex{} == complex{}));
    BOOST_TEST((complex{4} - complex{-4} == complex{8}));
    BOOST_TEST((complex{4} - complex{0, -4} == complex{4, 4}));
    BOOST_TEST((complex{2, 3} - complex{4, -5} == complex{-2, 8}));
}

BOOST_AUTO_TEST_CASE(tc_multiplication)
{
    BOOST_TEST((complex{} * complex{} == complex{}));

    BOOST_TEST((complex{3.14} * 0 == complex{}));
    BOOST_TEST((0 * complex{3.14} == complex{}));
    BOOST_TEST((complex{3.14} * 5.6 == complex{17.584}));
    BOOST_TEST((complex{3.14} * -5.6 == complex{-17.584}));

    BOOST_TEST((complex{0, 3.14} * 0 == complex{}));
    BOOST_TEST((0 * complex{0, 3.14} == complex{}));
    BOOST_TEST((complex{0, 3.14} * complex{0, 5.6} == complex{-17.584}));
    BOOST_TEST((complex{0, 3.14} * complex{0, -5.6} == complex{17.584}));

    BOOST_TEST((complex{3.14, 0} * complex{0, 5.6} == complex{0, 17.584}));
    BOOST_TEST((complex{-3.14, 0} * complex{0, -5.6} == complex{0, 17.584}));

    BOOST_TEST((complex{3.1, 4} * complex{5, 1.6} == complex{9.1, 24.96}));
    BOOST_TEST((complex{3.1, -4} * complex{-5, 1.6} == complex{-9.1, 24.96}));
}

BOOST_AUTO_TEST_CASE(tc_division)
{
    BOOST_CHECK_THROW((complex{} / complex{}), std::runtime_error);
    BOOST_CHECK_THROW((complex{1.2, 3.4} / complex{}), std::runtime_error);

    BOOST_TEST((complex{} / 10 == complex{}));
    BOOST_TEST((complex{4} / complex(10) == complex{0.4}));

    BOOST_TEST((complex{0, 3} / complex{0, 2} == complex{1.5}));
    BOOST_TEST((complex{0, -3} / complex{0, 2} == complex{-1.5}));

    BOOST_TEST((complex{3} / complex{0, 2} == complex{0, -1.5}));
    BOOST_TEST((complex{0, 3} / complex{2} == complex{0, 1.5}));

    BOOST_TEST((complex{2, 1} / complex{3, 1} == complex{0.7, 0.1}));
    BOOST_TEST((complex{-2, 1} / complex{3, -1} == complex{-0.7, 0.1}));
}

BOOST_AUTO_TEST_CASE(tc_remainder)
{
    BOOST_CHECK_THROW((complex{} % complex{}), std::runtime_error);
    BOOST_CHECK_THROW((complex{3.1} % complex{}), std::runtime_error);

    BOOST_TEST((complex{0} % complex{3.1}) == (complex{0}));
    BOOST_TEST((complex{3.1} % complex{-1.2}) == (complex{0.7}));
    BOOST_TEST((complex{0, 3.1} % complex{0, -1.2}) == (complex{0, -0.5}));

    BOOST_TEST((complex{26, 120} % complex{37, 226}) == (complex{-11, -106}));
    BOOST_TEST((complex{26, -120} % complex{37, -226}) == (complex{-11, 106}));
}

BOOST_AUTO_TEST_CASE(tc_negative)
{
    BOOST_TEST((-complex{} == complex{}));
    BOOST_TEST((-complex{1} == complex{-1}));
    BOOST_TEST((-complex{-1} == complex{1}));
    BOOST_TEST((-complex{0, 1} == complex{0, -1}));
    BOOST_TEST((-complex{0, -1} == complex{0, 1}));
    BOOST_TEST((-complex{1, 2} == complex{-1, -2}));
    BOOST_TEST((-complex{-1, 2} == complex{1, -2}));
    BOOST_TEST((-complex{1, -2} == complex{-1, 2}));
    BOOST_TEST((-complex{-1, -2} == complex{1, 2}));
}

BOOST_AUTO_TEST_CASE(tc_equality)
{
    BOOST_TEST((complex{} == 0));
    BOOST_TEST((complex{1} == 1));
    BOOST_TEST((complex{0, 1} == complex{0, 1}));
    BOOST_TEST((complex{1, 2} == complex{1, 2}));
    BOOST_TEST((complex{-1, -2} == complex{-1, -2}));
    BOOST_TEST((complex{1.2 * 3.4 * 5.6, 5.6 * 7.8 * 9.0} == complex{22.848, 393.12}));
    BOOST_TEST((complex{1.2 * 3.4 * -5.6, 5.6 * 7.8 * -9.0} == complex{-22.848, -393.12}));
}

BOOST_AUTO_TEST_CASE(tc_printing)
{
    BOOST_TEST((complex{}.str() == "0"));

    BOOST_TEST((complex{1}.str() == "1"));
    BOOST_TEST((complex{2}.str() == "2"));
    BOOST_TEST((complex{-1}.str() == "-1"));

    BOOST_TEST((complex{0, 1}.str() == "i"));
    BOOST_TEST((complex{0, 2}.str() == "2i"));
    BOOST_TEST((complex{0, -1}.str() == "-i"));

    BOOST_TEST((complex{1, 1}.str() == "1 + i"));
    BOOST_TEST((complex{1, -1}.str() == "1 - i"));

    BOOST_TEST((complex{2, 3}.str() == "2 + 3i"));
    BOOST_TEST((complex{2, -3}.str() == "2 - 3i"));
}

BOOST_AUTO_TEST_CASE(tc_round)
{
    BOOST_TEST((complex_round(complex{}) == complex{}));
    BOOST_TEST((complex_round(complex{1, 1}) == complex{1, 1}));
    BOOST_TEST((complex_round(complex{1.2, 1.2}) == complex{1, 1}));
    BOOST_TEST((complex_round(complex{1.5, 1.5}) == complex{2, 2}));
    BOOST_TEST((complex_round(complex{1.8, 1.8}) == complex{2, 2}));
    BOOST_TEST((complex_round(complex{-1, -1}) == complex{-1, -1}));
    BOOST_TEST((complex_round(complex{-1.2, -1.2}) == complex{-1, -1}));
    BOOST_TEST((complex_round(complex{-1.5, -1.5}) == complex{-2, -2}));
    BOOST_TEST((complex_round(complex{-1.8, -1.8}) == complex{-2, -2}));
}

BOOST_AUTO_TEST_CASE(tc_sqrt)
{
    BOOST_TEST((complex_sqrt(complex{}) == 0));
    BOOST_TEST((complex_sqrt(complex{4}) == 2));

    BOOST_TEST((complex_sqrt(complex{-4}) == complex{0, 2}));
    BOOST_TEST((complex_sqrt(complex{0, 2}) == complex{1, 1}));
    BOOST_TEST((complex_sqrt(complex{0, -2}) == complex{1, -1}));

    BOOST_TEST((complex_sqrt(complex{5, 12}) == complex{3, 2}));
    BOOST_TEST((complex_sqrt(complex{5, -12}) == complex{3, -2}));
}

BOOST_AUTO_TEST_SUITE_END()
