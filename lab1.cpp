/**
 * Лабораторная работа 1
 *
 * Вариант 14
 * Напишите программу для численного решения уравнения по методу 1, методу 2
 * согласно варианту с заданной точностью. Определите количество итераций для
 * каждого метода. Какой метод решил уравнение быстрее? Сравните полученные
 * ответы (см. Ответы), вычислите разницу.
 *
 * Уравнение: pow(x, 3) - 2 * pow(x, 2) - 10 * x + 15
 * Методы: Ньютона, секущих
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
constexpr int CALC_METHODS_MAX_ITERATIONS = 1000;

double func(double x) { return pow(x, 3) - 2 * pow(x, 2) - 10 * x + 15; };

double func_deriv(double x) { return 3 * pow(x, 2) - 4 * x - 10; };

class EquationCalculator {
   private:
    function<double(double)> f;
    function<double(double)> f_deriv;
    vector<pair<double, double>> intervals;

    void checkIntervalNum(int interNum) {
        if (interNum < 0 || interNum >= intervals.size()) {
            throw runtime_error("Номер интервала некорректен");
        }
    }

   public:
    vector<pair<string, int>> iterationsCountMap;

    EquationCalculator(function<double(double)> _f, function<double(double)> _f_deriv)
        : f(_f), f_deriv(_f_deriv) {}

    vector<pair<double, double>> findRootIntervals(double a, double b, int m) {
        intervals.clear();
        double h = (b - a) / m;

        for (int i = 0; i < m; ++i) {
            double start = a + i * h;
            double end = start + h;

            if (f(start) * f(end) < 0) {
                intervals.emplace_back(start, end);
            }
        }

        return intervals;
    };

    double startChordMethod(int interNum, double epsilon = 1e-6) {
        checkIntervalNum(interNum);

        int n = 0;
        double a = intervals[interNum].first, b = intervals[interNum].second;

        while (fabs(f(b)) > epsilon && n < CALC_METHODS_MAX_ITERATIONS) {
            double c = b - f(b) * (b - a) / (f(b) - f(a));

            a = b;
            b = c;

            n++;
        }

        cout << "Метод хорд завершен за " << RED << n << RESET << " итераций" << endl;
        iterationsCountMap.emplace_back("Метод хорд", n);

        return b;
    };

    double startNewtonMethod(int interNum, double epsilon = 1e-6) {
        checkIntervalNum(interNum);

        int n = 0;
        double x0 = (intervals[interNum].first + intervals[interNum].second) / 2.0;

        while (fabs(f(x0)) > epsilon && n < CALC_METHODS_MAX_ITERATIONS) {
            double deriv_val = f_deriv(x0);

            if (fabs(deriv_val) < 1e-11) {
                throw runtime_error("Слишком маленькое значение производной");
            }

            x0 = x0 - f(x0) / deriv_val;
            n++;
        }

        cout << "Метод Ньютона завершен за " << RED << n << RESET << " итераций" << endl;
        iterationsCountMap.emplace_back("Метод Ньютона", n);

        return x0;
    }

    double startBisectionMethod(int interNum, double epsilon = 1e-6) {
        checkIntervalNum(interNum);

        double a = intervals[interNum].first, b = intervals[interNum].second;
        double c = a;
        int n = 0;

        while (fabs(f(c)) > epsilon && n < CALC_METHODS_MAX_ITERATIONS) {
            c = (a + b) / 2.0;

            if (f(c) * f(a) < 0) {
                b = c;
            } else {
                a = c;
            }

            n++;
        }

        cout << "Метод бисекции завершен за " << RED << n << RESET << " итераций" << endl;
        iterationsCountMap.emplace_back("Метод бисекции", n);

        return c;
    }

    ~EquationCalculator() = default;
};

int main(void) {
    setlocale(LC_ALL, "RU");
    cout << setprecision(15);

    double a, b;
    int m;
    cout << "Введите отрезок поиска корня [a, b]: ";
    cin >> a >> b;
    cout << "Введите количество разбиений M: ";
    cin >> m;

    EquationCalculator calculator(func, func_deriv);

    vector<pair<double, double>> foundIntervals = calculator.findRootIntervals(a, b, m);

    cout << GREEN << "Отрезки, содержащие найденный корень:" << RESET << endl;

    if (foundIntervals.empty()) {
        cout << RED << "Отрезков с корнями нет" << RESET << endl;
        return 0;
    }

    for (int i = 0; i < foundIntervals.size(); ++i) {
        cout << i + 1 << ") Корень найден на отрезке: [" << foundIntervals[i].first << ", "
             << foundIntervals[i].second << "]" << endl;
    }

    cout << endl;

    int intervalNum;
    cout << "Выберите отрезок для поиска корня: ";
    cin >> intervalNum;
    intervalNum -= 1;

    cout << CYAN << "Выбран отрезок: [" << foundIntervals[intervalNum].first << ", "
         << foundIntervals[intervalNum].second << "]" << RESET << "\n\n";

    cout << BLUE << "Метод хорд" << RESET << endl;
    double rootByChordMethod = calculator.startChordMethod(intervalNum);
    cout << "   Значение корня: " << rootByChordMethod << "\n\n";

    cout << BLUE << "Метод Ньютона" << RESET << endl;
    double rootByNewtonMethod = calculator.startNewtonMethod(intervalNum);
    cout << "   Значение корня: " << rootByNewtonMethod << "\n\n";

    cout << BLUE << "Метод бисекции" << RESET << endl;
    double rootByBisectionMethod = calculator.startBisectionMethod(intervalNum);
    cout << "   Значение корня: " << rootByBisectionMethod << "\n\n";

    string highSpeedMethodName;
    int minIters = numeric_limits<int>::max();

    for (const auto &[name, n] : calculator.iterationsCountMap) {
        cout << name << " потребовал " << RED << n << RESET << " итераций\n";

        if (minIters > n) {
            minIters = n;
            highSpeedMethodName = name;
        }
    }

    cout << GREEN << "\nСамый быстрый метод: " << highSpeedMethodName << RESET << "\n";

    return 0;
}
