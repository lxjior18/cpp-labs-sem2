/**
 * Лабораторная работа 2
 *
 * Вариант 14
 * Вычислите интеграл по указанной формуле 1, формуле 2. Количество разбиений
 * отрезка интегрирования ≥ 10. Сравните получившиеся значения, вычислите
 * разницу.
 *
 * Интеграл: от функции x*(2 + x)*exp(x)
 * Методы: Правых прямоугольников, Симпсона
 */

#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>

using namespace std;

constexpr const char *RED = "\033[31m";
constexpr const char *GREEN = "\033[32m";
constexpr const char *BLUE = "\033[34m";
constexpr const char *CYAN = "\033[36m";
constexpr const char *RESET = "\033[0m";

double f(double x) { return x * (2 + x) * exp(x); }

double F(double x) { return exp(x) * x * x; }

class IntegralCalculator {
   private:
    function<double(double)> f;

   public:
    IntegralCalculator(function<double(double)> _f) : f(_f) {}

    double calcSimpson(double a, double b, int n) {
        n = n % 2 == 0 ? n : n + 1;

        double s = f(a) + f(b);
        double h = (b - a) / n;

        for (int i = 1; i < n; ++i) {
            double x = a + i * h;
            double factor = i % 2 == 0 ? 2 : 4;

            s += factor * f(x);
        }

        return s * h / 3.0;
    }

    double calcRectRight(double a, double b, int n) {
        double s = 0.0;
        double h = (b - a) / n;

        for (int i = 1; i <= n; ++i) {
            double x = a + i * h;
            s += f(x);
        }

        return s * h;
    }
};

int main(void) {
    setlocale(LC_ALL, "RU");

    double a = 0.0, b = 1.0;
    int n;

    IntegralCalculator calculator(f);

    cout << "Укажите количество разбиений (не меньше 10): ";
    cin >> n;

    double exactCalcRes = F(b) - F(a);

    double simpsonCalcRes = calculator.calcSimpson(a, b, n);
    double rectRightCalcRes = calculator.calcRectRight(a, b, n);

    vector<pair<string, double>> diffs = {
        {"Симпсон и правые прямоугольники", fabs(simpsonCalcRes - rectRightCalcRes)},
        {"Симпсон и точное значение", fabs(simpsonCalcRes - exactCalcRes)},
        {"Правые прямоугольники и точное значение", fabs(rectRightCalcRes - exactCalcRes)},
    };

    cout << setprecision(15);

    cout << BLUE << "Точное решение: " << RESET << exactCalcRes << "\n";
    cout << CYAN << "Расчет методом Симпсона: " << RESET << simpsonCalcRes << "\n";
    cout << GREEN << "Расчет методом правых прямоугольников: " << RESET << rectRightCalcRes
         << "\n\n";

    cout << RED << "Разницы между методами:" << RESET << endl;

    for (const auto &[label, v] : diffs) {
        cout << "   " << label << ": " << v << endl;
    };

    double min_diff = INFINITY;
    string method_name;

    for (auto v = diffs.begin() + 1; v != diffs.end(); ++v) {
        if (v->second < min_diff) {
            min_diff = v->second;
            method_name = v->first;
        }
    }

    cout << "\n" << RED << "Наименьшая разница для: " << RESET << method_name << endl;

    return 0;
}
