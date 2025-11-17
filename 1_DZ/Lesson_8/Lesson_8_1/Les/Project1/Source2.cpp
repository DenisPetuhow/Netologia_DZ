// строитель, если много полей у класса, тогда елементы выделяются в отдельную структуру, а отдельные классы выделяются для определеных елементов


//"Строитель" (Builder) идеально подходит, когда у нас есть сложный объект с множеством полей (как в твоем примере Event) или когда процесс создания объекта состоит из нескольких шагов, которые могут выполняться в разном порядке (как в нашем задании с SQL-запросом).



// когда тяжелый объект, и мы его инкапсулируем не сразу, а постепенно, когда это необходимо

// Подключение необходимых библиотек
#include "map"        // для std::map - ассоциативный контейнер (словарь)
#include "vector"     // для std::vector - динамический массив
#include "chrono"     // для работы со временем
#include "iostream"   // для ввода-вывода (cout)

// ===============================================
// СТРУКТУРА EVENT - ОПИСАНИЕ СОБЫТИЯ
// ===============================================
struct Event {
    std::string name;        // Название события (например, "VeryImportantEvent")
    std::string message;     // Сообщение события
    std::string description; // Описание события
    int level;               // Уровень важности события (например, 1-5)
    std::string action;      // Действие, связанное с событием
    bool isActive;           // Флаг активности события

    // Временная метка - момент создания события
    std::chrono::time_point<std::chrono::steady_clock> timestamp;

    uint8_t ttl;            // Time To Live - время жизни события (0-255)
    std::string target;     // Получатель события (куда отправляем)
    std::string source;     // Источник события (откуда отправляем)

    // Маршрут события - список промежуточных точек
    std::vector<std::string> route;

    // Мета-данные - дополнительная информация в формате ключ-значение
    std::map<std::string, std::string> meta;
};

// ===============================================
// КЛАСС EVENTBUILDER - ПАТТЕРН "СТРОИТЕЛЬ"
// ===============================================
// Этот класс упрощает создание сложных объектов Event
// через цепочку вызовов методов (fluent interface)
class EventBuilder {
public:
    // КОНСТРУКТОР
    // Принимает обязательный параметр - имя события
    explicit EventBuilder(const std::string& event_name) {
        event.name = event_name;
        // Автоматически генерируем описание
        event.description = "This is " + event_name;
        // Устанавливаем временную метку - текущее время
        event.timestamp = std::chrono::steady_clock::now();
    }

    // МЕТОД ФИНАЛИЗАЦИИ - ПОСТРОЕНИЕ СОБЫТИЯ
    // noexcept - гарантирует, что метод не выбросит исключение
    Event BuildEvent() noexcept {
        // Если источник не указан, но есть маршрут -
        // берем первую точку маршрута как источник
        if (event.source.empty() && !event.route.empty()) {
            event.source = event.route.front();
        }

        // Если получатель не указан, но есть маршрут -
        // берем последнюю точку маршрута как получателя
        if (event.target.empty() && !event.route.empty()) {
            event.target = event.route.back();
        }

        // Возвращаем готовое событие
        return event;
    }

    // МЕТОД ДОБАВЛЕНИЯ УРОВНЯ ВАЖНОСТИ
    // Возвращает ссылку на себя (*this) для цепочки вызовов
    EventBuilder& AddLevel(int level) noexcept {
        event.level = level;
        return *this;  // Позволяет писать builder.AddLevel(3).AddMeta(...)
    }

    // МЕТОД ДОБАВЛЕНИЯ МЕТА-ДАННЫХ
    // Добавляет пару ключ-значение в словарь meta
    EventBuilder& AddMeta(const std::string& key, const std::string& value) {
        event.meta[key] = value;
        return *this;  // Возврат себя для цепочки
    }

    // МЕТОД ДОБАВЛЕНИЯ ТОЧКИ МАРШРУТА
    // Добавляет новую точку в конец маршрута
    EventBuilder& AddRoutePoint(const std::string& point) {
        event.route.push_back(point);
        return *this;  // Возврат себя для цепочки
    }

private:
    Event event;  // Объект события, который мы строим
};

// ===============================================
// ФУНКЦИЯ ОТПРАВКИ СОБЫТИЯ
// ===============================================
// Принимает событие по константной ссылке (не копирует, не изменяет)
void SendEvent(const Event& e) {
    // Выводим основную информацию о событии
    std::cout << "Sending event " << e.name
        << " to " << e.target
        << " from " << e.source << std::endl;

    // Выводим маршрут события
    std::cout << "route is: \n";
    for (const auto& point : e.route) {
        std::cout << point << "->";
    }
}

// ===============================================
// ГЛАВНАЯ ФУНКЦИЯ - ДЕМОНСТРАЦИЯ
// ===============================================
int main3() {
    // 1. Создаем строитель с именем события "VeryImportantEvent"
    EventBuilder builder("VeryImportantEvent");

    // 2. Настраиваем событие через цепочку вызовов:
    //    - Устанавливаем уровень важности = 3
    //    - Добавляем мета-данные: ключ "key" со значением "value"
    builder.AddLevel(3).AddMeta("key", "value");

    // 3. Добавляем маршрут: first -> second
    builder.AddRoutePoint("first").AddRoutePoint("second").AddRoutePoint("thrid");

    // 4. Строим событие и отправляем его
    //    BuildEvent() создаст объект Event, где:
    //    - source = "first" (первая точка маршрута)
    //    - target = "second" (последняя точка маршрута)
    SendEvent(builder.BuildEvent());

    return 0;
}