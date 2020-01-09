#include "term.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ts_term)

using term = computorv2::term;
using complex = computorv2::complex;
using matrix = computorv2::matrix;

BOOST_AUTO_TEST_CASE(tc_str)
{
    BOOST_TEST((term{}.str() == "0"));
    BOOST_TEST((term{complex{1.23}}.str() == "1.23"));
    BOOST_TEST((term{complex{-1.23}}.str() == "-1.23"));
    BOOST_TEST((term{complex{1.23, -4.56}}.str() == "1.23 - 4.56i"));
    BOOST_TEST((term{complex{-1.23, 4.56}}.str() == "-1.23 + 4.56i"));

    BOOST_TEST((term{matrix{}}.str() == "[]"));
    BOOST_TEST((term{matrix{2, 3, complex{3.14}}}.str() == "[[3.14, 3.14, 3.14]; [3.14, 3.14, 3.14]]"));

    BOOST_TEST((term{"var"}.str()) == "var");
    BOOST_TEST((term{complex{-1.23}, "var"}.str()) == "-1.23var");
    BOOST_TEST((term{complex{-2, 3}, "var"}.str()) == "(-2 + 3i)var");
    BOOST_TEST((term{matrix{1, 2, complex{3.14}}, "var"}.str()) == "[[3.14, 3.14]] * var");
}

complex c0{};
complex c1{3.14};
complex c2{4};

matrix m1{2, 3, 2};
matrix m2{3, 2, 3};

term tc0{};
term tc1{c1};
term tc2{c2};

term tcx0{c0, "x"};
term tcx1{c1, "x"};
term tcx2{c2, "x"};
term tcy1{c1, "y"};

term tm1{m1};
term tm2{m2};

term tmx1{m1, "x"};
term tmx2{m2, "x"};


BOOST_AUTO_TEST_CASE(tc_add)
{
    BOOST_TEST(tc1 + tc2 == (term{complex{7.14}}));
    BOOST_TEST(tc1 + tm1 == (term{matrix{2, 3, 5.14}}));
    BOOST_TEST(tm1 + tc1 == (term{matrix{2, 3, 5.14}}));
    BOOST_TEST(tm1 + tm1 == (term{matrix{2, 3, 4}}));
    BOOST_CHECK_THROW(tm1 + tm2, std::runtime_error);

    BOOST_TEST(tc1 + tcx2 == (term{complex{7.14}, "x"}));
    BOOST_TEST(tc1 + tmx1 == (term{matrix{2, 3, 5.14}, "x"}));
    BOOST_TEST(tm1 + tcx1 == (term{matrix{2, 3, 5.14}, "x"}));
    BOOST_TEST(tm1 + tmx1 == (term{matrix{2, 3, 4}, "x"}));
    BOOST_CHECK_THROW(tcx1 + tcy1, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(tc_sub)
{
    BOOST_TEST(tc1 - tc2 == (term{complex{-0.86}}));
    BOOST_TEST(tc1 - tm1 == (term{matrix{2, 3, 1.14}}));
    BOOST_TEST(tm1 - tc1 == (term{matrix{2, 3, -1.14}}));
    BOOST_TEST(tm1 - tm1 == (term{matrix{2, 3, 0}}));
    BOOST_CHECK_THROW(tm1 - tm2, std::runtime_error);

    BOOST_TEST(tc1 - tcx2 == (term{complex{-0.86}, "x"}));
    BOOST_TEST(tc1 - tmx1 == (term{matrix{2, 3, 1.14}, "x"}));
    BOOST_TEST(tm1 - tcx1 == (term{matrix{2, 3, -1.14}, "x"}));
    BOOST_TEST(tm1 - tmx1 == (term{matrix{2, 3, 0}, "x"}));
    BOOST_CHECK_THROW(tcx1 - tcy1, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(tc_mul)
{
    BOOST_TEST(tc1 * tc2 == (term{complex{12.56}}));
    BOOST_TEST(tc1 * tm1 == (term{matrix{2, 3, 6.28}}));
    BOOST_TEST(tm1 * tc1 == (term{matrix{2, 3, 6.28}}));
    BOOST_TEST(tm1 * tm1 == (term{matrix{2, 3, 4}}));
    BOOST_CHECK_THROW(tm1 * tm2, std::runtime_error);

    BOOST_TEST(tc1 * tcx2 == (term{complex{12.56}, "x"}));
    BOOST_TEST(tc1 * tmx1 == (term{matrix{2, 3, 6.28}, "x"}));
    BOOST_TEST(tm1 * tcx1 == (term{matrix{2, 3, 6.28}, "x"}));
    BOOST_TEST(tm1 * tmx1 == (term{matrix{2, 3, 4}, "x"}));
    BOOST_CHECK_THROW(tcx1 * tcy1, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(tc_div)
{
    BOOST_TEST(tc1 / tc2 == (term{complex{0.785}}));
    BOOST_TEST(tc1 / tm1 == (term{matrix{2, 3, 1.57}}));
    BOOST_TEST(tm1 / tc1 == (term{matrix{2, 3, 2/3.14}}));
    BOOST_TEST(tm1 / tm1 == (term{matrix{2, 3, 1}}));
    BOOST_CHECK_THROW(tm1 / tm2, std::runtime_error);

    BOOST_TEST(tc1 / tcx2 == (term{complex{0.785}, "x"}));
    BOOST_TEST(tc1 / tmx1 == (term{matrix{2, 3, 1.57}, "x"}));
    BOOST_TEST(tm1 / tcx1 == (term{matrix{2, 3, 2/3.14}, "x"}));
    BOOST_TEST(tm1 / tmx1 == (term{matrix{2, 3, 1}, "x"}));
    BOOST_CHECK_THROW(tcx1 / tcy1, std::runtime_error);

    BOOST_CHECK_THROW(tc1 / tc0, std::runtime_error);
    BOOST_CHECK_THROW(tm1 / tc0, std::runtime_error);
    BOOST_CHECK_THROW(tcx1 / tc0, std::runtime_error);
    BOOST_CHECK_THROW(tmx1 / tc0, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(tc_mod)
{
    BOOST_TEST(tc1 % tc2 == (term{complex{3.14}}));
    BOOST_TEST(tc1 % tm1 == (term{matrix{2, 3, 1.14}}));
    BOOST_TEST(tm1 % tc1 == (term{matrix{2, 3, 2}}));
    BOOST_TEST(tm1 % tm1 == (term{matrix{2, 3, 0}}));
    BOOST_CHECK_THROW(tm1 % tm2, std::runtime_error);

    BOOST_TEST(tc1 % tcx2 == (term{complex{3.14}, "x"}));
    BOOST_TEST(tc1 % tmx1 == (term{matrix{2, 3, 1.14}, "x"}));
    BOOST_TEST(tm1 % tcx1 == (term{matrix{2, 3, 2}, "x"}));
    BOOST_TEST(tm1 % tmx1 == (term{matrix{2, 3, 0}, "x"}));
    BOOST_CHECK_THROW(tcx1 % tcy1, std::runtime_error);

    BOOST_CHECK_THROW(tc1 % tc0, std::runtime_error);
    BOOST_CHECK_THROW(tm1 % tc0, std::runtime_error);
    BOOST_CHECK_THROW(tcx1 % tc0, std::runtime_error);
    BOOST_CHECK_THROW(tmx1 % tc0, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(tc_neg)
{
    BOOST_TEST(-tc1 == term{complex{-3.14}});
    BOOST_TEST(-(-tc1) == tc1);
    BOOST_TEST(-tm1 == (term{matrix{2, 3, -2}}));
}

BOOST_AUTO_TEST_CASE(tc_degree)
{
    BOOST_CHECK_THROW(tc1.degree(), std::runtime_error);
    BOOST_TEST(tc2.degree() == 4);
    BOOST_TEST(tc0.degree() == 0);
    BOOST_TEST(term{-3}.degree() == -3);
    BOOST_CHECK_THROW((term{complex{3, 2}}).degree(), std::runtime_error);
    BOOST_CHECK_THROW(tm1.degree(), std::runtime_error);
    BOOST_CHECK_THROW(tcx1.degree(), std::runtime_error);
    BOOST_CHECK_THROW(tmx1.degree(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(tc_matrix_mul)
{
    BOOST_TEST((term_matrix_mul(tm1, tm2)) == (term{matrix{2, 2, 18}}));
    BOOST_CHECK_THROW((term_matrix_mul(tmx1, tm2)), std::runtime_error);
    BOOST_CHECK_THROW((term_matrix_mul(tm1, tc1)), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()