/* ---------- 0. ЗАГЛУШКА ДЛЯ MSVC C4275 ---------- */
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable:4275)
#endif

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>

#ifdef _MSC_VER
#  pragma warning(pop)
#endif

#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Windows.h"

namespace dbo = Wt::Dbo;

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

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, name, "name");
    }
};

class Book {
public:
    std::string title;
    dbo::ptr<Publisher> publisher;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, title, "title");
        dbo::belongsTo(a, publisher, "id_publisher");
    }
};

class Shop {
public:
    std::string name;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, name, "name");
    }
};

class Stock {
public:
    int count{ 0 };
    dbo::ptr<Book> book;
    dbo::ptr<Shop> shop;

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, count, "count");
        dbo::belongsTo(a, book, "id_book");
        dbo::belongsTo(a, shop, "id_shop");
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

/* ---------- 3. DDL-ФУНКЦИИ ---------- */

void createTables(dbo::Session& session) {
    std::cout << "Попытка создания таблиц...\n";
    try {
        dbo::Transaction txn(session);
        session.createTables();
        txn.commit();
        std::cout << "✓ Таблицы созданы успешно.\n";

        try {
            dbo::Transaction txn2(session);
            session.execute("CREATE UNIQUE INDEX IF NOT EXISTS stock_book_shop_unique "
                "ON stock (id_book, id_shop)");
            txn2.commit();
            std::cout << "✓ Индекс создан.\n";
        }
        catch (...) {
            std::cout << "⚠ Индекс уже существует.\n";
        }
    }
    catch (const std::exception& e) {
        std::cout << "⚠ Ошибка создания таблиц (возможно уже существуют): " << e.what() << "\n";
    }
}

/* ---------- 4. ЗАПОЛНЕНИЕ ТЕСТОВЫМИ ДАННЫМИ ---------- */

void fillTestData(dbo::Session& session) {
    std::cout << "Проверка существующих данных...\n";

    try {
        dbo::Transaction txn(session);

        int count = session.query<int>("SELECT COUNT(*) FROM publisher").resultValue();

        if (count > 0) {
            std::cout << "✓ Данные уже существуют (" << count << " издателей), пропускаем заполнение.\n";
            txn.commit();
            return;
        }
        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "⚠ Ошибка при проверке данных: " << e.what() << "\n";
    }

    std::cout << "Добавление тестовых данных...\n";
    try {
        dbo::Transaction txn(session);

        auto p1 = session.add(std::make_unique<Publisher>());
        p1.modify()->name = "Piter";
        auto p2 = session.add(std::make_unique<Publisher>());
        p2.modify()->name = "BHV";
        std::cout << "  ✓ Издатели добавлены\n";

        auto b1 = session.add(std::make_unique<Book>());
        b1.modify()->title = "C++20. Полное руководство";
        b1.modify()->publisher = p1;

        auto b2 = session.add(std::make_unique<Book>());
        b2.modify()->title = "PostgreSQL. Разработка и оптимизация";
        b2.modify()->publisher = p1;

        auto b3 = session.add(std::make_unique<Book>());
        b3.modify()->title = "1С для чайников";
        b3.modify()->publisher = p2;
        std::cout << "  ✓ Книги добавлены\n";

        auto s1 = session.add(std::make_unique<Shop>());
        s1.modify()->name = "Book24";
        auto s2 = session.add(std::make_unique<Shop>());
        s2.modify()->name = "Лабиринт";
        auto s3 = session.add(std::make_unique<Shop>());
        s3.modify()->name = "Читай-город";
        std::cout << "  ✓ Магазины добавлены\n";

        auto st1 = session.add(std::make_unique<Stock>());
        st1.modify()->book = b1;
        st1.modify()->shop = s1;
        st1.modify()->count = 10;

        auto st2 = session.add(std::make_unique<Stock>());
        st2.modify()->book = b2;
        st2.modify()->shop = s1;
        st2.modify()->count = 5;

        auto st3 = session.add(std::make_unique<Stock>());
        st3.modify()->book = b1;
        st3.modify()->shop = s2;
        st3.modify()->count = 7;

        auto st4 = session.add(std::make_unique<Stock>());
        st4.modify()->book = b3;
        st4.modify()->shop = s3;
        st4.modify()->count = 20;
        std::cout << "  ✓ Остатки добавлены\n";

        auto sale1 = session.add(std::make_unique<Sale>());
        sale1.modify()->stock = st1;
        sale1.modify()->count = 2;
        sale1.modify()->price = 1200.00;
        sale1.modify()->dateSale = std::chrono::system_clock::now();
        std::cout << "  ✓ Продажи добавлены\n";

        txn.commit();
        std::cout << "✓ Все тестовые данные успешно добавлены.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "✗ Ошибка при добавлении данных: " << e.what() << "\n";
    }
}

/* ---------- 5. ЗАПРОС: МАГАЗИНЫ ПО ИЗДАТЕЛЮ ---------- */

std::vector<std::string>
findShopsByPublisher(dbo::Session& session, const std::string& publisherName) {
    std::vector<std::string> result;

    try {
        dbo::Transaction txn(session);

        dbo::collection<std::string> shops =
            session.query<std::string>(
                "SELECT DISTINCT sh.name "
                "FROM shop sh "
                "JOIN stock st ON st.id_shop = sh.id "
                "JOIN book b   ON b.id = st.id_book "
                "JOIN publisher p ON p.id = b.id_publisher "
                "WHERE p.name = ?")
            .bind(publisherName);

        for (const auto& s : shops)
            result.push_back(s);

        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << "\n";
    }

    return result;
}

/* ---------- 6. MAIN ---------- */

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "=== ЗАПУСК ПРОГРАММЫ ===\n\n";

    try {
        std::cout << "1. Создание подключения к PostgreSQL...\n";

        std::unique_ptr<dbo::backend::Postgres> postgres;

        try {
            postgres = std::make_unique<dbo::backend::Postgres>(
                "host=localhost "
                "port=5432 "
                "dbname=DZ_6_6 "
                "user=postgres "
                "password=12345678");
        }
        catch (const std::exception& e) {
            std::cerr << "✗ Ошибка подключения к БД: " << e.what() << "\n";
            std::cerr << "\nПроверьте:\n";
            std::cerr << "  1. PostgreSQL запущен\n";
            std::cerr << "  2. База данных 'DZ_6_6' существует\n";
            std::cerr << "  3. Пользователь 'postgres' с паролем '12345678'\n";
            std::cerr << "\nДля создания БД выполните в psql:\n";
            std::cerr << "  CREATE DATABASE \"DZ_6_6\";\n";
            return 1;
        }

        std::cout << "✓ Объект подключения создан.\n";

        std::cout << "\n2. Создание сессии...\n";
        dbo::Session session;
        session.setConnection(std::move(postgres));
        std::cout << "✓ Сессия создана и подключена.\n";

        std::cout << "\n3. Регистрация классов в ORM...\n";
        try {
            session.mapClass<Publisher>("publisher");
            std::cout << "  ✓ Publisher\n";

            session.mapClass<Book>("book");
            std::cout << "  ✓ Book\n";

            session.mapClass<Shop>("shop");
            std::cout << "  ✓ Shop\n";

            session.mapClass<Stock>("stock");
            std::cout << "  ✓ Stock\n";

            session.mapClass<Sale>("sale");
            std::cout << "  ✓ Sale\n";

            std::cout << "✓ Все классы зарегистрированы.\n";
        }
        catch (const std::exception& e) {
            std::cerr << "✗ Ошибка регистрации классов: " << e.what() << "\n";
            return 1;
        }

        std::cout << "\n4. Создание структуры БД...\n";
        createTables(session);

        std::cout << "\n5. Заполнение данными...\n";
        fillTestData(session);

        std::cout << "\n=== ПРОГРАММА ГОТОВА К РАБОТЕ ===\n\n";

        std::cout << "Введите имя издателя (например: Piter или BHV): ";
        std::string pubName;
        std::getline(std::cin, pubName);

        std::cout << "\n6. Выполнение запроса...\n";
        auto shops = findShopsByPublisher(session, pubName);

        std::cout << "\n=== РЕЗУЛЬТАТ ===\n";
        if (shops.empty()) {
            std::cout << "Издатель \"" << pubName
                << "\" не найден или его книги нигде не продаются.\n";
        }
        else {
            std::cout << "Книги издателя \"" << pubName
                << "\" продаются в магазинах:\n";
            for (const auto& name : shops)
                std::cout << "  • " << name << '\n';
        }
    }
    catch (const dbo::Exception& ex) {
        std::cerr << "\n✗ ОШИБКА Wt::Dbo: " << ex.what() << std::endl;
        return 1;
    }
    catch (const std::exception& ex) {
        std::cerr << "\n✗ ОШИБКА: " << ex.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "\n✗ НЕИЗВЕСТНАЯ ОШИБКА (Access Violation)!\n";
        std::cerr << "Возможно отсутствуют DLL библиотеки.\n";
        return 1;
    }

    std::cout << "\n=== ПРОГРАММА ЗАВЕРШЕНА УСПЕШНО ===\n";
    return 0;
}