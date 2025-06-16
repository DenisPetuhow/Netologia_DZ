#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

// Функция для поиска циклов с помощью DFS для неориентированного графа
// parent - вершина, из которой мы пришли в current
// Возвращает true, если найден цикл
bool dfs_cycle_check(int** matrix, bool* visited, int n, int current, int parent) {
    // Отмечаем текущую вершину как посещенную
    visited[current] = true;

    // Обходим все смежные вершины
    for (int i = 0; i < n; i++) {
        // Если есть ребро от current к i
        if (matrix[current][i] == 1) {
            // Пропускаем вершину, из которой мы пришли (предыдущая в пути)
            if (i == parent) {
                continue;
            }

            // Если вершина i не посещена, рекурсивно проверяем её
            if (!visited[i]) {
                if (dfs_cycle_check(matrix, visited, n, i, current)) {
                    return true; // Цикл найден
                }
            }
            // Если вершина i уже посещена И это не родительская вершина,
            // значит найден цикл длиной >= 3
            else {
                return true;
            }
        }
    }

    return false;
}

// Функция для проверки наличия циклов во всем неориентированном графе
bool has_cycle(int** matrix, int n) {
    // Массив для отслеживания посещенных вершин
    bool* visited = new bool[n];

    // Инициализируем массив
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Проверяем каждую непосещенную вершину
    // (граф может быть несвязным)
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            // Начинаем DFS с вершины i, parent = -1 (нет родителя)
            if (dfs_cycle_check(matrix, visited, n, i, -1)) {
                // Освобождаем память перед возвратом
                delete[] visited;
                return true;
            }
        }
    }

    // Освобождаем память
    delete[] visited;
    return false;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Открываем файл для чтения
    ifstream file("input.txt");
    if (!file.is_open()) {
        cout << "Ошибка открытия файла" << endl;
        return 1;
    }

    int n;
    file >> n;

    // Создаем двумерный динамический массив для матрицы смежности
    int** matrix = new int* [n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];
    }

    // Читаем матрицу смежности из файла
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> matrix[i][j];
        }
    }

    // Проверяем наличие циклов в графе
    if (has_cycle(matrix, n)) {
        cout << "В графе есть цикл!" << endl;
    }
    else {
        cout << "В графе нет циклов" << endl;
    }

    // Освобождаем память
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    file.close();
    return 0;
}