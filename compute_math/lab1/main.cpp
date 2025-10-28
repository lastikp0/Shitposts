#include <iostream>
#include <math.h>
#include <stdlib.h>

double f(double x)
{
    return sinh(x) - x + 1;
}

double Round(double x, double delta)
{
    if (delta == 0.0)
    {
        return x;
    }

    if (x > 0.0)
    {
        return delta * (long((x / delta) + 0.5));
    }
    else
    {
        return delta * (long((x / delta) - 0.5));
    }
}

double bisect(double left, double right, double eps, double delta, int& n)
{
    double e = fabs(eps) * 2.0;
    double fLeft = Round(f(left), delta);
    double fRight = Round(f(right), delta);

    double x, y;

    if (fLeft * fRight > 0.0)
    {
        std::cout << "Неверное задание интервала" << "\n";
        exit(1);
    }

    if (eps <= 0.0)
    {
        std::cout << "Неверное задание точности" << "\n";
        exit(1);
    }

    n = 0;

    if (fLeft == 0.0)
    {
        return left;
    }
    if (fRight == 0.0)
    {
        return right;
    }

    while ((right - left) >= e)
    {
        x = 0.5 * (right + left);
        y = Round(f(x), delta);

        if (y == 0.0)
        {
            return x;
        }

        if (y * fLeft < 0.0)
        {
            right = x;
        }
        else
        {
            left = x;
            fLeft = y;
        }

        n++;
    }

    return x;
}

int main()
{
    double left, right, eps, delta;
    int n;
    double res;

    std::cout << "Введите левую и правую границы отрезка через пробел" << "\n";
    std::cin >> left >> right;
    std::cout << "\n";

    std::cout << "Подсчет количества итераций" << "\n";
    for (int i = 1; i <= 6; i++)
    {
        eps = std::pow(10, -i);

        res = bisect(left, right, eps, 0.0, n);

        std::cout << eps << " " << n << "\n";
    }
    std::cout << "\n";

    std::cout << "Исследование обусловленности" << "\n";
    for (int i = 1; i <= 6; i++)
    {
        eps = std::pow(10, -i);

        for (int j = 1; j <= 6; j++)
        {
            delta = std::pow(10, -j);

            res = bisect(left, right, eps, delta, n);

            std::cout << eps << " " << delta << " " << res << "\n";
        }
    }
    std::cout << "\n";

    return 0;
}