#include "math.hpp"

double power(double base, int degree)
{
    if (degree == 0)
        return 1;

    bool neg = false;
    if (degree < 0)
    {
        degree *= -1;
        neg = true;
    }

    double ret = base;
    while (--degree)
        ret *= base;
    
    return (neg ? 1 / ret : ret);
}

double sqrt(double nb)
{
    if (nb == 0)
        return 0;

    double x = 0;
    while (x * x <= nb)
        x += 0.1;

    double prev_x = x + 1;
    while (prev_x - x >= 0.000001)
    {
        prev_x = x;
        x = (x + nb / x) / 2;
    }

    return x;
}