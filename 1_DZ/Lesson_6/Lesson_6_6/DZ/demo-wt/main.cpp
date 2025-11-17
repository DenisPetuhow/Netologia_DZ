/* ---------- 0. ЗАГЛУШКА ДЛЯ MSVC C4275 ---------- */
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4275)
# pragma execution_character_set("utf-8")  // UTF-8 для строковых литералов
#endif

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>

#ifdef _MSC_VER
# pragma warning(pop)
#endif

#ifdef _WIN32
#include <Windows.h>  // Для SetConsoleOutputCP
#endif

#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <set>

namespace dbo = Wt::Dbo;

/* ---------- ФУНКЦИЯ НАСТРОЙКИ КОНСОЛИ ---------- */
void setupConsole() {
#ifdef _WIN32
    // SetConsoleOutputCP(CP_UTF8) - устанавливает кодировку вывода консоли в UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8) - устанавливает кодировку ввода консоли в UTF-8
    SetConsoleCP(CP_UTF8);

    std::cout << "? Консоль настроена на UTF-8\n";
#endif
}

/* ---------- 1. КЛАССЫ-СУЩНОСТИ ---------- */
class Publisher;
class Book;
class Shop;
class Stock;
class Sale;

/* ---------- 2. ОПИСАНИЕ КЛАССОВ ---------- */
class Publisher {
public:
    std::string name;

    dbo::collection<dbo::ptr<Book>> books;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, name, "name");
        dbo::hasMany(a, books, dbo::ManyToOne, "id_publisher");
    }
};

class Book {
public:
    std::string title;
    dbo::ptr<Publisher> publisher;

    dbo::collection<dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, title, "title");
        dbo::belongsTo(a, publisher, "id_publisher");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "id_book");
    }
};

class Shop {
public:
    std::string name;

    dbo::collection<dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, name, "name");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "id_shop");
    }
};

class Stock {
public:
    int count{ 0 };
    dbo::ptr<Book> book;
    dbo::ptr<Shop> shop;

    dbo::collection<dbo::ptr<Sale>> sales;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, count, "count");
        dbo::belongsTo(a, book, "id_book");
        dbo::belongsTo(a, shop, "id_shop");
        dbo::hasMany(a, sales, dbo::ManyToOne, "id_stock");
    }
};

class Sale {
public:
    double price{ 0.0 };
    std::chrono::system_clock::time_point dateSale;
    int count{ 0 };
    dbo::ptr<Stock> stock;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, price, "price");
        dbo::field(a, dateSale, "date_sale");
        dbo::field(a, count, "count");
        dbo::belongsTo(a, stock, "id_stock");
    }
};

/* ---------- 3. ФУНКЦИЯ ПРОВЕРКИ СОЕДИНЕНИЯ ---------- */
bool testConnection(dbo::Session& session) {
    std::cout << "Проверка соединения с БД...\n";
    try {
        dbo::Transaction txn(session);
        int result = session.query<int>("SELECT 1").resultValue();

        if (result != 1) {
            std::cerr << "? Неожиданный результат тестового запроса: "
                << result << "\n";
            return false;
        }

        txn.commit();
        std::cout << "? Соединение успешно.\n";
        return true;
    }
    catch (const dbo::Exception& e) {
        std::cerr << "? Ошибка соединения: " << e.what() << "\n";
        return false;
    }
}

/* ---------- 4. DDL-ФУНКЦИИ С ПРОВЕРКАМИ ---------- */
void createTables(dbo::Session& session) {
    std::cout << "Попытка создания таблиц...\n";

    try {
        session.createTables();
        std::cout << "? Таблицы созданы успешно.\n";

        try {
            session.execute(
                "CREATE UNIQUE INDEX IF NOT EXISTS stock_book_shop_unique "
                "ON stock (id_book_id, id_shop_id)"
            );
            std::cout << "? Уникальный индекс создан.\n";
        }
        catch (const dbo::Exception& e) {
            std::cout << "? Индекс уже существует: " << e.what() << "\n";
        }
    }
    catch (const dbo::Exception& e) {
        std::cout << "? Таблицы уже существуют: " << e.what() << "\n";
    }
}

/* ---------- 5. ЗАПОЛНЕНИЕ ТЕСТОВЫМИ ДАННЫМИ С ПРОВЕРКАМИ ---------- */
void fillTestData(dbo::Session& session) {
    std::cout << "Проверка существующих данных...\n";

    try {
        dbo::Transaction txn(session);

        int publisherCount = session.query<int>(
            "SELECT COUNT(*) FROM publisher"
        ).resultValue();

        std::cout << "  Найдено издателей: " << publisherCount << "\n";

        if (publisherCount > 0) {
            std::cout << "? Данные уже существуют, пропускаем заполнение.\n";
            txn.commit();
            return;
        }

        txn.commit();
    }
    catch (const dbo::Exception& e) {
        std::cerr << "? Ошибка при проверке данных: " << e.what() << "\n";
        return;
    }

    std::cout << "Добавление тестовых данных...\n";

    try {
        dbo::Transaction txn(session);

        // ---- СОЗДАНИЕ ИЗДАТЕЛЕЙ ----
        std::cout << "  ? Создание издателей...\n";

        auto p1 = session.add(std::make_unique<Publisher>());
        if (!p1) {
            throw std::runtime_error("Не удалось добавить издателя 'Питер'");
        }
        p1.modify()->name = "Питер";

        auto p2 = session.add(std::make_unique<Publisher>());
        if (!p2) {
            throw std::runtime_error("Не удалось добавить издателя 'BHV'");
        }
        p2.modify()->name = "BHV";

        std::cout << "    ? Издатели добавлены (ID: "
            << p1.id() << ", " << p2.id() << ")\n";

        // ---- СОЗДАНИЕ КНИГ ----
        std::cout << "  ? Создание книг...\n";

        auto b1 = session.add(std::make_unique<Book>());
        if (!b1) {
            throw std::runtime_error("Не удалось добавить книгу 1");
        }
        b1.modify()->title = "C++20. Полное руководство";
        b1.modify()->publisher = p1;

        auto b2 = session.add(std::make_unique<Book>());
        if (!b2) {
            throw std::runtime_error("Не удалось добавить книгу 2");
        }
        b2.modify()->title = "PostgreSQL. Разработка и оптимизация";
        b2.modify()->publisher = p1;

        auto b3 = session.add(std::make_unique<Book>());
        if (!b3) {
            throw std::runtime_error("Не удалось добавить книгу 3");
        }
        b3.modify()->title = "1С для чайников";
        b3.modify()->publisher = p2;

        std::cout << "    ? Книги добавлены (ID: "
            << b1.id() << ", " << b2.id() << ", " << b3.id() << ")\n";

        // ---- СОЗДАНИЕ МАГАЗИНОВ ----
        std::cout << "  ? Создание магазинов...\n";

        auto s1 = session.add(std::make_unique<Shop>());
        if (!s1) {
            throw std::runtime_error("Не удалось добавить магазин 1");
        }
        s1.modify()->name = "Book24";

        auto s2 = session.add(std::make_unique<Shop>());
        if (!s2) {
            throw std::runtime_error("Не удалось добавить магазин 2");
        }
        s2.modify()->name = "Лабиринт";

        auto s3 = session.add(std::make_unique<Shop>());
        if (!s3) {
            throw std::runtime_error("Не удалось добавить магазин 3");
        }
        s3.modify()->name = "Читай-город";

        std::cout << "    ? Магазины добавлены (ID: "
            << s1.id() << ", " << s2.id() << ", " << s3.id() << ")\n";

        // ---- СОЗДАНИЕ ОСТАТКОВ ----
        std::cout << "  ? Создание остатков...\n";

        auto st1 = session.add(std::make_unique<Stock>());
        if (!st1) {
            throw std::runtime_error("Не удалось добавить остаток 1");
        }
        st1.modify()->book = b1;
        st1.modify()->shop = s1;
        st1.modify()->count = 10;

        auto st2 = session.add(std::make_unique<Stock>());
        if (!st2) {
            throw std::runtime_error("Не удалось добавить остаток 2");
        }
        st2.modify()->book = b2;
        st2.modify()->shop = s1;
        st2.modify()->count = 5;

        auto st3 = session.add(std::make_unique<Stock>());
        if (!st3) {
            throw std::runtime_error("Не удалось добавить остаток 3");
        }
        st3.modify()->book = b1;
        st3.modify()->shop = s2;
        st3.modify()->count = 7;

        auto st4 = session.add(std::make_unique<Stock>());
        if (!st4) {
            throw std::runtime_error("Не удалось добавить остаток 4");
        }
        st4.modify()->book = b3;
        st4.modify()->shop = s3;
        st4.modify()->count = 20;

        std::cout << "    ? Остатки добавлены (4 записи)\n";

        // ---- СОЗДАНИЕ ПРОДАЖ ----
        std::cout << "  ? Создание продаж...\n";

        auto sale1 = session.add(std::make_unique<Sale>());
        if (!sale1) {
            throw std::runtime_error("Не удалось добавить продажу 1");
        }
        sale1.modify()->stock = st1;
        sale1.modify()->count = 2;
        sale1.modify()->price = 1200.00;
        sale1.modify()->dateSale = std::chrono::system_clock::now();

        std::cout << "    ? Продажи добавлены (1 запись)\n";

        txn.commit();

        std::cout << "? Все тестовые данные успешно добавлены.\n";
    }
    catch (const dbo::Exception& e) {
        std::cerr << "? Ошибка Wt::Dbo при добавлении данных: "
            << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "? Ошибка при добавлении данных: "
            << e.what() << "\n";
    }
}

/* ---------- 6. ФУНКЦИЯ ВЫВОДА СТАТИСТИКИ ---------- */
void printDatabaseStats(dbo::Session& session) {
    std::cout << "\n--- СТАТИСТИКА БД ---\n";

    try {
        dbo::Transaction txn(session);

        int pubCount = session.query<int>(
            "SELECT COUNT(*) FROM publisher"
        ).resultValue();

        int bookCount = session.query<int>(
            "SELECT COUNT(*) FROM book"
        ).resultValue();

        int shopCount = session.query<int>(
            "SELECT COUNT(*) FROM shop"
        ).resultValue();

        int stockCount = session.query<int>(
            "SELECT COUNT(*) FROM stock"
        ).resultValue();

        int saleCount = session.query<int>(
            "SELECT COUNT(*) FROM sale"
        ).resultValue();

        std::cout << "  Издателей: " << pubCount << "\n";
        std::cout << "  Книг: " << bookCount << "\n";
        std::cout << "  Магазинов: " << shopCount << "\n";
        std::cout << "  Остатков: " << stockCount << "\n";
        std::cout << "  Продаж: " << saleCount << "\n";

        txn.commit();
    }
    catch (const dbo::Exception& e) {
        std::cerr << "? Ошибка получения статистики: " << e.what() << "\n";
    }

    std::cout << "---------------------\n\n";
}

/* ---------- 7. ЗАПРОС: МАГАЗИНЫ ПО ИЗДАТЕЛЮ ---------- */
std::vector<std::string> findShopsByPublisher(
    dbo::Session& session,
    const std::string& publisherName)
{
    std::vector<std::string> result;
    if (publisherName.empty()) {
        std::cerr << "? Предупреждение: пустое имя издателя\n";
        return result;
    }

    try {
        dbo::Transaction txn(session);

        std::cout << "  ? Поиск издателя \"" << publisherName << "\"...\n";

        dbo::ptr<Publisher> publisher = session.find<Publisher>()
            .where("name = ?")
            .bind(publisherName);

        if (!publisher) {
            std::cout << "  ? Издатель \"" << publisherName
                << "\" не найден в БД.\n";
            txn.commit();
            return result;
        }

        std::cout << "  ? Издатель найден (ID: " << publisher.id() << ")\n";

        std::cout << "  ? Книги издателя:\n";
        int bookCount = 0;
        for (const dbo::ptr<Book>& book : publisher->books) {
            std::cout << "    • " << book->title << " (ID: "
                << book.id() << ")\n";
            bookCount++;
        }

        if (bookCount == 0) {
            std::cout << "  ? У издателя нет книг в БД.\n";
            txn.commit();
            return result;
        }

        std::cout << "  ? Выполнение запроса магазинов...\n";

        dbo::collection<std::string> shops = session.query<std::string>(
            "SELECT DISTINCT sh.name "
            "FROM shop sh "
            "JOIN stock st ON st.id_shop_id = sh.id "
            "JOIN book b ON b.id = st.id_book_id "
            "JOIN publisher p ON p.id = b.id_publisher_id "
            "WHERE p.name = ?"
        ).bind(publisherName);

        int shopCount = shops.size();
        std::cout << "  ? Найдено магазинов: " << shopCount << "\n";

        for (const auto& shopName : shops) {
            result.push_back(shopName);
        }

        txn.commit();
    }
    catch (const dbo::Exception& e) {
        std::cerr << "? Ошибка Wt::Dbo при выполнении запроса: "
            << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "? Ошибка при выполнении запроса: "
            << e.what() << "\n";
    }

    return result;
}

/* ---------- 8. MAIN С ПРАВИЛЬНЫМ ПОРЯДКОМ И КОДИРОВКОЙ ---------- */
int main() {
    // ====================================================================
    // ЭТАП 0: НАСТРОЙКА КОНСОЛИ (ПЕРВЫМ ДЕЛОМ!)
    // ====================================================================
    setupConsole();

    std::cout << "=== ЗАПУСК ПРОГРАММЫ ===\n\n";

    try {
        // ====================================================================
        // ЭТАП 1: СОЗДАНИЕ ПОДКЛЮЧЕНИЯ С UTF-8
        // ====================================================================
        std::cout << "1. Создание подключения к PostgreSQL...\n";

        // ВАЖНО: добавляем client_encoding=UTF8 для корректной работы с кириллицей
        std::string connectionString =
            "host=localhost "
            "port=5432 "
            "dbname=DZ_6_6 "
            "user=postgres "
            "password=12345678 "
            "client_encoding=UTF8";  // <-- КЛЮЧЕВОЙ ПАРАМЕТР!

        std::unique_ptr<dbo::backend::Postgres> postgres;

        try {
            postgres = std::make_unique<dbo::backend::Postgres>(connectionString);

            if (!postgres) {
                std::cerr << "? КРИТИЧЕСКАЯ ОШИБКА: "
                    << "Не удалось создать объект подключения!\n";
                return 1;
            }

            std::cout << "? Объект подключения создан.\n";
        }
        catch (const dbo::Exception& e) {
            std::cerr << "? ОШИБКА при создании подключения: "
                << e.what() << "\n";
            std::cerr << "\nВозможные причины:\n";
            std::cerr << "  • PostgreSQL не запущен\n";
            std::cerr << "  • База данных 'DZ_6_6' не существует\n";
            std::cerr << "  • Неверный пользователь или пароль\n";
            std::cerr << "  • Неверный хост или порт\n";
            return 1;
        }

        // ====================================================================
        // ЭТАП 2: СОЗДАНИЕ СЕССИИ
        // ====================================================================
        std::cout << "\n2. Создание сессии...\n";

        dbo::Session session;

        try {
            session.setConnection(std::move(postgres));
            std::cout << "? Сессия создана и подключена.\n";
        }
        catch (const dbo::Exception& e) {
            std::cerr << "? ОШИБКА при создании сессии: "
                << e.what() << "\n";
            return 1;
        }

        // ====================================================================
        // ЭТАП 3: РЕГИСТРАЦИЯ КЛАССОВ (ДО ЛЮБЫХ ТРАНЗАКЦИЙ!)
        // ====================================================================
        std::cout << "\n3. Регистрация классов в ORM...\n";

        try {
            session.mapClass<Publisher>("publisher");
            session.mapClass<Book>("book");
            session.mapClass<Shop>("shop");
            session.mapClass<Stock>("stock");
            session.mapClass<Sale>("sale");

            std::cout << "? Все классы зарегистрированы (5 шт.).\n";
        }
        catch (const dbo::Exception& e) {
            std::cerr << "? ОШИБКА при регистрации классов: "
                << e.what() << "\n";
            return 1;
        }

        // ====================================================================
        // ЭТАП 4: ПРОВЕРКА СОЕДИНЕНИЯ (ПОСЛЕ mapClass!)
        // ====================================================================
        std::cout << "\n4. Проверка соединения...\n";

        if (!testConnection(session)) {
            std::cerr << "? КРИТИЧЕСКАЯ ОШИБКА: "
                << "Соединение не работает!\n";
            return 1;
        }

        // ====================================================================
        // ЭТАП 5: СОЗДАНИЕ СТРУКТУРЫ БД
        // ====================================================================
        std::cout << "\n5. Создание структуры БД...\n";
        createTables(session);

        // ====================================================================
        // ЭТАП 6: ЗАПОЛНЕНИЕ ДАННЫМИ
        // ====================================================================
        std::cout << "\n6. Заполнение данными...\n";
        fillTestData(session);

        // ====================================================================
        // ЭТАП 7: ВЫВОД СТАТИСТИКИ
        // ====================================================================
        printDatabaseStats(session);

        // ====================================================================
        // ЭТАП 8: ИНТЕРАКТИВНЫЙ ЗАПРОС
        // ====================================================================
        std::cout << "=== ПРОГРАММА ГОТОВА К РАБОТЕ ===\n\n";

        std::cout << "Доступные издатели: Питер, BHV\n";
        std::cout << "Введите имя издателя: ";

        std::string pubName;
        std::getline(std::cin, pubName);

        if (pubName.empty()) {
            std::cerr << "? ОШИБКА: Имя издателя не может быть пустым!\n";
            return 1;
        }

        std::cout << "\n7. Выполнение запроса...\n";

        auto shops = findShopsByPublisher(session, pubName);

        // ====================================================================
        // ЭТАП 9: ВЫВОД РЕЗУЛЬТАТОВ
        // ====================================================================
        std::cout << "\n=== РЕЗУЛЬТАТ ===\n";

        if (shops.empty()) {
            std::cout << "? Издатель \"" << pubName
                << "\" не найден или его книги нигде не продаются.\n";
        }
        else {
            std::cout << "? Книги издателя \"" << pubName
                << "\" продаются в магазинах:\n\n";

            for (size_t i = 0; i < shops.size(); ++i) {
                std::cout << "  " << (i + 1) << ". " << shops[i] << "\n";
            }

            std::cout << "\nВсего магазинов: " << shops.size() << "\n";
        }
    }
    catch (const dbo::Exception& ex) {
        std::cerr << "\n? КРИТИЧЕСКАЯ ОШИБКА Wt::Dbo: "
            << ex.what() << std::endl;
        std::cerr << "\nВозможные причины:\n";
        std::cerr << "  • PostgreSQL не запущен\n";
        std::cerr << "  • База данных 'DZ_6_6' не существует\n";
        std::cerr << "  • Неверный пользователь или пароль\n";
        std::cerr << "  • Неверный хост или порт\n";
        std::cerr << "  • Нарушение целостности данных\n";
        return 1;
    }
    catch (const std::exception& ex) {
        std::cerr << "\n? КРИТИЧЕСКАЯ ОШИБКА: "
            << ex.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "\n? НЕИЗВЕСТНАЯ КРИТИЧЕСКАЯ ОШИБКА!\n";
        return 1;
    }

    std::cout << "\n=== ПРОГРАММА ЗАВЕРШЕНА УСПЕШНО ===\n";
    return 0;
}