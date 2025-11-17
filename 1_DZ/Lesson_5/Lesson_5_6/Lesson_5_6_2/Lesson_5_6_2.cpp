#include <iostream>
#include <set>
#include <vector>
#include <list>

/*
 * print_container
 * ----------------
 * Универсальная шаблонная функция для печати любого контейнера
 * (std::vector, std::list, std::set, std::deque, std::map, ...).
 *
 * Параметр TContainer может быть:
 *   - контейнером STL (у которого есть begin()/end())
 *   - C-style массивом (int arr[4]) — тоже отработает
 *
 * Алгоритм:
 *   1. Берём итераторы begin() и end().
 *   2. Перебираем элементы.
 *   3. После каждого элемента, кроме последнего, выводим разделитель.
 *
 * Параметры:
 *   container — сам контейнер (передаётся по константной ссылке,
 *               чтобы избежать лишнего копирования).
 *   sep       — строка-разделитель между элементами (по умолчанию ", ").
 *   os        — поток вывода (по умолчанию std::cout).
 */
template <typename TContainer>
void print_container(const TContainer& container,
    const std::string& sep = ", ",
    std::ostream& os = std::cout)
{
    // auto& используем, чтобы не копировать каждый элемент
    auto it = container.begin();
    const auto end = container.end();

    while (it != end)
    {
        os << *it;                   // печатаем текущий элемент
        ++it;

        // Если это не последний элемент — печатаем разделитель
        if (it != end)
            os << sep;
    }
    // По желанию можно добавить перевод строки:

}

int main()
{
    // std::set хранит элементы в отсортированном порядке (лексикографический
    // для std::string), поэтому вывод: four one three two
    std::set<std::string> test_set = { "one", "two", "three", "four" };
    print_container(test_set, " ");   // four one three two
    std::cout << '\n';

    // std::list сохраняет порядок вставки
    std::list<std::string> test_list = { "one", "two", "three", "four" };
    print_container(test_list);       // one, two, three, four
    std::cout << '\n';

    // std::vector тоже сохраняет порядок
    std::vector<std::string> test_vector = { "one", "two", "three", "four" };
    print_container(test_vector, " - "); // one - two - three - four
    std::cout << '\n';
}