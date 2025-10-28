#include <iostream>
#include <math.h>
#include <stdlib.h>

double f(double x)
{
    return sinh(x) - x + 1;
}

double f_deriv(double x)
{
    return cosh(x) - 1;
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

double newton(double x, double eps, double delta, int& n)
{
    double y, y_deriv, dx;

    n = 0;

    do
    {
        y = Round(f(x), delta);

        if (y == 0.0)
        {
            return x;
        }

        y_deriv = Round(f_deriv(x), delta);

        if (y_deriv == 0.0)
        {
            std::cout << "Производная обратилась в ноль" << "\n";
            exit(1);
        }

        dx = y / y_deriv;
        x = x - dx;
        n++;
    }
    while (fabs(dx) > eps);
    
    return x;
}

int main()
{
    double x, eps, eps0, delta;
    int n;
    double res;

    std::cout << "Введите первое приближение" << "\n";
    std::cin >> x;
    std::cout << "\n";

    double m1 = abs(cosh(-1) - 1);
    double M2 = abs(sinh(-1));

    std::cout << "Подсчет количества итераций (скорости сходимости)" << "\n";
    for (int i = 1; i <= 6; i++)
    {
        eps = std::pow(10, -i);
        eps0 = sqrt((2 * m1 * eps) / M2);

        res = newton(x, eps0, 0.0, n);

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
            eps0 = sqrt((2 * m1 * eps) / M2);

            res = newton(x, eps0, delta, n);

            std::cout << delta << " " << eps << " " << res << "\n";
        }
    }
    std::cout << "\n";

    return 0;
}