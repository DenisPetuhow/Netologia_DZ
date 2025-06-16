
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>

using namespace std;

// Рекурсивная функция поиска в глубину для пометки всех вершин, входящих в одну компоненту связности
void dfs(int** matrix, vector<int>& components, int n, int current, int component_number) {
    components[current] = component_number; // Помечаем вершину номером компоненты

    // Обходим все смежные вершины
    for (int i = 0; i < n; i++) {
        // Если между текущей вершиной и i-й есть ребро и она ещё не посещена (её компонент == 0)
        if (matrix[current][i] == 1 && components[i] == 0) {
            dfs(matrix, components, n, i, component_number);
        }
    }
}

int main() {
    // Русская кодировка для корректного отображения в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ifstream file("input.txt");
    if (!file.is_open()) {
        cout << "Ошибка открытия файла" << endl;
        return 1;
    }

    int n;
    file >> n;

    // Динамическое выделение памяти под матрицу смежности n x n
    int** matrix = new int* [n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];
    }

    // Чтение матрицы смежности из файла
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> matrix[i][j];
        }
    }

    // Вектор для хранения номера компоненты связности для каждой вершины (0 — не посещена)
    vector<int> components(n, 0);
    int component_number = 0;

    // Ищем компоненты связности
    for (int i = 0; i < n; i++) {
        // Если вершина еще не была посещена (не принадлежит компоненте)
        if (components[i] == 0) {
            component_number++; // Новый номер компоненты
            dfs(matrix, components, n, i, component_number);
        }
    }

    // Вывод количества компонент связности
    cout << "Количество компонент связности: " << component_number << endl;

    // Вывод номера компоненты для каждой вершины (вершины нумеруются с 1)
    cout << "Номера компонент для каждой вершины:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Вершина " << (i + 1) << ": " << components[i] << endl;
    }

    // Освобождение памяти
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    file.close();
    return 0;
}