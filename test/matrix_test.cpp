#include "matrix.hpp"
#include <boost/test/unit_test.hpp>

using matrix = computorv2::matrix;

BOOST_AUTO_TEST_SUITE(ts_matrix)

BOOST_AUTO_TEST_CASE(tc_printing)
{
    BOOST_TEST((matrix{}.str() == "[]"));
    BOOST_TEST((matrix{1, 1}.str() == "[[0]]"));
    BOOST_TEST((matrix{2, 3}.str() == "[[0, 0, 0]; [0, 0, 0]]"));
    BOOST_TEST((matrix{std::vector<std::vector<double>>{{1, 2, 3}, {4, 5, 6}}}.str() == "[[1, 2, 3]; [4, 5, 6]]"));
}

BOOST_AUTO_TEST_SUITE_END()