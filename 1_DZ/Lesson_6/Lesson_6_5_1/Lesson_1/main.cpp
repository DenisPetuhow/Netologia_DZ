#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "Windows.h"

// Класс для управления клиентами в базе данных
class ClientManager {
private:
    std::string connection_string; // Строка подключения к БД

public:
    // Конструктор принимает параметры подключения
    ClientManager(const std::string& conn_str) : connection_string(conn_str) {}

    // Метод 1: Создание структуры БД (таблицы clients и phones)
    void createTables() {
        try {
            pqxx::connection conn(connection_string);
            pqxx::work txn(conn);

            // Удаляем таблицы если они существуют (для чистоты эксперимента)
            txn.exec("DROP TABLE IF EXISTS phones CASCADE;");
            txn.exec("DROP TABLE IF EXISTS clients CASCADE;");

            // Создаём таблицу клиентов
            txn.exec(
                "CREATE TABLE clients ("
                "id SERIAL PRIMARY KEY, "           // Уникальный идентификатор
                "first_name VARCHAR(100) NOT NULL, " // Имя клиента
                "last_name VARCHAR(100) NOT NULL, "  // Фамилия клиента
                "email VARCHAR(100) UNIQUE NOT NULL" // Email (уникальный)
                ");"
            );

            // Создаём таблицу телефонов
            txn.exec(
                "CREATE TABLE phones ("
                "id SERIAL PRIMARY KEY, "              // Уникальный идентификатор
                "client_id INTEGER NOT NULL, "         // ID клиента
                "phone_number VARCHAR(20) NOT NULL, "  // Номер телефона
                "FOREIGN KEY (client_id) REFERENCES clients(id) ON DELETE CASCADE" // Связь с клиентом
                ");"
            );

            txn.commit(); // Подтверждаем транзакцию
            std::cout << "Таблицы успешно созданы!" << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка создания таблиц: " << e.what() << std::endl;
        }
    }

    // Метод 2: Добавление нового клиента
    // Возвращает ID созданного клиента или -1 в случае ошибки
    int addClient(const std::string& first_name,
        const std::string& last_name,
        const std::string& email) {
        try {
            pqxx::connection conn(connection_string);
            pqxx::work txn(conn);

            // Экранируем данные для безопасности
            std::string query = "INSERT INTO clients (first_name, last_name, email) "
                "VALUES (" +
                txn.quote(first_name) + ", " +
                txn.quote(last_name) + ", " +
                txn.quote(email) +
                ") RETURNING id;"; // RETURNING id возвращает ID нового клиента

            pqxx::result res = txn.exec(query);
            txn.commit();

            int client_id = res[0][0].as<int>(); // Получаем ID созданного клиента
            std::cout << "Клиент добавлен с ID: " << client_id << std::endl;
            return client_id;
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка добавления клиента: " << e.what() << std::endl;
            return -1;
        }
    }

    // Метод 3: Добавление телефона существующему клиенту
    void addPhone(int client_id, const std::string& phone_number) {
        try {
            pqxx::connection conn(connection_string);
            pqxx::work txn(conn);

            // Проверяем, существует ли клиент
            std::string check_query = "SELECT COUNT(*) FROM clients WHERE id = " +
                std::to_string(client_id) + ";";
            pqxx::result check_res = txn.exec(check_query);

            if (check_res[0][0].as<int>() == 0) {
                std::cout << "Клиент с ID " << client_id << " не найден!" << std::endl;
                return;
            }

            // Добавляем телефон
            std::string query = "INSERT INTO phones (client_id, phone_number) VALUES (" +
                std::to_string(client_id) + ", " +
                txn.quote(phone_number) + ");";

            txn.exec(query);
            txn.commit();
            std::cout << "Телефон добавлен клиенту с ID: " << client_id << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка добавления телефона: " << e.what() << std::endl;
        }
    }

    // Метод 4: Изменение данных о клиенте
    void updateClient(int client_id,
        const std::string& first_name,
        const std::string& last_name,
        const std::string& email) {
        try {
            pqxx::connection conn(connection_string);
            pqxx::work txn(conn);

            std::string query = "UPDATE clients SET "
                "first_name = " + txn.quote(first_name) + ", "
                "last_name = " + txn.quote(last_name) + ", "
                "email = " + txn.quote(email) +
                " WHERE id = " + std::to_string(client_id) + ";";

            pqxx::result res = txn.exec(query);
            txn.commit();

            // Проверяем, была ли обновлена хотя бы одна строка
            if (res.affected_rows() > 0) {
                std::cout << "Данные клиента обновлены!" << std::endl;
            }
            else {
                std::cout << "Клиент с ID " << client_id << " не найден!" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка обновления клиента: " << e.what() << std::endl;
        }
    }

    // Метод 5: Удаление телефона у существующего клиента
    void deletePhone(int client_id, const std::string& phone_number) {
        try {
            pqxx::connection conn(connection_string);
            pqxx::work txn(conn);

            std::string query = "DELETE FROM phones WHERE client_id = " +
                std::to_string(client_id) +
                " AND phone_number = " + txn.quote(phone_number) + ";";

            pqxx::result res = txn.exec(query);
            txn.commit();

            if (res.affected_rows() > 0) {
                std::cout << "Телефон удалён!" << std::endl;
            }
            else {
                std::cout << "Телефон не найден!" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка удаления телефона: " << e.what() << std::endl;
        }
    }

    // Метод 6: Удаление существующего клиента
    // Благодаря CASCADE, все телефоны клиента удалятся автоматически
    void deleteClient(int client_id) {
        try {
            pqxx::connection conn(connection_string);
            pqxx::work txn(conn);

            std::string query = "DELETE FROM clients WHERE id = " +
                std::to_string(client_id) + ";";

            pqxx::result res = txn.exec(query);
            txn.commit();

            if (res.affected_rows() > 0) {
                std::cout << "Клиент удалён!" << std::endl;
            }
            else {
                std::cout << "Клиент с ID " << client_id << " не найден!" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка удаления клиента: " << e.what() << std::endl;
        }
    }

    // Метод 7: Поиск клиента по его данным
    void findClient(const std::string& search_value) {
        try {
            pqxx::connection conn(connection_string);
            pqxx::work txn(conn);

            // Ищем по всем полям: имя, фамилия, email или телефон
            std::string query =
                "SELECT DISTINCT c.id, c.first_name, c.last_name, c.email "
                "FROM clients c "
                "LEFT JOIN phones p ON c.id = p.client_id "
                "WHERE c.first_name ILIKE " + txn.quote("%" + search_value + "%") +
                " OR c.last_name ILIKE " + txn.quote("%" + search_value + "%") +
                " OR c.email ILIKE " + txn.quote("%" + search_value + "%") +
                " OR p.phone_number ILIKE " + txn.quote("%" + search_value + "%") + ";";

            pqxx::result res = txn.exec(query);

            if (res.empty()) {
                std::cout << "Клиенты не найдены!" << std::endl;
                return;
            }

            std::cout << "\n=== Найденные клиенты ===" << std::endl;
            for (const auto& row : res) {
                int id = row[0].as<int>();
                std::string first_name = row[1].as<std::string>();
                std::string last_name = row[2].as<std::string>();
                std::string email = row[3].as<std::string>();

                std::cout << "\nID: " << id << std::endl;
                std::cout << "Имя: " << first_name << std::endl;
                std::cout << "Фамилия: " << last_name << std::endl;
                std::cout << "Email: " << email << std::endl;

                // Получаем все телефоны клиента
                std::string phone_query = "SELECT phone_number FROM phones WHERE client_id = " +
                    std::to_string(id) + ";";
                pqxx::result phone_res = txn.exec(phone_query);

                if (!phone_res.empty()) {
                    std::cout << "Телефоны: ";
                    for (size_t i = 0; i < phone_res.size(); ++i) {
                        std::cout << phone_res[i][0].as<std::string>();
                        if (i < phone_res.size() - 1) std::cout << ", ";
                    }
                    std::cout << std::endl;
                }
                else {
                    std::cout << "Телефонов нет" << std::endl;
                }
            }
            std::cout << "========================\n" << std::endl;

        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка поиска клиента: " << e.what() << std::endl;
        }
    }

    // Дополнительный метод: вывод всех клиентов
    void showAllClients() {
        try {
            pqxx::connection conn(connection_string);
            pqxx::work txn(conn);

            pqxx::result res = txn.exec("SELECT id, first_name, last_name, email FROM clients;");

            if (res.empty()) {
                std::cout << "База данных пуста!" << std::endl;
                return;
            }

            std::cout << "\n=== Все клиенты ===" << std::endl;
            for (const auto& row : res) {
                int id = row[0].as<int>();
                std::cout << "\nID: " << id << std::endl;
                std::cout << "Имя: " << row[1].as<std::string>() << std::endl;
                std::cout << "Фамилия: " << row[2].as<std::string>() << std::endl;
                std::cout << "Email: " << row[3].as<std::string>() << std::endl;

                // Получаем телефоны
                std::string phone_query = "SELECT phone_number FROM phones WHERE client_id = " +
                    std::to_string(id) + ";";
                pqxx::result phone_res = txn.exec(phone_query);

                if (!phone_res.empty()) {
                    std::cout << "Телефоны: ";
                    for (size_t i = 0; i < phone_res.size(); ++i) {
                        std::cout << phone_res[i][0].as<std::string>();
                        if (i < phone_res.size() - 1) std::cout << ", ";
                    }
                    std::cout << std::endl;
                }
            }
            std::cout << "===================\n" << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка вывода клиентов: " << e.what() << std::endl;
        }
    }
};

// Главная функция - демонстрация работы класса
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    try {
        // Создаём строку подключения
        std::string connection_str =
            "host=localhost "
            "port=5432 "
            "dbname=DZ_6_5 "
            "user=postgres "
            "password=12345678";

        // Создаём объект менеджера клиентов
        ClientManager manager(connection_str);

        std::cout << "=== ДЕМОНСТРАЦИЯ РАБОТЫ ПРОГРАММЫ ===\n" << std::endl;

        // 1. Создаём структуру БД
        std::cout << "1. Создание структуры БД..." << std::endl;
        manager.createTables();
        std::cout << std::endl;

        // 2. Добавляем клиентов
        std::cout << "2. Добавление клиентов..." << std::endl;
        int client1_id = manager.addClient("Den", "pet", "ivanov@mail.ru");
        int client2_id = manager.addClient("Sam", "gghj", "petrov@mail.ru");
        int client3_id = manager.addClient("ASS", "ABG", "sidorova@mail.ru");
        std::cout << std::endl;

        // 3. Добавляем телефоны
        std::cout << "3. Добавление телефонов..." << std::endl;
        manager.addPhone(client1_id, "+7-900-123-45-67");
        manager.addPhone(client1_id, "+7-900-123-45-68"); // У Иванова два телефона
        manager.addPhone(client2_id, "+7-911-222-33-44");
        // У Марии телефона нет - это нормально
        std::cout << std::endl;

        // 4. Показываем всех клиентов
        std::cout << "4. Просмотр всех клиентов..." << std::endl;
        manager.showAllClients();

        // 5. Изменяем данные клиента
        std::cout << "5. Изменение email клиента Иванова..." << std::endl;
        manager.updateClient(client1_id, "Den", "pet", "ivanov_new@mail.ru");
        std::cout << std::endl;

        // 6. Поиск клиента по имени
        std::cout << "6. Поиск клиента по имени 'Den'..." << std::endl;
        manager.findClient("Den");

        // 7. Поиск клиента по телефону
        std::cout << "7. Поиск клиента по телефону '+7-911'..." << std::endl;
        manager.findClient("+7-911");

        // 8. Поиск клиента по email
        std::cout << "8. Поиск клиента по email 'sidorova'..." << std::endl;
        manager.findClient("sidorova");

        // 9. Удаляем телефон
        std::cout << "9. Удаление одного телефона у Иванова..." << std::endl;
        manager.deletePhone(client1_id, "+7-900-123-45-68");
        manager.showAllClients();

        // 10. Удаляем клиента
        std::cout << "10. Удаление клиента Sam..." << std::endl;
        manager.deleteClient(client2_id);
        manager.showAllClients();

        std::cout << "=== ПРОГРАММА ЗАВЕРШЕНА УСПЕШНО ===" << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}