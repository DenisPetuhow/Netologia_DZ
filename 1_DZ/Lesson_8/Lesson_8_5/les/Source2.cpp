#include <iostream>
#include <vector>
#include <algorithm> // Для std::swap
#include <unordered_map> // Включен в оригинальном коде, но не используется в этом примере.

using namespace std;

//Стратегия разная обработка в зависимости от входных данных

/**
 * @brief Абстрактный базовый класс для стратегий сортировки.
 * Это интерфейс, который должны реализовывать все конкретные стратегии.
 */
class SortStrategy {
public:
    // Чисто виртуальная функция для выполнения сортировки
    virtual void Sort(vector<int>& arr) = 0;

    // Виртуальный деструктор для корректного освобождения памяти
    virtual ~SortStrategy() {}
};

/**
 * @brief Конкретная стратегия, реализующая сортировку пузырьком.
 */
class BubbleSort : public SortStrategy {
public:
    void Sort(vector<int>& arr) override {
        cout << "Использование: Сортировка пузырьком (Bubble Sort)" << endl;
        size_t n = arr.size();

        // Основной цикл сортировки пузырьком
        for (size_t k = 0; k < n - 1; ++k) {
            // Внутренний цикл для сравнения и обмена элементов
            for (size_t i = 0; i < n - k - 1; ++i) {
                // Если текущий элемент больше следующего, меняем их местами
                if (arr[i] > arr[i + 1]) {
                    std::swap(arr[i], arr[i + 1]);
                }
            }
        }
    }
};

class QuickSort : public SortStrategy {
public:
    void Sort(vector<int>& arr) override {
        cout << "Использование: QuickSort" << endl;
        std::sort(arr.begin(), arr.end());
    }
};


/**
 * @brief Класс "Контекст", который использует стратегию.
 * Он хранит ссылку на объект стратегии и делегирует ей задачу сортировки.
 */
class Sorting {
private:
    SortStrategy* __strategy;

public:
    // Конструктор
    Sorting(size_t size)
    {
        if (size < 100) {
            __strategy = new BubbleSort();
        }
        else { __strategy = new QuickSort(); }
    }

    // Деструктор
    ~Sorting() {
        // Ответственность за удаление объекта стратегии обычно лежит на Context.
        // Но в зависимости от реализации, можно передать эту ответственность клиенту.
        // Здесь предполагаем, что Context владеет стратегией.
        delete __strategy;
    }

    // Установка новой стратегии
    void sort(vector<int>& arr) {
        __strategy->Sort(arr);
    }
};

  
int main3() {
    // 1. Создаем исходные данные
    vector<int> data (50, 1);
   Sorting Sort(data.size());
   Sort.sort(data);

    // Пример смены стратегии (если бы была другая, например QuickSort)
    // context.SetStrategy(new QuickSort());
    // context.ExecuteSort(data);

    return 0;
}