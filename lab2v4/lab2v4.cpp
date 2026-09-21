#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>

const int MIN_VALUE = 0;
const int MAX_VALUE = 255;

enum class Operation {
    invert = 1,
    binarize = 2,
    brightest_row = 3,
    void_pointer = 4
};

/**
 * Заполняет квадратную матрицу случайными числами от 0 до 255.
 *
 * @param p_matrix указатель на первый элемент матрицы.
 * @param size размер стороны матрицы.
 */
void fillRandom(int* p_matrix, int size) {
    const int ELEMENT_COUNT = size * size;

    for (int i = 0; i < ELEMENT_COUNT; i++) {
        p_matrix[i] = MIN_VALUE + std::rand() % (MAX_VALUE - MIN_VALUE + 1);
    }
}

/**
 * Выводит квадратную матsрицу в консоль.
 *
 * @param p_matrix указатель на первый элемент матрицы.
 * @param size размер стороны матрицы.
 */
void printMatrix(const int* p_matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            const int INDEX = row * size + column;
            std::cout << std::setw(4) << p_matrix[INDEX];
        }
        std::cout << '\n';
    }
}

/**
 * Заменяет каждый элемент x на 255 - x.
 *
 * @param p_matrix указатель на первый элемент матрицы.
 * @param size размер стороны матрицы.
 */
void invertMatrix(int* p_matrix, int size) {
    const int ELEMENT_COUNT = size * size;

    for (int i = 0; i < ELEMENT_COUNT; i++) {
        p_matrix[i] = MAX_VALUE - p_matrix[i];
    }
}

/**
 * Преобразует матрицу в бинарную по заданному порогу.
 * Значение меньше порога становится 0, иначе 1.
 *
 * @param p_matrix указатель на первый элемент матрицы.
 * @param size размер стороны матрицы.
 * @param threshold порог от 0 до 255.
 */
void binarizeMatrix(int* p_matrix, int size, int threshold) {
    const int ELEMENT_COUNT = size * size;

    for (int i = 0; i < ELEMENT_COUNT; i++) {
        if (p_matrix[i] >= threshold) {
            p_matrix[i] = 1;
        } else {
            p_matrix[i] = 0;
        }
    }
}

/**
 * Вычисляет сумму элементов указанной строки.
 *
 * @param p_matrix указатель на первый элемент матрицы.
 * @param size размер стороны матрицы.
 * @param row индекс строки.
 * @return сумма элементов строки.
 */
int calculateRowSum(const int* p_matrix, int size, int row) {
    int sum = 0;

    for (int column = 0; column < size; column++) {
        const int INDEX = row * size + column;
        sum += p_matrix[INDEX];
    }

    return sum;
}

/**
 * Находит строку с максимальной суммой элементов.
 *
 * @param p_matrix указатель на первый элемент матрицы.
 * @param size размер стороны матрицы.
 * @return индекс строки с максимальной суммой.
 */
int findBrightestRow(const int* p_matrix, int size) {
    int brightest_row = 0;
    int max_sum = calculateRowSum(p_matrix, size, 0);

    for (int row = 1; row < size; row++) {
        const int CURRENT_SUM = calculateRowSum(p_matrix, size, row);

        if (CURRENT_SUM > max_sum) {
            max_sum = CURRENT_SUM;
            brightest_row = row;
        }
    }

    return brightest_row;
}

/**
 * Принимает массив через void*, приводит его к int* и вычисляет
 * сумму всех элементов матрицы.
 *
 * @param p_data нетипизированный указатель на массив int.
 * @param size размер стороны матрицы.
 * @return сумма всех элементов матрицы.
 */
int calculateSumViaVoid(void* p_data, int size) {
    int* p_matrix = static_cast<int*>(p_data);
    const int ELEMENT_COUNT = size * size;
    int sum = 0;

    for (int i = 0; i < ELEMENT_COUNT; i++) {
        sum += p_matrix[i];
    }

    return sum;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int n = 0;
    std::cout << "Введите N: ";
    std::cin >> n;

    if (n <= 0) {
        std::cout << "Ошибка: N должно быть положительным числом.\n";
        return 1;
    }

    int* p_matrix = new int[n * n]{};
    fillRandom(p_matrix, n);

    std::cout << "\nИсходная матрица:\n";
    printMatrix(p_matrix, n);

    std::cout << "\nВыберите действие:\n"
              << "1 - заменить каждый элемент x на 255 - x\n"
              << "2 - преобразовать матрицу в 0 и 1 по порогу\n"
              << "3 - найти строку с максимальной суммой\n"
              << "4 - передать массив через void* и найти сумму всех элементов\n"
              << "Ваш выбор: ";

    int operation_number = 0;
    std::cin >> operation_number;

    const Operation OPERATION = static_cast<Operation>(operation_number);

    switch (OPERATION) {
        case Operation::invert:
            invertMatrix(p_matrix, n);
            std::cout << "\nРезультат преобразования 255 - x:\n";
            printMatrix(p_matrix, n);
            break;

        case Operation::binarize: {
            int threshold = 0;
            std::cout << "Введите порог от " << MIN_VALUE << " до " << MAX_VALUE << ": ";
            std::cin >> threshold;

            if (threshold < MIN_VALUE || threshold > MAX_VALUE) {
                std::cout << "Ошибка: порог должен быть от 0 до 255.\n";
                delete[] p_matrix;
                p_matrix = nullptr;
                return 1;
            }

            binarizeMatrix(p_matrix, n, threshold);
            std::cout << "\nБинарная матрица:\n";
            printMatrix(p_matrix, n);
            break;
        }

        case Operation::brightest_row: {
            const int BRIGHTEST_ROW = findBrightestRow(p_matrix, n);
            const int ROW_SUM = calculateRowSum(p_matrix, n, BRIGHTEST_ROW);

            std::cout << "\nСтрока с максимальной суммой: " << BRIGHTEST_ROW + 1
                      << "\nСумма элементов: " << ROW_SUM << '\n';
            break;
        }

        case Operation::void_pointer: {
            const int TOTAL_SUM = calculateSumViaVoid(static_cast<void*>(p_matrix), n);
            std::cout << "\nСумма всех элементов, рассчитанная через void*: "
                      << TOTAL_SUM << '\n';
            break;
        }

        default:
            std::cout << "Ошибка: неизвестная операция.\n";
            delete[] p_matrix;
            p_matrix = nullptr;
            return 1;
    }

    delete[] p_matrix;
    p_matrix = nullptr;

    return 0;
}
