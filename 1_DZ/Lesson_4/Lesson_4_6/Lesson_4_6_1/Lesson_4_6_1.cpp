
#include <iostream>
#include <Windows.h>
#include <vector>

using namespace std;

// Функция для вывода информации об узле пирамиды
void print_pyramid(const vector<int>& heap) {
    int n = heap.size();
    for (int i = 0; i < n; ++i) {
        int level = 0, j = i;
        // Определяем уровень текущего элемента, двигаясь вверх по пирамиде
        while (j > 0) {
            j = (j - 1) / 2; // Поиск родителя
            ++level;
        }

        // Определяем тип: root, left, right
        if (i == 0) {
            cout << level << " root " << heap[i] << endl;
        }
        else {
            int parent = (i - 1) / 2; // Индекс родителя
            if (i == 2 * parent + 1) {
                // Левый ребенок
                cout << level << " left(" << heap[parent] << ") " << heap[i] << endl;
            }
            else {
                // Правый ребенок
                cout << level << " right(" << heap[parent] << ") " << heap[i] << endl;
            }
        }
    }
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<int> heap1 = { 1, 3, 6, 5, 9, 8 };
    cout << "Исходный массив: ";
    for (int x : heap1) cout << x << " ";
    cout << endl;
    cout << "Пирамида:" << endl;
    print_pyramid(heap1);

    // Еще один пример
    vector<int> heap2 = { 10, 20, 30, 40 };
    cout << "\nИсходный массив: ";
    for (int x : heap2) cout << x << " ";
    cout << endl;
    cout << "Пирамида:" << endl;
    print_pyramid(heap2);

    return 0;

}

