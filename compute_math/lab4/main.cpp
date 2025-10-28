#include <iostream>
#include <math.h>
#include <stdlib.h>


double f(double x)
{
    return x - 0.6 * (sinh(x) - x + 1);
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

double iter(double x0, double eps, double delta, int& n)
{
    double x1 = Round(f(x0), delta);
    double x2 = Round(f(x1), delta);

    n = 2;

    while ((x1 - x2) * (x1 - x2) > fabs((2 * x1 - x0 - x2) * eps))
    {
        x0 = x1;
        x1 = x2;
        x2 = Round(f(x1), delta);
        n++;
    }

    return x2;
}

int main()
{
    double x, eps, delta;
    int n;
    double res;

    std::cout << "Введите первое приближение" << "\n";
    std::cin >> x;
    std::cout << "\n";

    std::cout << "Подсчет количества итераций (скорости сходимости)" << "\n";
    for (int i = 1; i <= 6; i++)
    {
        eps = std::pow(10, -i);

        res = iter(x, eps, 0.0, n);

        std::cout << eps << " " << n << "\n";
    }
    std::cout << "\n";

    std::cout << "Исследование обусловленности" << "\n";
    for (int i = 1; i <= 6; i++)
    {
        delta = std::pow(10, -i);

        for (int j = i; j <= 6; j++)
        {
            eps = std::pow(10, -j);

            res = iter(x, eps, delta, n);

            std::cout << delta << " " << eps << " " << res << "\n";
        }
    }
    std::cout << "\n";

    return 0;
}