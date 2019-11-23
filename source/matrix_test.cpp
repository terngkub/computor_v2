#include "matrix.hpp"
#include <iostream>

void add()
{
    computorv2::matrix a{{{1, 2}, {3, 4}}};
    computorv2::matrix b{{{5, 6}, {7, 8}}};

    auto c = a + b;

    std::cout << "Test Matrix Additiion\n";
    std::cout << a << '\n';
    std::cout << b << '\n';
    std::cout << c << '\n';
}

void mul()
{
    computorv2::matrix a{{{1, 2}, {3, 4}, {5, 6}}};
    computorv2::matrix b{{{7, 8, 9}, {10, 11, 12}}};

    auto c = a.multiply(b);

    std::cout << "Test Matrix Additiion\n";
    std::cout << a << '\n';
    std::cout << b << '\n';
    std::cout << c << '\n';
}

int main()
{
    mul();
}