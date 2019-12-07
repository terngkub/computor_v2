#include "complex.hpp"
#include <boost/test/unit_test.hpp>

using complex = computorv2::complex;

BOOST_AUTO_TEST_SUITE(complex_test)

BOOST_AUTO_TEST_CASE(printing)
{
    BOOST_TEST((complex{}).str() == "0");

    BOOST_TEST((complex{1}).str() == "1");
    BOOST_TEST((complex{2}).str() == "2");
    BOOST_TEST((complex{-1}).str() == "-1");

    BOOST_TEST((complex{0, 1}).str() == "i");
    BOOST_TEST((complex{0, 2}).str() == "2i");
    BOOST_TEST((complex{0, -1}).str() == "-i");

    BOOST_TEST((complex{1, 1}).str() == "1 + i");
    BOOST_TEST((complex{1, -1}).str() == "1 - i");

    BOOST_TEST((complex{2, 3}).str() == "2 + 3i");
    BOOST_TEST((complex{2, -3}).str() == "2 - 3i");
}

BOOST_AUTO_TEST_CASE(square_root)
{
    BOOST_TEST(complex_sqrt(complex{}) == 0);
    BOOST_TEST(complex_sqrt(complex{4}) == 2);
    BOOST_TEST(complex_sqrt(complex{-4}) == (complex{0, 2}));

    BOOST_TEST(complex_sqrt(complex{0, 2}) == (complex{1, 1}));
    BOOST_TEST(complex_sqrt(complex{0, -2}) == (complex{1, -1}));

    BOOST_TEST(complex_sqrt(complex{5, 12}) == (complex{3, 2}));
    BOOST_TEST(complex_sqrt(complex{5, -12}) == (complex{3, -2}));
}

BOOST_AUTO_TEST_SUITE_END()
