# Лабораторная работа №3
## «Изучение циклических алгоритмов, операторов цикла, программирование циклического вычислительного процесса»
## по дисциплине: «Программирование»
### Цели и задачи работы: изучение циклических алгоритмов, операторов цикла, программирование циклического вычислительного процесса. Задание к работе: Реализовать циклический вычислительный процесс. Самостоятельно решить задачи в соответствии с индивидуальным вариантом.  
### Индивидуальные задания:
### Задание 1. Вычислить и вывести на экран или в файл в виде таблицы значения функции, заданной графически, на интервале от Xнач до Xкон с шагом dx. Интервал и шаг задать таким образом, чтобы проверить все ветви программы. Таблица должна иметь заголовком и шапку. 
### Вариант - 15
### Решение задачи на языке С++ в mainw.cpp
```C++
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
```
### Решение 1 задачи на Java в maine.txt

### Задание 2. Тесты на простоту (реализовать метод перебора делителей, тесты Миллера, Поклингтона, ГОСТ Р 34.10-94 
### Решение задачи на языке С++ в mainr.cpp, maina.cpp, maind.cpp
### Тест Миллера
```C++
#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>

using namespace std;

// Функция для генерации случайного числа в заданном диапазоне
int RandomGenerator(int min, int max) {
    random_device rd;  // источник случайных чисел
    mt19937 gen(rd()); // используем Вихрь Мерсена в качестве генератора
    uniform_int_distribution<int> dis(min, max); // распределение в указанном диапазоне
    return dis(gen);
}

// Простая функция для проверки простоты числа
bool IsPrime(int i, vector<int>& prime) { // передали число по значению и вектор по ссылке
    for (int j = 0; j < prime.size(); j++) { // проверяем на простоту
        if (i % prime[j] == 0) {
            return false;
        }
    }
    return true;
}

// Функция для вычисления функции Эйлера f(n)
double eulerTotient(int n) {
    int result = n;
    for (int p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0)
                n /= p;
            result -= result / p;
        }
    }
    if (n > 1)
        result -= result / n;
    return result;
}

vector<int> IsPrimeEratosfen(int p) { // проверка числа на простоту через решето эратосфена
    vector<int> primeNumb;
    primeNumb.push_back(2);
    for (int i = 3; i < p; i += 2) { // перебираем нечетные числа
        if (IsPrime(i, primeNumb) == 1) {
            primeNumb.push_back(i); // складываем в вектор простые числа
        }
    }
    return primeNumb;
}


// Функция для модульного возведения в степень
uint64_t mod(uint64_t number, uint64_t power, uint64_t n) {
    uint64_t ostatok = 1; //  Храним в ней модульное возведение в степень.
    while (power > 0) {
        if (power % 2 != 0) { // Если степень нечетная
            ostatok = (ostatok * number) % n;
        }
        power /= 2; // Делим степень на 2
        number = (number * number) % n; // Увеличиваем основание в квадрат и вычисляем по модулю
    }
    return ostatok;
}

// Функция для проверки теоремы Ферма
bool TeoremaFerma(int a, int x, int p) {
    uint64_t result = mod(a, x, p);
    return result == 1;
}

// Тест Миллера на простоту
bool MillerTest(int64_t n, const vector<int>& factors, int t) {
    vector<int> a;
    for (int i = 0; i < t; i++) {
        int aj = RandomGenerator(2, n - 1);
        if (TeoremaFerma(aj, n - 1, n) != 1) {
            return false; // n - составное число
        }

        int found = 0;
        for (int qj : factors) {
            if (TeoremaFerma(aj, (n - 1) / qj, n) == 1) {
                found++;
            }
        }

        if (found == factors.size()) {
            return false; // вероятно, n - составное число
        }
    }
    return true; // n - простое число
}

// Функция для вычисления количества бит в числе
int countBits(uint64_t number) {
    if (number == 0) return 1; // 0 занимает 1 бит
    return static_cast<int>(log2(number) + 1);
}

// Функция для проверки, является ли число меньше или больше заданного количества бит
int isWithinBitRange(uint64_t number, int bitCount, int formin) {
    int numBits = countBits(number);
    if (numBits < bitCount) return 1; // Число меньше заданного количества бит
    if (numBits > bitCount - formin) return 2; // Число больше заданного количества бит
    return 3; // Число равно заданному количеству бит
}

// Функция для создания числа и проведения тестов
bool createNum(int k, vector<int>& numbers, vector<string>& plusminus, vector<int>& oshibki, vector<double>& errorProbabilities, vector<int>& primes, atomic<bool>& shouldStop) {
    // Использование глобального вектора простых чисел
    vector<int> q; // Вектор для хранения простых множителей
    int64_t m = 1;
    int tempM = 1; // Временное значение для m
    int qi; // Случайный индекс простых чисел
    int ai; // Случайное целое число
    int64_t n;
    int targetBitCountM = k - 1; // Целевое количество бит для числа m
    int tempOshibki = 0;
    int formin = 0;
    if (k < 7 && k > 4) {
        formin = -1;
    }
    else if (k == 4) {
        formin = -2;
    }

    while (!shouldStop) { // генерация числа m в заданном диапазоне битов
        qi = primes[RandomGenerator(0, primes.size() / 10)]; // Выбор случайного простого числа из таблицы
        ai = RandomGenerator(1, 12);
        tempM *= pow(qi, ai); // Умножение текущего значения tempM на qi в степени ai
        if (isWithinBitRange(tempM, targetBitCountM, formin) == 2) { // Проверка, не превышает ли tempM максимальное значение для k битов
            m = 1;
            tempM = 1;
            q.clear();
        }
        else if (isWithinBitRange(tempM, targetBitCountM, formin) == 1) { // Если tempM меньше минимального значения для k битов, продолжаем генерировать
            q.push_back(qi); // Добавляем qi в вектор q
        }
        else {
            m = tempM;
            q.push_back(qi);
            n = 2 * m + 1;
            if (MillerTest(n, q, 1)) {
                break;
            }
            else {
                tempOshibki++;
                m = 1;
                tempM = 1;
                n = 0;
                q.clear();
            }
        }
    }

    if (shouldStop) return false;



    // Проверка на уникальность числа и повторная генерация, если число уже существует
    while (!numbers.empty() && find(numbers.begin(), numbers.end(), n) != numbers.end()) {
        // Генерация нового числа m и n
        m = 1;
        tempM = 1;
        q.clear();
        while (!shouldStop) { // генерация числа m в заданном диапазоне битов
            qi = primes[RandomGenerator(0, primes.size() / 10)]; // генерация индекса случайных простых чисел
            ai = RandomGenerator(1, 12); // генерация случайных целых чисел
            tempM *= pow(qi, ai);
            if (isWithinBitRange(tempM, targetBitCountM, formin) == 2) {
                m = 1;
                tempM = 1;
                q.clear();
            }
            else if (isWithinBitRange(tempM, targetBitCountM, formin) == 1) {
                q.push_back(qi);
            }
            else {
                m = tempM;
                q.push_back(qi);
                n = 2 * m + 1;
                if (MillerTest(n, q, 1)) {
                    break;
                }
                else {
                    tempOshibki++;
                    m = 1;
                    tempM = 1;
                    n = 0;
                    q.clear();
                }
            }
        }
        if (shouldStop) return false;
    }

    if (shouldStop) return false;

    int tempOshibki2 = 0;
    int i;
    int iter = 100;
    while (!shouldStop) {
        // Проверяем n на простоту с помощью теста Миллера
        for (i = 0; i < iter; i++) {
            if (!MillerTest(n, q, 1)) {
                tempOshibki2++;
            }
        }
        if (tempOshibki2 <= i / 10) {
            break;
        }
        else {
            tempOshibki2 = 0;
            iter -= 10;
            if (iter < 10) {
                iter = 10;
            }

        }
    }
    if (shouldStop) return false;

    if (tempOshibki2 <= i / 10) {
        plusminus.push_back("+");
    }
    else {
        plusminus.push_back("-");
    }
    numbers.push_back(n);
    oshibki.push_back(tempOshibki);
    errorProbabilities.push_back(eulerTotient(n - 1) / (n - 1)); // Вычисление вероятности ошибки
    return true; // Число успешно добавлено
}

// Функция для ввода одного слова с проверкой
int getSingleWordInput(const std::string& prompt) {
    std::string input;
    double value;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        // Проверка на количество слов
        std::istringstream iss(input);
        std::string word;
        if (!(iss >> word) || (iss >> word)) {
            std::cout << "Неправильное количество. Пожалуйста, введите одно число.\n";
            continue;
        }

        // Проверка на корректность числа
        bool valid = true;
        bool hasDot = false;
        for (size_t i = 0; i < input.size(); ++i) {
            char c = input[i];
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "Некорректное число. Попробуйте снова.\n";
            continue;
        }
        std::istringstream(input) >> value; // Конвертируем слово в число
        if (value <= 3 || value > 20)
        {
            std::cout << "Некорректное число. Попробуйте снова.\n";
            continue;
        }
        return value;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<int> numbers;
    vector<string> plusminus;
    vector<int> oshibki;
    vector<double> errorProbabilities; // Вектор для хранения вероятностей ошибок
    int lenght = 0;
    int input;

    while (true) {
        // Ввод значений с проверкой на количество слов и корректность числа
        input = getSingleWordInput("Введите размерность простого числа \n");
        break; // Если проверки пройдены, выходим из цикла
    }

    lenght = input;

    // Вывод результатов метода Эратосфена
    vector<int> primes = IsPrimeEratosfen(500);
    cout << "Простые числа до 500 (Решето Эратосфена):\n" << endl;
    for (int prime : primes) {
        cout << prime << " ";
    }
    cout << endl << endl;

    int i = 0;
    while (i < 10) {
        atomic<bool> shouldStop(false);
        thread t([&]() {
            createNum(lenght, numbers, plusminus, oshibki, errorProbabilities, primes, shouldStop);
            });

        auto start = chrono::high_resolution_clock::now();
        while (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < 10) {
            this_thread::sleep_for(chrono::milliseconds(1));
        }

        shouldStop = true;
        t.join();

        if (numbers.size() > i) {
            i++;
        }
    }
    cout << "Простые числа, созданные с помощью теста Миллера: \n"  << endl;
    cout << "| Num | " << setw(5) << 1 << " | " << setw(5) << 2 << " | " << setw(5) << 3 << " | " << setw(5) << 4 << " | " << setw(5) << 5 << " | " << setw(5) << 6 << " | " << setw(5) << 7 << " | " << setw(5) << 8 << " | " << setw(5) << 9 << " | " << setw(5) << 10 << " |" << endl;
    cout << " " << endl;
    cout << "|  P  | " << setw(5) << numbers[0] << " | " << setw(5) << numbers[1] << " | " << setw(5) << numbers[2] << " | " << setw(5) << numbers[3] << " | " << setw(5) << numbers[4] << " | " << setw(5) << numbers[5] << " | " << setw(5) << numbers[6] << " | " << setw(5) << numbers[7] << " | " << setw(5) << numbers[8] << " | " << setw(5) << numbers[9] << " |" << endl;
    cout << " " << endl;
    cout << "| Res | " << setw(5) << plusminus[0] << " | " << setw(5) << plusminus[1] << " | " << setw(5) << plusminus[2] << " | " << setw(5) << plusminus[3] << " | " << setw(5) << plusminus[4] << " | " << setw(5) << plusminus[5] << " | " << setw(5) << plusminus[6] << " | " << setw(5) << plusminus[7] << " | " << setw(5) << plusminus[8] << " | " << setw(5) << plusminus[9] << " |" << endl;
    cout << " " << endl;
    cout << "|  K  | " << setw(5) << oshibki[0] << " | " << setw(5) << oshibki[1] << " | " << setw(5) << oshibki[2] << " | " << setw(5) << oshibki[3] << " | " << setw(5) << oshibki[4] << " | " << setw(5) << oshibki[5] << " | " << setw(5) << oshibki[6] << " | " << setw(5) << oshibki[7] << " | " << setw(5) << oshibki[8] << " | " << setw(5) << oshibki[9] << " |" << endl;
    cout << " " << endl;
    cout << "| Err | " << fixed << setprecision(3) << setw(4) << errorProbabilities[0] << " | " << setw(4) << errorProbabilities[1] << " | " << setw(4) << errorProbabilities[2] << " | " << setw(4) << errorProbabilities[3] << " | " << setw(4) << errorProbabilities[4] << " | " << setw(4) << errorProbabilities[5] << " | " << setw(4) << errorProbabilities[6] << " | " << setw(4) << errorProbabilities[7] << " | " << setw(4) << errorProbabilities[8] << " | " << setw(4) << errorProbabilities[9] << " |" << endl;
}
```
### Тест Поклингтона
```C++
#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <chrono>
#include <atomic>

using namespace std;

// Функция для генерации случайного числа в заданном диапазоне
int RandomGenerator(int min, int max) {
    random_device rd;  // источник случайных чисел
    mt19937 gen(rd()); // используем Вихрь Мерсена в качестве генератора
    uniform_int_distribution<int> dis(min, max); // распределение в указанном диапазоне
    return dis(gen);
}

// Простая функция для проверки простоты числа
bool IsPrime(int i, vector<int>& prime) { // передали число по значению и вектор по ссылке
    for (int j = 0; j < prime.size(); j++) { // проверяем на простоту
        if (i % prime[j] == 0) {
            return false;
        }
    }
    return true;
}

vector<int> IsPrimeEratosfen(int p) { // проверка числа на простоту через решето эратосфена
    vector<int> primeNumb;
    primeNumb.push_back(2);
    for (int i = 3; i < p; i += 2) { // перебираем нечетные числа
        if (IsPrime(i, primeNumb) == 1) {
            primeNumb.push_back(i); // складываем в вектор простые числа
        }
    }
    return primeNumb;
}

// Функция для модульного возведения в степень
uint64_t mod(uint64_t number, uint64_t power, uint64_t n) {
    uint64_t ostatok = 1; // Храним в ней модульное возведение в степень.
    while (power > 0) {
        if (power % 2 != 0) { // Если степень нечетная
            ostatok = (ostatok * number) % n;
        }
        power /= 2; // Делим степень на 2
        number = (number * number) % n; // Увеличиваем основание в квадрат и вычисляем по модулю
    }
    return ostatok;
}

// Функция для проверки теоремы Ферма
bool TeoremaFerma(int a, int x, int p) {
    uint64_t result = mod(a, x, p);
    return result == 1;
}

// Тест Поклингтона на простоту
bool poklingtonTest(int64_t n, const vector<int>& factors, int t) {
    vector<int> a;
    for (int i = 0; i < t; i++) {
        int aj = RandomGenerator(2, n - 1);
        if (TeoremaFerma(aj, n - 1, n) != 1) {
            return false; // n - составное число
        }

        bool found = false;
        for (int qj : factors) {
            if (TeoremaFerma(aj, (n - 1) / qj, n) == 1) {
                found = true;
                break;
            }
        }

        if (!found) {
            return true; // n - простое число
        }
    }
    return false; // вероятно, n - составное число
}

// Функция для вычисления количества бит в числе
int countBits(uint64_t number) {
    if (number == 0) return 1; // 0 занимает 1 бит
    return static_cast<int>(log2(number));
}

// Функция для проверки, является ли число меньше или больше заданного количества бит
int isWithinBitRange(uint64_t number, int bitCount, int formin) {
    int numBits = countBits(number);
    if (numBits < bitCount - 1) return 1; // Число меньше заданного количества бит
    if (numBits > bitCount - formin) return 2; // Число больше заданного количества бит
    return 3; // Число равно заданному количеству бит
}

// Функция для создания числа и проведения тестов
bool createNum(int k, vector<int>& numbers, vector<string>& plusminus, vector<int>& oshibki, vector<double>& errorProbabilities, vector<int>& primes, atomic<bool>& shouldStop) {
    vector<int> q; // Вектор для хранения простых множителей
    uint64_t f = 1;
    uint64_t tempf = 1;
    int qi; // Переменная для хранения индекса случайного простого числа
    int ai; // Переменная для хранения случайного целого числа
    int64_t n;
    int targetBitCountF = (k / 2) + 1; // Целевое количество бит для числа F
    int targetBitCountR = targetBitCountF - 1; // Целевое количество бит для числа R
    int tempOshibki = 0;
    int formin = 1;
    uint64_t r;
    if (k < 8) {
        formin = 0;
    }

    while (!shouldStop) { // генерация числа f в заданном диапазоне битов
        qi = primes[RandomGenerator(0, primes.size() / 10)]; // Выбор случайного простого числа из таблицы
        ai = RandomGenerator(1, 12);
        tempf *= pow(qi, ai); // Умножение текущего значения tempf на qi в степени ai
        if (isWithinBitRange(tempf, targetBitCountF, formin) == 2) { // Проверка, не превышает ли tempf максимальное значение для k битов
            f = 1;
            tempf = 1;
            q.clear();
        }
        else if (isWithinBitRange(tempf, targetBitCountF, formin) == 1) { // Если tempf меньше минимального значения для k битов, продолжаем генерировать
            q.push_back(qi); // Добавляем qi в вектор q
        }
        else {
            f = tempf;
            q.push_back(qi);
            do {
                r = RandomGenerator(1, 30) * 2; // Генерация четного числа
            } while (isWithinBitRange(r, targetBitCountR, formin) != 3); // Убеждаемся, что r на 1 бит меньше f
            n = r * f + 1;
            if (poklingtonTest(n, q, 1)) {
                break;
            }
            else {
                tempOshibki++;
                f = 1;
                tempf = 1;
                n = 0;
                q.clear();
            }
        }
    }
    if (shouldStop) return false;

    // Проверка на уникальность числа и повторная генерация, если число уже существует
    while (!numbers.empty() && find(numbers.begin(), numbers.end(), n) != numbers.end()) {
        // Генерация нового числа f и n
        f = 1;
        tempf = 1;
        q.clear();
        while (!shouldStop) { // генерация числа f в заданном диапазоне битов
            qi = primes[RandomGenerator(0, primes.size() / 10)]; // генерация индекса случайных простых чисел
            ai = RandomGenerator(1, 12); // генерация случайных целых чисел
            tempf *= pow(qi, ai);
            if (isWithinBitRange(tempf, targetBitCountF, formin) == 2) {
                f = 1;
                tempf = 1;
                q.clear();
            }
            else if (isWithinBitRange(tempf, targetBitCountF, formin) == 1) {
                q.push_back(qi);
            }
            else {
                f = tempf;
                q.push_back(qi);
                do {
                    r = RandomGenerator(1, 30) * 2; // Генерация четного числа
                } while (isWithinBitRange(r, targetBitCountR, formin) != 3); // Убеждаемся, что r на 1 бит меньше f
                n = r * f + 1;
                if (poklingtonTest(n, q, 1)) {
                    break;
                }
                else {
                    tempOshibki++;
                    f = 1;
                    tempf = 1;
                    n = 0;
                    q.clear();
                }
            }
        }
        if (shouldStop) return false;
    }
    if (shouldStop) return false;

    int tempOshibki2 = 0;
    int i;
    int iter = 1;
    while (!shouldStop) {
        // Проверяем n на простоту с помощью теста Миллера
        for (i = 0; i < 100; i++) {
            if (!poklingtonTest(n, q, iter)) {
                tempOshibki2++;
            }
        }
        if (tempOshibki2 <= i / 10) {
            break;
        }
        else {
            tempOshibki2 = 0;
            iter++;
        }
    }
    if (shouldStop) return false;

    if (tempOshibki2 <= i / 10) {
        plusminus.push_back("+");
    }
    else {
        plusminus.push_back("-");
    }
    numbers.push_back(n);
    oshibki.push_back(tempOshibki);

    double veroatnost = 1.0;
    for (int qj : q) {
        veroatnost = veroatnost * (1.0 / qj);
    }
    errorProbabilities.push_back(static_cast<double>(veroatnost)); // Вычисление вероятности ошибки
    return true; // Число успешно добавлено
}

// Функция для ввода одного слова с проверкой
int getSingleWordInput(const std::string& prompt) {
    std::string input;
    double value;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        // Проверка на количество слов
        std::istringstream iss(input);
        std::string word;
        if (!(iss >> word) || (iss >> word)) {
            std::cout << "Неправильное количество. Пожалуйста, введите одно число.\n";
            continue;
        }

        // Проверка на корректность числа
        bool valid = true;
        bool hasDot = false;
        for (size_t i = 0; i < input.size(); ++i) {
            char c = input[i];
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "Некорректное число. Попробуйте снова.\n";
            continue;
        }
        std::istringstream(input) >> value; // Конвертируем слово в число
        if (value <= 3 || value > 13)
        {
            std::cout << "Некорректное число. Попробуйте снова.\n";
            continue;
        }
        return value;
    }
}


int main() {
    setlocale(LC_ALL, "Russian");
    vector<int> numbers;
    vector<string> plusminus;
    vector<int> oshibki;
    vector<double> errorProbabilities; // Вектор для хранения вероятностей ошибок
    int lenght = 0;
    int input;

    while (true) {
        // Ввод значений с проверкой на количество слов и корректность числа
        input = getSingleWordInput("Введите размерность простого числа \n");
        break; // Если проверки пройдены, выходим из цикла
    }

    lenght = input;

    // Вывод результатов метода Эратосфена
    vector<int> primes = IsPrimeEratosfen(500);
    cout << "Простые числа до 500 (Решето Эратосфена):\n" << endl;
    for (int prime : primes) {
        cout << prime << " ";
    }
    cout << endl << endl;

    int i = 0;
    while (i < 10) {
        atomic<bool> shouldStop(false);
        thread t([&]() {
            createNum(lenght, numbers, plusminus, oshibki, errorProbabilities, primes, shouldStop);
            });

        auto start = chrono::high_resolution_clock::now();
        while (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < 10) {
            this_thread::sleep_for(chrono::milliseconds(1));
        }

        shouldStop = true;
        t.join();

        if (numbers.size() > i) {
            i++;
        }
    }
    cout << "Простые числа, созданные с помощью теста Поклингтона\n" << endl;
    cout << "| Num | " << setw(5) << 1 << " | " << setw(5) << 2 << " | " << setw(5) << 3 << " | " << setw(5) << 4 << " | " << setw(5) << 5 << " | " << setw(5) << 6 << " | " << setw(5) << 7 << " | " << setw(5) << 8 << " | " << setw(5) << 9 << " | " << setw(5) << 10 << " |" << endl;
    cout << " " << endl;
    cout << "|  P  | " << setw(5) << numbers[0] << " | " << setw(5) << numbers[1] << " | " << setw(5) << numbers[2] << " | " << setw(5) << numbers[3] << " | " << setw(5) << numbers[4] << " | " << setw(5) << numbers[5] << " | " << setw(5) << numbers[6] << " | " << setw(5) << numbers[7] << " | " << setw(5) << numbers[8] << " | " << setw(5) << numbers[9] << " |" << endl;
    cout << " " << endl;
    cout << "| Res | " << setw(5) << plusminus[0] << " | " << setw(5) << plusminus[1] << " | " << setw(5) << plusminus[2] << " | " << setw(5) << plusminus[3] << " | " << setw(5) << plusminus[4] << " | " << setw(5) << plusminus[5] << " | " << setw(5) << plusminus[6] << " | " << setw(5) << plusminus[7] << " | " << setw(5) << plusminus[8] << " | " << setw(5) << plusminus[9] << " |" << endl;
    cout << " " << endl;
    cout << "|  K  | " << setw(5) << oshibki[0] << " | " << setw(5) << oshibki[1] << " | " << setw(5) << oshibki[2] << " | " << setw(5) << oshibki[3] << " | " << setw(5) << oshibki[4] << " | " << setw(5) << oshibki[5] << " | " << setw(5) << oshibki[6] << " | " << setw(5) << oshibki[7] << " | " << setw(5) << oshibki[8] << " | " << setw(5) << oshibki[9] << " |" << endl;
    cout << " " << endl;
    cout << "| Err | " << fixed << setprecision(3) << setw(4) << errorProbabilities[0] << " | " << setw(4) << errorProbabilities[1] << " | " << setw(4) << errorProbabilities[2] << " | " << setw(4) << errorProbabilities[3] << " | " << setw(4) << errorProbabilities[4] << " | " << setw(4) << errorProbabilities[5] << " | " << setw(4) << errorProbabilities[6] << " | " << setw(4) << errorProbabilities[7] << " | " << setw(4) << errorProbabilities[8] << " | " << setw(4) << errorProbabilities[9] << " |" << endl;
}
```
### ГОСТ
```C++
#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>

using namespace std;

// Функция для генерации случайного числа в заданном диапазоне
int RandomGenerator(int min, int max) {
    random_device rd;  // источник случайных чисел
    mt19937 gen(rd()); // используем Вихрь Мерсена в качестве генератора
    uniform_int_distribution<int> dis(min, max); //распределение в указанном диапазоне
    return dis(gen);
}

// Функция для модульного возведения в степень
uint64_t mod(uint64_t number, uint64_t power, uint64_t n) {
    uint64_t ostatok = 1; //  Храним в ней модульное возведение в степень.
    while (power > 0) {
        if (power % 2 != 0) { // Если степень нечетная
            ostatok = (ostatok * number) % n;
        }
        power /= 2; // Делим степень на 2
        number = (number * number) % n; // Увеличиваем основание в квадрат и вычисляем по модулю
    }
    return ostatok;
}

// Простая функция для проверки простоты числа
bool IsPrime(int i, vector<int>& prime) { // передали число по значению и вектор по ссылке
    for (int j = 0; j < prime.size(); j++) { // проверяем на простоту
        if (i % prime[j] == 0) {
            return false;
        }
    }
    return true;
}

vector<int> IsPrimeEratosfen(int p) { // проверка числа на простоту через решето эратосфена
    vector<int> primeNumb;
    primeNumb.push_back(2);
    for (int i = 3; i < p; i += 2) { // перебираем нечетные числа
        if (IsPrime(i, primeNumb) == 1) {
            primeNumb.push_back(i); // складываем в вектор простые числа
        }
    }
    return primeNumb;
}

// Функция для проверки теоремы Ферма
bool TeoremaFerma(int a, int x, int p) {
    uint64_t result = mod(a, x, p);
    return result == 1;
}

int GenerateN(int k, int q, int e) {
    // Вычисление начального значения n как суммы двух дробных частей
    // Первая часть: округление вверх частного от деления (2^(t-1)) на q
    // Вторая часть: округление вверх частного от деления (2^(t-1) * 0) на q (это всегда будет 0)
    int N = ceil((double)pow(2, k - 1) / q) + ceil((double)(pow(2, k - 1) * e) / q);
    if (N % 2 != 0) {
        N = N + 1;
    }
    return N;
}

// Тест Диемитко на простоту
bool DiemitkoTest(int64_t n, int t, int N, int u) {
    for (int i = 0; i < t; i++) {
        if (TeoremaFerma(2, n - 1, n) != 1) {
            return false; // n - составное число
        }

        if (TeoremaFerma(2, (N + u), n) == 1) {
            return false; // n - составное число 
        }
    }
    return true; // n - простое число
}

// Функция для вычисления количества бит в числе
int countBits(uint64_t number) {
    if (number == 0) return 1; // 0 занимает 1 бит
    return static_cast<int>(log2(number)) + 1;
}

// Функция для проверки, является ли число меньше или больше заданного количества бит
int isWithinBitRange(int number, int bitCount, int formin) {
    int numBits = countBits(number);
    if (numBits < bitCount - formin) return 1; // Число меньше заданного количества бит
    if (numBits > bitCount + formin) return 2; // Число больше заданного количества бит
    return 3; // Число равно заданному количеству бит
}

// Функция для создания числа и проведения тестов
bool createNum(int k, vector<int>& numbers, vector<string>& plusminus, vector<int>& oshibki, vector<double>& errorProbabilities, atomic<bool>& shouldStop, vector<int>& primes) {
    // Использование глобального вектора простых чисел
    int q;
    int N;
    int u = 0;
    int e = 0;
    int n;
    int tempq = k / 2; // Целевое количество бит для числа q
    int tempOshibki = 0;
    int formin = 0;
    if (k < 10 && k > 5) {
        formin = 1;
    }
    else if (k <= 5) {
        formin = 2;
    }
    // Первый бесконечный цикл для генерации значения n и проверки условия для p
    while (!shouldStop) { // генерация числа m в заданном диапазоне битов
        q = primes[RandomGenerator(0, primes.size() - 1)];
        if (isWithinBitRange(q, tempq, formin) == 3)
        {
            N = GenerateN(k, q, e);
            n = (N + u) * q + 1;
            if (isWithinBitRange(n, k, formin) != 2) {
                if (DiemitkoTest(n, 1, N, u)) {
                    break;
                }
                else {
                    u = u + 2;
                    tempOshibki++;
                }
            }
        }
    }

    if (shouldStop) return false;

    // Проверка на уникальность числа и повторная генерация, если число уже существует
    while (!numbers.empty() && find(numbers.begin(), numbers.end(), n) != numbers.end()) {
        // Генерация нового числа m и n
        while (!shouldStop) { // генерация числа m в заданном диапазоне битов
            q = primes[RandomGenerator(0, primes.size() - 1)];
            if (isWithinBitRange(q, tempq, formin) == 3)
            {
                N = GenerateN(k, q, e);
                n = (N + u) * q + 1;
                if (isWithinBitRange(n, k, formin) != 2) {
                    if (DiemitkoTest(n, 1, N, u)) {
                        break;
                    }
                    else {
                        u = u + 2;
                        tempOshibki++;
                    }
                }
            }
        }
        if (shouldStop) return false;
    }

    if (shouldStop) return false;

    int tempOshibki2 = 0;
    int i;
    int iter = 1;
    while (!shouldStop) {
        // Проверяем n на простоту с помощью теста Диемитко
        for (i = 0; i < 100; i++) {
            if (!DiemitkoTest(n, iter, N, u)) {
                tempOshibki2++;
            }
        }
        if (tempOshibki2 <= i / 10) {
            break;
        }
        else {
            tempOshibki2 = 0;
            iter++;
        }
    }
    if (shouldStop) return false;

    numbers.push_back(n);
    if (tempOshibki2 <= 10) {
        plusminus.push_back("+");
    }
    else {
        plusminus.push_back("-");
    }
    oshibki.push_back(tempOshibki);
    errorProbabilities.push_back(static_cast<double>(1.0 / q)); // Вычисление вероятности ошибки
    return true; // Число успешно добавлено
}

// Функция для ввода одного слова с проверкой
int getSingleWordInput(const std::string& prompt) {
    std::string input;
    double value;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        // Проверка на количество слов
        std::istringstream iss(input);
        std::string word;
        if (!(iss >> word) || (iss >> word)) {
            std::cout << "Неправильное количество. Пожалуйста, введите одно число.\n";
            continue;
        }

        // Проверка на корректность числа
        bool valid = true;
        bool hasDot = false;
        for (size_t i = 0; i < input.size(); ++i) {
            char c = input[i];
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "Некорректное число. Попробуйте снова.\n";
            continue;
        }
        std::istringstream(input) >> value; // Конвертируем слово в число
        if (value < 4 || value > 20)
        {
            std::cout << "Некорректное число. Попробуйте снова.\n";
            continue;
        }
        return value;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<int> numbers;
    vector<string> plusminus;
    vector<int> oshibki;
    vector<double> errorProbabilities; // Вектор для хранения вероятностей ошибок
    int lenght = 0;
    int input;

    while (true) {
        // Ввод значений с проверкой на количество слов и корректность числа
        input = getSingleWordInput("Введите размерность простого числа \n");
        break; // Если проверки пройдены, выходим из цикла
    }

    lenght = input;

    // Вывод результатов метода Эратосфена
    vector<int> primes = IsPrimeEratosfen(500);
    cout << "Простые числа до 500 (Решето Эратосфена):\n" << endl;
    for (int prime : primes) {
        cout << prime << " ";
    }
    cout << endl << endl;

    int i = 0;
    while (i < 10) {
        atomic<bool> shouldStop(false);
        thread t([&]() {
            createNum(lenght, numbers, plusminus, oshibki, errorProbabilities, shouldStop, primes);
            });

        auto start = chrono::high_resolution_clock::now();
        while (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < 10) {
            this_thread::sleep_for(chrono::milliseconds(5));
        }

        shouldStop = true;
        t.join();

        if (numbers.size() > i) {
            i++;
        }
    }
    cout << "Простые числа, созданные с помощью теста ГОСТ:\n" << endl;
    cout << "| Num | " << setw(5) << 1 << " | " << setw(5) << 2 << " | " << setw(5) << 3 << " | " << setw(5) << 4 << " | " << setw(5) << 5 << " | " << setw(5) << 6 << " | " << setw(5) << 7 << " | " << setw(5) << 8 << " | " << setw(5) << 9 << " | " << setw(5) << 10 << " |" << endl;
    cout << " " << endl;
    cout << "|  P  | " << setw(5) << numbers[0] << " | " << setw(5) << numbers[1] << " | " << setw(5) << numbers[2] << " | " << setw(5) << numbers[3] << " | " << setw(5) << numbers[4] << " | " << setw(5) << numbers[5] << " | " << setw(5) << numbers[6] << " | " << setw(5) << numbers[7] << " | " << setw(5) << numbers[8] << " | " << setw(5) << numbers[9] << " |" << endl;
    cout << " " << endl;
    cout << "| Res | " << setw(5) << plusminus[0] << " | " << setw(5) << plusminus[1] << " | " << setw(5) << plusminus[2] << " | " << setw(5) << plusminus[3] << " | " << setw(5) << plusminus[4] << " | " << setw(5) << plusminus[5] << " | " << setw(5) << plusminus[6] << " | " << setw(5) << plusminus[7] << " | " << setw(5) << plusminus[8] << " | " << setw(5) << plusminus[9] << " |" << endl;
    cout << " " << endl;
    cout << "|  K  | " << setw(5) << oshibki[0] << " | " << setw(5) << oshibki[1] << " | " << setw(5) << oshibki[2] << " | " << setw(5) << oshibki[3] << " | " << setw(5) << oshibki[4] << " | " << setw(5) << oshibki[5] << " | " << setw(5) << oshibki[6] << " | " << setw(5) << oshibki[7] << " | " << setw(5) << oshibki[8] << " | " << setw(5) << oshibki[9] << " |" << endl;
    cout << " " << endl;
    cout << "| Err | " << fixed << setprecision(3) << setw(4) << errorProbabilities[0] << " | " << setw(4) << errorProbabilities[1] << " | " << setw(4) << errorProbabilities[2] << " | " << setw(4) << errorProbabilities[3] << " | " << setw(4) << errorProbabilities[4] << " | " << setw(4) << errorProbabilities[5] << " | " << setw(4) << errorProbabilities[6] << " | " << setw(4) << errorProbabilities[7] << " | " << setw(4) << errorProbabilities[8] << " | " << setw(4) << errorProbabilities[9] << " |" << endl;
}
```
### Решение 2 задачи на Java в maint.txt, mains.txt, mainf.txt

### Задание 3*. Решить задачу об остывании чашки кофе из кофейни НГТУ. Написать программу, моделирующую процесс остывания кофе. Программа должна позволять задавать все необходимые параметры. Построить графики или таблицы зависимостей величин. Считаем, что температуру кофе измеряли через некоторые, необязательно равномерные промежутки времени. Написать программу, которая бы по данным измерений строила линейную модель, вычисляя статистические критерии их адекватности.
### Решение задачи на языке С++ в mainz.cpp
```C++
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <stdexcept>
#include <iomanip>

using namespace std;

double Temp;
double EnvT;
double CoolC;
int time_limit;
vector<double> apox_vector;

// Функция которая вычисляет коэффициент корреляции.
double korrel(const vector<double>& temperatures, double mean_y) {
    double sum_xy = 0, sum_x2 = 0, sum_y2 = 0;
    double mean_x = (time_limit - 1) / 2.0;
    for (int i = 0; i < temperatures.size(); i++) {
        sum_xy += (i - mean_x) * (temperatures[i] - mean_y);
        sum_x2 += (i - mean_x) * (i - mean_x);
        sum_y2 += (temperatures[i] - mean_y) * (temperatures[i] - mean_y);
    }
    return sum_xy / sqrt(sum_x2 * sum_y2);
}

// Функция, которая вычисляет коэффициенты линейной аппроксимации для векторов температур и времени.
pair<double, double> aprox(const vector<double>& x_vector, const vector<double>& y_vector) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    int n = x_vector.size();

    for (int i = 0; i < n; i++) {
        sum_x += x_vector[i];
        sum_y += y_vector[i];
        sum_xy += x_vector[i] * y_vector[i];
        sum_x2 += x_vector[i] * x_vector[i];
    }

    double a = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    double b = (sum_y - a * sum_x) / n;

    return make_pair(a, b);
}

// Функция кофе, которая вычисляет температуру кофе в зависимости от времени, используя заданные коэффициенты.
vector<pair<double, double>> coffee(double Temp, double EnvT, double CoolC, int time_limit) {
    vector<double> temperatures;
    vector<double> times;
    vector<pair<double, double>> temp_corr;

    for (int t = 0; t <= time_limit; t++) {
        double temperature = EnvT + (Temp - EnvT) * exp(-CoolC * t);
        temperatures.push_back(temperature);
        times.push_back(t);
    }

    pair<double, double> aprox_coeffs = aprox(times, temperatures);
    double a = aprox_coeffs.first;
    double b = aprox_coeffs.second;

    apox_vector = { a, b };

    double mean_y = accumulate(temperatures.begin(), temperatures.end(), 0.0) / temperatures.size();
    double corr = korrel(temperatures, mean_y);
    for (int t = 0; t <= time_limit; t++) {
        temp_corr.push_back(make_pair(temperatures[t], corr));
    }

    return temp_corr;
}

// Ввод и вывод параметров.
int main() {
    cout << "Enter start coffee temperature(Celsius): ";
    cin >> Temp;
    cout << "Enter temperature of environment(Celsius): ";
    cin >> EnvT;
    cout << "Enter cooling coefficient(dC): ";
    cin >> CoolC;
    cout << "Enter time of full cool(Seconds): ";
    cin >> time_limit;

    vector<pair<double, double>> results = coffee(Temp, EnvT, CoolC, time_limit);

    cout << "Coefficients of the approximating line a - " << apox_vector[0] << ", b - " << apox_vector[1] << endl;
    cout << "|------|" << "------------|" << "---------------|" << endl;
    cout << "| Time |" << " Temperature|" << "CorrelationCoef|" << endl;
    cout << "|------|" << "------------|" << "---------------|" << endl;

    int t = 0;

    for (const auto& result : results) {
        cout << "|" << setw(6) << t << "|" << setw(12) << result.first << "|" << setw(15) << result.second << "|" << endl;
        t++;
        cout << "|------|" << "------------|" << "---------------|" << endl;
    }

    return 0;
}
```
### Решение 3 задачи на Java в mainx.txt
