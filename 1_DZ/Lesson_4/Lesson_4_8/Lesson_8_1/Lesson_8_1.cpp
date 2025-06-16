#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

void dfs_recursive(int** matrix, bool* visited, int n, int current) {
    visited[current] = true;
    cout << current + 1 << " "; // +1 так как вершины нумеруются с 1

    // Обходим все смежные вершины
    for (int i = 0; i < n; i++) {
        if (matrix[current][i] == 1 && !visited[i]) {
            dfs_recursive(matrix, visited, n, i);
        }
    }
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

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

    // Создаем массив для отслеживания посещенных вершин
    bool* visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Выполняем обход в глубину, начиная с вершины 1 (индекс 0)
    cout << "Порядок обхода вершин: ";
    dfs_recursive(matrix, visited, n, 0);
    cout << endl;

    // Освобождаем память
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] visited;

    file.close();
    return 0;
}
