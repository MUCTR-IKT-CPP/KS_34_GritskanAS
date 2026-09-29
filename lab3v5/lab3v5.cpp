#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

struct Book{
    std::string isbn = "";
    std::string title = "";
    std::string author = "";
    int year = 0;
    bool is_available = false;
    int pages = 0;
};

/**
 * Читает целое число в заданном диапазоне, повторяя ввод при ошибке.
 * @param prompt приглашение к вводу.
 * @param min_value минимальное допустимое значение.
 * @param max_value максимальное допустимое значение.
 * @param value ссылка для записи результата.
 * @return true при успешном вводе, false при завершении потока.
 */
bool readInteger(const std::string& prompt, int min_value, int max_value, int& value){
    std::string line = "";
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, line)) {
            return false;
        }
        std::istringstream input{line};
        int number = 0;
        char extra = '\0';
        if ((input >> number) && !(input >> extra) &&
            number >= min_value && number <= max_value) {
            value = number;
            return true;
        }
        std::cout << "Введите целое число от " << min_value << " до " << max_value << ".\n";
    }
}

/**
 * Читает непустую строку и удаляет пробелы по краям.
 * @param prompt приглашение к вводу.
 * @param value ссылка для записи строки.
 * @return true при успешном вводе, false при завершении потока.
 */
bool readText(const std::string& prompt, std::string& value){
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, value)) {
            return false;
        }
        const std::size_t FIRST = value.find_first_not_of(" \t\r");
        if (FIRST != std::string::npos) {
            const std::size_t LAST = value.find_last_not_of(" \t\r");
            value = value.substr(FIRST, LAST - FIRST + 1);
            return true;
        }
        std::cout << "Строка не должна быть пустой.\n";
    }
}

/**
 * Создаёт учебный ISBN формата XXX-X-XXX-XXXXX-X с контрольной цифрой.
 * @param number уникальное число от 0 до 99999999 в пределах каталога.
 * @return строка ISBN-13 (не означает регистрацию реального издания).
 */
std::string makeIsbn(int number){
    std::ostringstream digits{};
    digits << "9785" << std::setfill('0') << std::setw(8) << number;
    const std::string CODE = digits.str();
    int sum = 0;
    for (std::size_t i = 0; i < CODE.size(); i++) {
        const int WEIGHT = (i % 2 == 0) ? 1 : 3;
        sum += (CODE[i] - '0') * WEIGHT;
    }
    const int CHECK_DIGIT = (10 - sum % 10) % 10;
    return CODE.substr(0, 3) + "-" + CODE.substr(3, 1) + "-" + CODE.substr(4, 3) +
           "-" + CODE.substr(7, 5) + "-" + std::to_string(CHECK_DIGIT);
}

/**
 * Заполняет вектор случайными изданиями классических произведений.
 * Годы изданий: 1950–2026; объём: 80–1200 страниц; ISBN уникальны.
 * @param books вектор заранее заданного размера (не более 10000).
 */
void fillRandom(std::vector<Book>& books){
    const std::vector<std::string> TITLES = {
        "Война и мир", "Анна Каренина", "Евгений Онегин", "Капитанская дочка",
        "Мёртвые души", "Ревизор", "Преступление и наказание", "Идиот",
        "Вишнёвый сад", "Собачье сердце"
    };
    const std::vector<std::string> AUTHORS = {
        "Толстой Лев", "Толстой Лев", "Пушкин Александр", "Пушкин Александр",
        "Гоголь Николай", "Гоголь Николай", "Достоевский Фёдор", "Достоевский Фёдор",
        "Чехов Антон", "Булгаков Михаил"
    };
    const int MIN_YEAR = 1950;
    const int MAX_YEAR = 2026;
    const int MIN_PAGES = 80;
    const int MAX_PAGES = 1200;
    const int ISBN_COUNT = 100000000;
    std::random_device seed{};
    std::mt19937 generator{seed()};
    std::uniform_int_distribution<int> title_distribution{0, static_cast<int>(TITLES.size()) - 1};
    std::uniform_int_distribution<int> year_distribution{MIN_YEAR, MAX_YEAR};
    std::uniform_int_distribution<int> page_distribution{MIN_PAGES, MAX_PAGES};
    std::uniform_int_distribution<int> isbn_distribution{0, ISBN_COUNT - 1};
    std::bernoulli_distribution availability_distribution{0.5};
    const int ISBN_START = isbn_distribution(generator);

    for (std::size_t i = 0; i < books.size(); i++) {
        const std::size_t INDEX = static_cast<std::size_t>(title_distribution(generator));
        books[i] = {
            makeIsbn((ISBN_START + static_cast<int>(i)) % ISBN_COUNT),
            TITLES[INDEX], AUTHORS[INDEX], year_distribution(generator),
            availability_distribution(generator), page_distribution(generator)
        };
    }
}

/**
 * Выводит все поля каждой книги или сообщение о пустом результате.
 * @param books каталог или результат поиска/фильтрации.
 */
void printBooks(const std::vector<Book>& books){
    if (books.empty()) {
        std::cout << "Книг нет.\n";
        return;
    }
    for (const Book& BOOK : books) {
        std::cout << "\nISBN: " << BOOK.isbn
                  << "\nНазвание: " << BOOK.title
                  << "\nАвтор: " << BOOK.author
                  << "\nГод издания: " << BOOK.year
                  << "\nСтраниц: " << BOOK.pages
                  << "\nСтатус: " << (BOOK.is_available ? "доступна" : "выдана") << '\n';
    }
}

/**
 * Ищет подстроку в названии или имени автора с учётом регистра.
 * @param books исходный каталог.
 * @param query искомая подстрока.
 * @return новый вектор всех подходящих книг.
 */
std::vector<Book> searchBooks(const std::vector<Book>& books, const std::string& query){
    std::vector<Book> result{};
    for (const Book& BOOK : books) {
        if (BOOK.title.find(query) != std::string::npos ||
            BOOK.author.find(query) != std::string::npos) {
            result.push_back(BOOK);
        }
    }
    return result;
}

/**
 * Отбирает книги за период, включая оба граничных года.
 * @param books исходный каталог.
 * @param first_year начало периода.
 * @param last_year конец периода.
 * @return новый вектор подходящих книг; пустой при обратном диапазоне.
 */
std::vector<Book> filterByYear(const std::vector<Book>& books, int first_year, int last_year){
    std::vector<Book> result{};
    for (const Book& BOOK : books) {
        if (BOOK.year >= first_year && BOOK.year <= last_year) {
            result.push_back(BOOK);
        }
    }
    return result;
}

/**
 * Выводит число книг, средний объём и количество книг каждого статуса.
 * @param books каталог; для пустого каталога среднее не вычисляется.
 */
void printStatistics(const std::vector<Book>& books){
    long long total_pages = 0;
    std::size_t available_count = 0;
    for (const Book& BOOK : books) {
        total_pages += BOOK.pages;
        if (BOOK.is_available) {
            available_count++;
        }
    }
    std::cout << "Всего книг: " << books.size()
              << "\nДоступных: " << available_count
              << "\nНедоступных: " << books.size() - available_count << '\n';
    if (books.empty()) {
        std::cout << "Среднее количество страниц: нет данных.\n";
        return;
    }
    const double AVERAGE = static_cast<double>(total_pages) / static_cast<double>(books.size());
    std::cout << "Среднее количество страниц: " << std::fixed << std::setprecision(2)
              << AVERAGE << '\n';
}

/**
 * Сравнивает книги по убыванию года, затем по фамилии автора по алфавиту.
 * @param first первая книга; автор записан как «Фамилия Имя».
 * @param second вторая книга с тем же форматом имени автора.
 * @return true, если первая книга должна предшествовать второй.
 */
bool compareBooks(const Book& first, const Book& second){
    if (first.year != second.year) {
        return first.year > second.year;
    }
    return first.author < second.author;
}

/**
 * Сортирует исходный каталог от новых изданий к старым, затем по фамилии.
 * @param books изменяемый каталог.
 */
void sortBooks(std::vector<Book>& books){
    std::sort(books.begin(), books.end(), compareBooks);
}

/**
 * Выводит список доступных команд.
 */
void printMenu(){
    std::cout << "\n1 - показать каталог\n"
              << "2 - поиск по автору или названию\n"
              << "3 - фильтр по году\n"
              << "4 - статистика\n"
              << "5 - сортировка\n"
              << "0 - выход\n";
}

/**
 * Создаёт каталог из N книг и запускает интерактивное меню.
 * @return код успешного завершения программы.
 */
int main(){
    const int MAX_BOOKS = 10000;
    const int MAX_YEAR = 2026;
    int n = 0;
    if (!readInteger("Введите N (0–10000): ", 0, MAX_BOOKS, n)) {
        return 0;
    }
    std::vector<Book> books(static_cast<std::size_t>(n));
    fillRandom(books);
    printBooks(books);

    while (true) {
        printMenu();
        int operation = 0;
        if (!readInteger("Ваш выбор: ", 0, 5, operation) || operation == 0) {
            break;
        }
        switch (operation) {
            case 1:
                printBooks(books);
                break;
            case 2: {
                std::string query = "";
                if (!readText("Подстрока (с учётом регистра): ", query)) {
                    return 0;
                }
                printBooks(searchBooks(books, query));
                break;
            }
            case 3: {
                int first_year = 0;
                int last_year = 0;
                if (!readInteger("Начальный год: ", 1, MAX_YEAR, first_year) ||
                    !readInteger("Конечный год: ", 1, MAX_YEAR, last_year)) {
                    return 0;
                }
                if (first_year > last_year) {
                    std::cout << "Ошибка: начальный год больше конечного.\n";
                    break;
                }
                printBooks(filterByYear(books, first_year, last_year));
                break;
            }
            case 4:
                printStatistics(books);
                break;
            case 5:
                sortBooks(books);
                std::cout << "Каталог отсортирован по убыванию года и по фамилии автора.\n";
                printBooks(books);
                break;

        }
    }
    std::cout << "Работа завершена.\n";
    return 0;
}
