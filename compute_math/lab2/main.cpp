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

double horda(double left, double right, double eps, double delta, int& n)
{
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
    
    do
    {
	    x = left - (right - left) * fLeft / (fRight - fLeft);
	    y = Round(f(x), delta);
	    
        if (y == 0.0)
        {
            return x;
        }
        if (y * fLeft < 0.0)
        {
            right = x;
            fRight = y;
        }
        else
        {
            left = x;
            fLeft = y;
        }

        n++;
    }
    while (fabs(y) >= eps);
    
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

    std::cout << "Подсчет количества итераций (скорости сходимости)" << "\n";
    for (int i = 1; i <= 6; i++)
    {
        eps = std::pow(10, -i);

        res = horda(left, right, eps, 0.0, n);

        std::cout << eps << " " << n << "\n";
    }
    std::cout << "\n";

    std::cout << "Исследование обусловленности" << "\n";
    for (int i = 1; i <= 6; i++)
    {
        delta = std::pow(10, -i);

        for (int j = 1; j <= 6; j++)
        {
            eps = std::pow(10, -j);

            res = horda(left, right, eps, delta, n);

            std::cout << delta << " " << eps << " " << res << "\n";
        }
    }
    std::cout << "\n";

    return 0;
}