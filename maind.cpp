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
