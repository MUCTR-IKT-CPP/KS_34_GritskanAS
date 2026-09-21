#include <iostream>
#include <clocale> 

/*
 * Проверка простоты числа
 *
 * @param n проверяемое целое число.
 * @return возвращает истину, если число простое, иначе ложь.
 */
bool isPrime(int n) {
    constexpr int MIN_PRIME{ 2 };

    if (n < MIN_PRIME) {
        return false;
    }
    for (int i{ 2 }; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    std::setlocale(LC_ALL, "");

    int n{ 0 };
    std::cout << "Ввод: N = ";
    std::cin >> n;

    constexpr int MIN_VALID_N{ 2 };
    if (n < MIN_VALID_N) {
        std::cout << "Вывод: \n";
        std::cout << "Количество: 0\n";
        return 0;
    }

    int array_size{ n - 1 };


    int* p_numbers{ new int[array_size] };

    for (int i{ 0 }; i < array_size; ++i) {
        p_numbers[i] = i + 2;
    }

    int prime_count{ 0 };

    std::cout << "Вывод: ";


    for (int i{ 0 }; i < array_size; ++i) {
        if (isPrime(p_numbers[i])) {
            std::cout << p_numbers[i] << " ";
            ++prime_count;
        }
    }
    std::cout << '\n';

    std::cout << "Количество: " << prime_count << '\n';

    delete[] p_numbers;

    return 0;
}
