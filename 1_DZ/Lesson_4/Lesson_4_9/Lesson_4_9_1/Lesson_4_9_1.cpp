#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

void dfs_recursive(int** matrix, int n) {
    // Обход и вывод графа
    for (int i = 0; i < n; i++) {
        std::cout << (i + 1) << ": ";

        bool hasConnections = false;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
                std::cout << (j + 1) << " ";
                hasConnections = true;
            }
        }

        if (!hasConnections) {
            std::cout << "нет";
        }

        std::cout << std::endl;
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

    file.close();



    // Выполняем обход в глубину, начиная с вершины 1 (индекс 0)
    cout << "Текстовый вид орграфа  ";
    dfs_recursive(matrix, n);
    cout << endl;

    // Освобождаем память
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;


    return 0;
}

