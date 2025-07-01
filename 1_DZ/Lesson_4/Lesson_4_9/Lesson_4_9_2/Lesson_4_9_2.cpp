#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

// Функция для сдвига элементов массива вправо и вставки в начало
void insertAtBeginning(int* order, int& size, int vertex) {
    // Сдвигаем все элементы на одну позицию вправо
    for (int i = size; i > 0; i--) {
        order[i] = order[i - 1];
    }
    // Вставляем новый элемент в начало
    order[0] = vertex;
    size++; // Увеличиваем размер массива
}

// Модифицированная функция DFS согласно псевдокоду
void dfs(int** adjacencyMatrix, int n, int vertex, bool* visited, int* order, int& orderSize) {
    visited[vertex] = true; // Помечаем вершину как посещенную

    // Обходим все смежные вершины
    for (int i = 0; i < n; i++) {
        // Если есть ребро от текущей вершины к вершине i и она не посещена
        if (adjacencyMatrix[vertex][i] == 1 && !visited[i]) {
            dfs(adjacencyMatrix, n, i, visited, order, orderSize);
        }
    }

    // КЛЮЧЕВОЕ ОТЛИЧИЕ: вставляем вершину в НАЧАЛО order
    // Это происходит ПОСЛЕ обработки всех смежных вершин
    insertAtBeginning(order, orderSize, vertex);
}

// Функция топологической сортировки согласно псевдокоду
void topologicalSort(int** adjacencyMatrix, int n) {
    bool* visited = new bool[n]; // Массив для отслеживания посещенных вершин
    int* order = new int[n]; // Массив для хранения топологического порядка
    int orderSize = 0; // Текущий размер массива order

    // Инициализируем массив посещенных вершин
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Запускаем DFS для всех непосещенных вершин
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs(adjacencyMatrix, n, i, visited, order, orderSize);
        }
    }

    // Выводим результат в прямом порядке (уже отсортирован)
    cout << "Топологический порядок вершин: ";
    for (int i = 0; i < orderSize; i++) {
        cout << order[i] + 1; // +1 так как вершины нумеруются с 1
        if (i < orderSize - 1) cout << " ";
    }
    cout << endl;

    // Освобождаем память
    delete[] visited;
    delete[] order;
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ifstream inputFile("input.txt"); // Открываем файл для чтения

    if (!inputFile.is_open()) {
        cout << "Ошибка открытия файла!" << endl;
        return 1;
    }

    int n; // Количество вершин
    inputFile >> n;

    // Создаем двумерный динамический массив для матрицы смежности
    int** adjacencyMatrix = new int* [n];
    for (int i = 0; i < n; i++) {
        adjacencyMatrix[i] = new int[n];
    }

    // Читаем матрицу смежности из файла
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inputFile >> adjacencyMatrix[i][j];
        }
    }

    inputFile.close(); // Закрываем файл

    // Выполняем топологическую сортировку
    topologicalSort(adjacencyMatrix, n);

    // Освобождаем память, выделенную под матрицу смежности
    for (int i = 0; i < n; i++) {
        delete[] adjacencyMatrix[i];
    }
    delete[] adjacencyMatrix;

    return 0;
}