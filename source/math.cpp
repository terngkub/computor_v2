#include "math.hpp"
#include <stdexcept>
#include <limits>

namespace computorv2
{
namespace math
{

static inline double remove_decimal(double x)
{
    return static_cast<double>(static_cast<long long>(x));
}

double fabs(double x)
{
    return x > 0 ? x : -x;
}

bool fequal(double x, double y)
{
    return fabs(x - y) <= 0.000000001;
}

double floor(double x)
{
	if (x >= 0)
		return remove_decimal(x);
	return remove_decimal(x - 1);
}

double ceil(double x)
{
	if (x > 0)
		return remove_decimal(x + 1);
	return remove_decimal(x);
}

double round(double x)
{
    double no_decimal = remove_decimal(x);

    if (x < 0)
    {
        if (no_decimal - x >= 0.5)
            return no_decimal - 1;
        return no_decimal;
    }

    if (x - no_decimal >= 0.5)
        return no_decimal + 1;
    return no_decimal;
}

double fmod(double a, double b)
{
    if (b == 0)
        throw std::runtime_error("modulo by zero");
    if (a < 0 ^ b < 0)
        return a - b * ceil(a / b);
	return a - b * floor(a / b);
}

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
    if (nb < 0)
        throw std::runtime_error("sqrt on negative number");

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

} // namespace math
} // namespace computorv2