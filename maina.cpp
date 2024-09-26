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
