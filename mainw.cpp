#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <cctype>

// Определение функции
double f(double x) {
    if (x >= -7.0 && x <= -6) {
        return 1;
    }
    else if (x > -6 && x <= -4) {
        return -x / 2 - 2;
    }
    else if (x > -4 && x <= 0) {
        return sqrt(4 - pow(x + 2, 2));
    }
    else if (x > 0 && x <= 2) {
        return -sqrt(1 - pow(x - 1, 2));
    }
    else if (x > 2 && x <= 3) {
        return -x + 2;
    }
    else {
        return 0; // Вне интервала
    }
}

// Функция для ввода одного слова с проверкой
double getSingleWordInput(const std::string& prompt) {
    std::string input;
    double value;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        // Проверка на количество слов
        std::istringstream iss(input);
        std::string word;
        if (!(iss >> word) || (iss >> word)) {
            std::cout << "Неправильное количество слов. Пожалуйста, введите одно число.\n";
            continue;
        }

        // Проверка на корректность числа
        bool valid = true;
        bool hasDot = false;
        for (size_t i = 0; i < input.size(); ++i) {
            char c = input[i];

            if (i == 0) {
                // Первый символ может быть '-' или цифрой, но не '0' (если нет знака '-')
                if (c == '-') {
                    if (input.size() == 1 || !isdigit(input[1])) {
                        valid = false;
                        break;
                    }
                }
                else if (c == '0' && input.size() > 1 && input[1] != '.') {
                    valid = false;
                    break;
                }
                else if (!isdigit(c)) {
                    valid = false;
                    break;
                }
            }
            else {
                // Остальные символы могут быть цифрами, точкой (одна), но '-' недопустим
                if (c == '.') {
                    if (hasDot || i == input.size() - 1) {
                        valid = false;
                        break;
                    }
                    hasDot = true;
                }
                else if (!isdigit(c)) {
                    valid = false;
                    break;
                }
            }
        }

        if (!valid) {
            std::cout << "Некорректное число. Попробуйте снова.\n";
            continue;
        }

        std::istringstream(input) >> value; // Конвертируем слово в число
        return value;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    double Xнач, Xкон, dx;

    while (true) {
        // Ввод значений с проверкой на количество слов и корректность числа
        Xнач = getSingleWordInput("Введите начальное значение X: ");
        Xкон = getSingleWordInput("Введите конечное значение X: ");
        dx = getSingleWordInput("Введите шаг dx: ");

        // Проверка на корректность введенных значений
        if (Xкон <= Xнач) {
            std::cout << "Некорректные значения: конечное значение X должно быть больше начального значения.\n";
            continue;
        }
        if (dx <= 0 || dx >= Xкон-Xнач ) {
            std::cout << "Некорректное значение шага dx\n";
            continue;
        }

        break; // Если проверки пройдены, выходим из цикла
    }

    // Заголовок таблицы
    std::cout << std::setw(7) << "x" << std::setw(13) << "f(x)" << std::endl;

    // Вычисление и вывод значений функции
    for (double x = Xнач; x <= Xкон; x += dx) {
        std::cout << std::setw(10) << std::fixed << std::setprecision(2) << x
            << std::setw(10) << std::fixed << std::setprecision(2) << f(x)
            << std::endl;
    }

    return 0;
}
