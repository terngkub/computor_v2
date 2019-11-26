#include "matrix.hpp"
#include <iostream>

void matrix_add()
{
    computorv2::matrix a{{{1, 2}, {3, 4}}};
    computorv2::matrix b{{{5, 6}, {7, 8}}};

    auto c = a.matrix_add(b);

    std::cout << "Test Matrix Additiion\n";
    std::cout << a << '\n';
    std::cout << b << '\n';
    std::cout << c << '\n';
}

void matrix_sub()
{
    computorv2::matrix a{{{1, 2}, {3, 4}}};
    computorv2::matrix b{{{5, 6}, {7, 8}}};

    auto c = a.matrix_sub(b);

    std::cout << "Test Matrix Subtraction\n";
    std::cout << a << '\n';
    std::cout << b << '\n';
    std::cout << c << '\n';
}

void matrix_mul()
{
    computorv2::matrix a{{{1, 2}, {3, 4}, {5, 6}}};
    computorv2::matrix b{{{7, 8, 9}, {10, 11, 12}}};

    auto c = a.matrix_mul(b);

    std::cout << "Test Matrix Multiplication\n";
    std::cout << a << '\n';
    std::cout << b << '\n';
    std::cout << c << '\n';
}

void scalar_mul()
{
    computorv2::matrix a{{{1, 2}, {3, 4}, {5, 6}}};
    double b = 4.2;

    auto c = a.scalar_mul(b);

    std::cout << "Test Matrix-Scalar Multiplication\n";
    std::cout << a << '\n';
    std::cout << b << '\n';
    std::cout << c << '\n';
}

void scalar_div()
{
    computorv2::matrix a{{{1, 2}, {3, 4}, {5, 6}}};
    double b = 4.2;

    auto c = a.scalar_div(b);

    std::cout << "Test Matrix-Scalar Division\n";
    std::cout << a << '\n';
    std::cout << b << '\n';
    std::cout << c << '\n';
}

int main()
{
    matrix_add();
    matrix_sub();
    matrix_mul();
    scalar_mul();
    scalar_div();
}