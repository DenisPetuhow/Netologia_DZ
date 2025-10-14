#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <string>
#include <memory>
#include <iostream>

// Сокращенный псевдоним для удобства
namespace dbo = Wt::Dbo;

// ============================================================================
// КЛАСС PUBLISHER (Издатель)
// ============================================================================
class Publisher;
class Book;
class Shop;
class Stock;
class Sale;

class Publisher {
public:
    std::string name; // Название издательства

    // Коллекция книг, принадлежащих этому издателю
    // collection<> - контейнер для связанных объектов
    // ptr<Book> - умный указатель Wt::Dbo на объект Book
    dbo::collection<dbo::ptr<Book>> books;

    // Метод persist определяет маппинг класса на таблицу БД
    // Action - шаблонный параметр для различных операций (чтение/запись/схема)
    template<class Action>
    void persist(Action& a)
    {
        // field() - регистрирует обычное поле таблицы
        // Параметры: (action, переменная, "имя_столбца_в_БД")
        dbo::field(a, name, "name");

        // hasMany() - определяет связь "один ко многим" (1→N)
        // Параметры: (action, коллекция, тип_связи, "имя_FK_в_связанной_таблице")
        // ManyToOne означает, что со стороны Book это связь "многие к одному"
        dbo::hasMany(a, books, dbo::ManyToOne, "publisher");
    }
};

// ============================================================================
// КЛАСС BOOK (Книга)
// ============================================================================
class Book {
public:
    std::string title; // Название книги

    // Указатель на издателя этой книги
    dbo::ptr<Publisher> publisher;

    // Коллекция записей остатков (в каких магазинах есть эта книга)
    dbo::collection<dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, title, "title");

        // belongsTo() - определяет связь "многие к одному" (N→1)
        // Параметры: (action, указатель_на_родителя, "имя_FK")
        // Создаст столбец publisher_id в таблице book
        dbo::belongsTo(a, publisher, "publisher");

        // Связь "один ко многим" с остатками
        dbo::hasMany(a, stocks, dbo::ManyToOne, "book");
    }
};

// ============================================================================
// КЛАСС SHOP (Магазин)
// ============================================================================
class Shop {
public:
    std::string name; // Название магазина

    // Коллекция остатков книг в этом магазине
    dbo::collection<dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, name, "name");

        // Связь "один ко многим" с остатками
        dbo::hasMany(a, stocks, dbo::ManyToOne, "shop");
    }
};

// ============================================================================
// КЛАСС STOCK (Остатки - связь книг и магазинов)
// ============================================================================
class Stock {
public:
    int count; // Количество экземпляров книги в магазине

    // Указатель на книгу
    dbo::ptr<Book> book;

    // Указатель на магазин
    dbo::ptr<Shop> shop;

    // Коллекция продаж из этого остатка
    dbo::collection<dbo::ptr<Sale>> sales;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, count, "count");

        // Связь "многие к одному" с книгой
        // Создаст столбец book_id
        dbo::belongsTo(a, book, "book");

        // Связь "многие к одному" с магазином
        // Создаст столбец shop_id
        dbo::belongsTo(a, shop, "shop");

        // Связь "один ко многим" с продажами
        dbo::hasMany(a, sales, dbo::ManyToOne, "stock");
    }
};

// ============================================================================
// КЛАСС SALE (Продажа)
// ============================================================================
class Sale {
public:
    double price;      // Цена продажи
    std::string date_sale; // Дата продажи (используем string вместо WDateTime)
    int count;         // Количество проданных экземпляров

    // Указатель на запись остатка, из которого произведена продажа
    dbo::ptr<Stock> stock;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, price, "price");
        dbo::field(a, date_sale, "date_sale");
        dbo::field(a, count, "count");

        // Связь "многие к одному" с остатком
        // Создаст столбец stock_id
        dbo::belongsTo(a, stock, "stock");
    }
};

// ============================================================================
// ЗАДАНИЕ 2: ПРОГРАММА С ЗАПРОСОМ
// ============================================================================

int main()
{
    try
    {
        // ====================================================================
        // 1. ПОДКЛЮЧЕНИЕ К POSTGRESQL
        // ====================================================================

        // Строка подключения к PostgreSQL
        // Формат: "параметр=значение параметр=значение ..."
        std::string connectionString =
            "host=localhost "          // Адрес сервера БД
            "port=5432 "               // Порт PostgreSQL (по умолчанию 5432)
            "dbname=bookstore "        // Имя базы данных
            "user=postgres "           // Имя пользователя
            "password=your_password";  // Пароль

        // Создаём backend для PostgreSQL
        // make_unique - создаёт unique_ptr с объектом
        auto postgres = std::make_unique<dbo::backend::Postgres>(connectionString);

        // Создаём сессию - основной объект для работы с БД
        // Session управляет соединением и транзакциями
        dbo::Session session;

        // setConnection() - привязывает backend к сессии
        // std::move() передаёт ownership указателя
        session.setConnection(std::move(postgres));

        // ====================================================================
        // 2. РЕГИСТРАЦИЯ КЛАССОВ И СОЗДАНИЕ ТАБЛИЦ
        // ====================================================================

        // mapClass() - регистрирует класс C++ как таблицу БД
        // Параметры: <Класс>("имя_таблицы")
        session.mapClass<Publisher>("publisher");
        session.mapClass<Book>("book");
        session.mapClass<Shop>("shop");
        session.mapClass<Stock>("stock");
        session.mapClass<Sale>("sale");

        // createTables() - создаёт все таблицы в БД если их нет
        // Анализирует persist() методы и генерирует CREATE TABLE
        try {
            session.createTables();
            std::cout << "Таблицы успешно созданы.\n\n";
        }
        catch (const dbo::Exception& e) {
            // Если таблицы уже существуют, продолжаем работу
            std::cout << "Таблицы уже существуют: " << e.what() << "\n\n";
        }

        // ====================================================================
        // 3. ЗАПОЛНЕНИЕ ТЕСТОВЫМИ ДАННЫМИ
        // ====================================================================

        {
            // Transaction - RAII-обертка для транзакции БД
            // Автоматически откатывается при исключении
            dbo::Transaction transaction(session);

            // Проверяем, есть ли уже данные
            int publisherCount = session.query<int>("SELECT COUNT(*) FROM publisher");

            if (publisherCount == 0) {
                std::cout << "Заполнение базы тестовыми данными...\n";

                // ---- СОЗДАНИЕ ИЗДАТЕЛЕЙ ----

                // Создаём объект Publisher в куче
                std::unique_ptr<Publisher> pub1(new Publisher());
                pub1->name = "Эксмо";

                // add() - добавляет объект в БД и возвращает dbo::ptr
                // std::move() передаёт ownership в сессию
                dbo::ptr<Publisher> publisher1 = session.add(std::move(pub1));

                std::unique_ptr<Publisher> pub2(new Publisher());
                pub2->name = "АСТ";
                dbo::ptr<Publisher> publisher2 = session.add(std::move(pub2));

                std::unique_ptr<Publisher> pub3(new Publisher());
                pub3->name = "Питер";
                dbo::ptr<Publisher> publisher3 = session.add(std::move(pub3));

                std::cout << "  ✓ Создано 3 издателя\n";

                // ---- СОЗДАНИЕ КНИГ ----

                // Книги издателя "Эксмо"
                std::unique_ptr<Book> book1(new Book());
                book1->title = "Мастер и Маргарита";
                book1->publisher = publisher1; // Связываем с издателем
                dbo::ptr<Book> bookPtr1 = session.add(std::move(book1));

                std::unique_ptr<Book> book2(new Book());
                book2->title = "Анна Каренина";
                book2->publisher = publisher1;
                dbo::ptr<Book> bookPtr2 = session.add(std::move(book2));

                // Книги издателя "АСТ"
                std::unique_ptr<Book> book3(new Book());
                book3->title = "Война и мир";
                book3->publisher = publisher2;
                dbo::ptr<Book> bookPtr3 = session.add(std::move(book3));

                // Книги издателя "Питер"
                std::unique_ptr<Book> book4(new Book());
                book4->title = "Алгоритмы на C++";
                book4->publisher = publisher3;
                dbo::ptr<Book> bookPtr4 = session.add(std::move(book4));

                std::unique_ptr<Book> book5(new Book());
                book5->title = "Чистый код";
                book5->publisher = publisher3;
                dbo::ptr<Book> bookPtr5 = session.add(std::move(book5));

                std::cout << "  ✓ Создано 5 книг\n";

                // ---- СОЗДАНИЕ МАГАЗИНОВ ----

                std::unique_ptr<Shop> shop1(new Shop());
                shop1->name = "Буквоед на Невском";
                dbo::ptr<Shop> shopPtr1 = session.add(std::move(shop1));

                std::unique_ptr<Shop> shop2(new Shop());
                shop2->name = "Москва";
                dbo::ptr<Shop> shopPtr2 = session.add(std::move(shop2));

                std::unique_ptr<Shop> shop3(new Shop());
                shop3->name = "Читай-город";
                dbo::ptr<Shop> shopPtr3 = session.add(std::move(shop3));

                std::cout << "  ✓ Создано 3 магазина\n";

                // ---- СОЗДАНИЕ ОСТАТКОВ (связь книг и магазинов) ----

                // Магазин "Буквоед" - книги Эксмо и Питер
                std::unique_ptr<Stock> stock1(new Stock());
                stock1->book = bookPtr1;      // Мастер и Маргарита
                stock1->shop = shopPtr1;      // Буквоед
                stock1->count = 10;
                dbo::ptr<Stock> stockPtr1 = session.add(std::move(stock1));

                std::unique_ptr<Stock> stock2(new Stock());
                stock2->book = bookPtr4;      // Алгоритмы на C++
                stock2->shop = shopPtr1;
                stock2->count = 5;
                dbo::ptr<Stock> stockPtr2 = session.add(std::move(stock2));

                // Магазин "Москва" - книги всех издателей
                std::unique_ptr<Stock> stock3(new Stock());
                stock3->book = bookPtr2;      // Анна Каренина (Эксмо)
                stock3->shop = shopPtr2;
                stock3->count = 8;
                dbo::ptr<Stock> stockPtr3 = session.add(std::move(stock3));

                std::unique_ptr<Stock> stock4(new Stock());
                stock4->book = bookPtr3;      // Война и мир (АСТ)
                stock4->shop = shopPtr2;
                stock4->count = 15;
                dbo::ptr<Stock> stockPtr4 = session.add(std::move(stock4));

                std::unique_ptr<Stock> stock5(new Stock());
                stock5->book = bookPtr5;      // Чистый код (Питер)
                stock5->shop = shopPtr2;
                stock5->count = 12;
                dbo::ptr<Stock> stockPtr5 = session.add(std::move(stock5));

                // Магазин "Читай-город" - только Питер
                std::unique_ptr<Stock> stock6(new Stock());
                stock6->book = bookPtr4;      // Алгоритмы на C++
                stock6->shop = shopPtr3;
                stock6->count = 20;
                dbo::ptr<Stock> stockPtr6 = session.add(std::move(stock6));

                std::unique_ptr<Stock> stock7(new Stock());
                stock7->book = bookPtr5;      // Чистый код
                stock7->shop = shopPtr3;
                stock7->count = 7;
                dbo::ptr<Stock> stockPtr7 = session.add(std::move(stock7));

                std::cout << "  ✓ Создано 7 записей остатков\n";

                // ---- СОЗДАНИЕ ПРОДАЖ ----

                std::unique_ptr<Sale> sale1(new Sale());
                sale1->stock = stockPtr1;     // Мастер и Маргарита в Буквоеде
                sale1->price = 450.50;
                sale1->date_sale = "2024-01-15";
                sale1->count = 2;
                session.add(std::move(sale1));

                std::unique_ptr<Sale> sale2(new Sale());
                sale2->stock = stockPtr2;     // Алгоритмы в Буквоеде
                sale2->price = 890.00;
                sale2->date_sale = "2024-01-16";
                sale2->count = 1;
                session.add(std::move(sale2));

                std::unique_ptr<Sale> sale3(new Sale());
                sale3->stock = stockPtr5;     // Чистый код в Москве
                sale3->price = 750.00;
                sale3->date_sale = "2024-01-17";
                sale3->count = 3;
                session.add(std::move(sale3));

                std::cout << "  ✓ Создано 3 продажи\n\n";
            }

            // commit() - фиксирует все изменения в БД
            // Если не вызвать, транзакция откатится при выходе из блока
            transaction.commit();
        }

        // ====================================================================
        // 4. ЗАПРОС: ПОИСК МАГАЗИНОВ ПО ИЗДАТЕЛЮ
        // ====================================================================

        std::cout << "==============================================\n";
        std::cout << "Введите название издателя: ";
        std::string publisherName;
        std::getline(std::cin, publisherName);
        std::cout << "==============================================\n\n";

        {
            dbo::Transaction transaction(session);

            // find<Publisher>() - начинает построение запроса SELECT
            // where() - добавляет условие WHERE
            // bind() - безопасно подставляет параметр (защита от SQL-инъекций)
            dbo::ptr<Publisher> targetPublisher =
                session.find<Publisher>()
                .where("name = ?")
                .bind(publisherName);

            // Проверяем, найден ли издатель
            // Если нет, targetPublisher будет "пустым"
            if (!targetPublisher) {
                std::cout << "❌ Издатель \"" << publisherName << "\" не найден.\n";
                transaction.commit();
                return 0;
            }

            std::cout << "📚 Издатель: " << targetPublisher->name << "\n\n";

            // Выводим книги издателя
            std::cout << "Книги издателя:\n";
            // Итерация по коллекции books (автоматически загружается из БД)
            for (const dbo::ptr<Book>& book : targetPublisher->books) {
                std::cout << "  • " << book->title << "\n";
            }
            std::cout << "\n";

            // ---- ЗАПРОС МАГАЗИНОВ ЧЕРЕЗ SQL ----

            // query<ResultType>() - выполняет произвольный SQL-запрос
            // QueryModel<Shop> - результат будет набором объектов Shop
            typedef dbo::collection<dbo::ptr<Shop>> Shops;

            // Запрос с JOIN через связанные таблицы
            Shops shops = session.query<dbo::ptr<Shop>>(
                // DISTINCT - убирает дубликаты магазинов
                "SELECT DISTINCT s FROM shop s "
                // JOIN с таблицей остатков
                "JOIN stock st ON st.shop_id = s.id "
                // JOIN с таблицей книг
                "JOIN book b ON b.id = st.book_id "
                // Условие: книга принадлежит нужному издателю
                "WHERE b.publisher_id = ?"
            ).bind(targetPublisher.id()); // .id() возвращает первичный ключ

            // Выводим результаты
            std::cout << "🏪 Магазины, продающие книги издателя \""
                << publisherName << "\":\n\n";

            // size() - количество результатов (выполняет COUNT запрос)
            if (shops.size() == 0) {
                std::cout << "  Не найдено магазинов.\n";
            }
            else {
                // Итерация по результатам запроса
                for (const dbo::ptr<Shop>& shop : shops) {
                    std::cout << "  🏬 " << shop->name << "\n";

                    // Дополнительно выводим, какие книги издателя есть в магазине
                    std::cout << "     Книги в наличии:\n";

                    // Проходим по остаткам магазина
                    for (const dbo::ptr<Stock>& stock : shop->stocks) {
                        // Проверяем, что книга принадлежит нужному издателю
                        if (stock->book->publisher.id() == targetPublisher.id()) {
                            std::cout << "       - " << stock->book->title
                                << " (остаток: " << stock->count << " шт.)\n";
                        }
                    }
                    std::cout << "\n";
                }
            }

            // ---- АЛЬТЕРНАТИВНЫЙ СПОСОБ: ЧЕРЕЗ КОЛЛЕКЦИИ ----

            std::cout << "==============================================\n";
            std::cout << "Альтернативный подход (через навигацию по объектам):\n\n";

            // Используем set для уникальных магазинов
            std::set<long long> uniqueShopIds;

            // Проходим по всем книгам издателя
            for (const dbo::ptr<Book>& book : targetPublisher->books) {
                // Для каждой книги смотрим остатки
                for (const dbo::ptr<Stock>& stock : book->stocks) {
                    // Добавляем id магазина в set
                    uniqueShopIds.insert(stock->shop.id());
                }
            }

            // Выводим найденные магазины
            for (long long shopId : uniqueShopIds) {
                // load<Shop>() - загружает объект по первичному ключу
                dbo::ptr<Shop> shop = session.load<Shop>(shopId);
                std::cout << "  🏬 " << shop->name << "\n";
            }

            transaction.commit();
        }

        std::cout << "\n✅ Программа завершена успешно.\n";

    }
    catch (const dbo::Exception& e)
    {
        // Обработка исключений Wt::Dbo
        std::cerr << "❌ Ошибка БД: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e)
    {
        // Обработка других исключений
        std::cerr << "❌ Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}