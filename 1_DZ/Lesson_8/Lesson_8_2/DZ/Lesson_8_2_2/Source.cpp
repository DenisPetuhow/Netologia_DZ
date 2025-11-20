#include <map>
#include <string>
#include <iostream>
#include <memory>

// --- Исходный класс (Субъект) ---
class BD {
public:
	virtual std::string GetData(const std::string& text) {
		return "Very Big Data String: " + text;
	}
	virtual ~BD() = default; // Важно: виртуальный деструктор для полиморфизма
};

// код кеширующего прокси
class Cach_BD : public BD {
public:
	// Принимает любой объект типа VeryHeavyDatabase (включая CacheProxyDB)
	explicit Cach_BD(std::unique_ptr<BD> text_bd) : _text_bd (move(text_bd)) {}
	std::string GetData(const std::string& key) override {
		if (_cache.find(key) == _cache.end()) { // если нет ключа перебрасывает в конец списка
			std::cout << "Get from real object\n";
			_cache[key] = _text_bd->GetData(key);
		}
		else {
			std::cout << "Get from cache\n";
		}
		return _cache.at(key); // вернуть результат
	}
private:
	std::unique_ptr<BD> _text_bd;
	std::map<std::string, std::string> _cache;
};

// --- РЕАЛИЗАЦИЯ ЗАДАЧИ: OneShotDB ---
class OneShotDB : public BD {
public:
	// Конструктор принимает "сырой" указатель на реальный объект и количество попыток
	explicit OneShotDB(BD* real_object, size_t shots = 1)
		: real_db_(real_object), shots_(shots) {
	}

	// Переопределяем метод получения данных
	std::string GetData(const std::string& key) override {
		// Проверяем, остались ли у нас "выстрелы" (попытки)
		if (shots_ > 0) {
			shots_--; // Уменьшаем счетчик
			// --- ВЫВОД КОЛИЧЕСТВА ОСТАВШИХСЯ ПОПЫТОК ---
			std::cout << "[OneShot] Запрос прошел. Осталось попыток: " << shots_ << std::endl;
			return real_db_->GetData(key); // Делегируем (передаем) запрос реальному объекту
		}
		// Если лимит исчерпан
		return "error";
	}

private:
	BD* real_db_; // Указатель на реальную базу (не владеет ею, просто ссылается)
	size_t shots_;               // Счетчик оставшихся запросов
};

int main() {
	setlocale(LC_ALL, "Russian");
	// 1.1 Создаем самую глубокую часть - реальную базу
	auto real_db = std::make_unique<BD>();

	// 1.2 Оборачиваем реальную базу в Кеш.
		// Теперь cached_db владеет real_db.
	auto cached_db = std::make_unique<Cach_BD>(std::move(real_db));
	// 1.3 Оборачиваем Кеш в Лимитер (OneShot).
	// OneShot принимает обычный указатель (observer pointer), он не владеет объектом.
	// Мы используем cached_db.get(), чтобы получить сырой указатель на кеш.
	OneShotDB limit_db(cached_db.get(), 2);

	// Запрос 1: OneShot пропускает -> Кеш не находит -> Реальная БД (Счетчик: 1)
	std::cout << "1: " << limit_db.GetData("key1") << "\n\n";

	// Запрос 2: OneShot пропускает -> Кеш находит -> Возврат из кеша (Счетчик: 0)
	std::cout << "2: " << limit_db.GetData("key1") << "\n\n";

	// Запрос 3: OneShot БЛОКИРУЕТ (Счетчик исчерпан). До кеша дело даже не дойдет.
	std::cout << "3: " << limit_db.GetData("key1") << "\n\n";

	std::cout << cached_db->GetData("key2") << std::endl;
	std::cout << cached_db->GetData("key2") << std::endl;
	std::cout << cached_db->GetData("key1") << std::endl;
	return 0;
}

/*комбинировать эти классы как конструктор LEGO.

Хотите кеш без лимита ? Используйте просто CacheProxyDB.

Хотите лимит без кеша ? Оерните VeryHeavyDatabase сразу в OneShotDB.

Хотите Кеш->Лимит->Логирование->База ? Просто накладывайте слои друг на друга.
*/ 
