#include <iostream>
#include <fstream>
#include <queue> // Для работы с очередью FIFO
#include <Windows.h>

using namespace std;

void bfs(int** matrix, bool* visited, int n, int start) {
    queue<int> q;  // Создаем очередь для хранения индексов вершин
    q.push(start); // Добавляем стартовую вершину в очередь
    visited[start] = true; // Помечаем стартовую вершину как посещенную

    cout << "Порядок обхода вершин: ";

    while (!q.empty()) { // Пока очередь не пуста
        int current = q.front(); // Берем первый элемент из очереди
        q.pop(); // Удаляем этот элемент из очереди

        cout << current + 1 << " "; // Выводим номер вершины (+1 для пользователя)

        // Проверяем все возможные смежные вершины
        for (int i = 0; i < n; i++) {
            // Если есть связь И вершина не посещена
            if (matrix[current][i] == 1 && !visited[i]) {
                visited[i] = true; // Помечаем как посещенную
                q.push(i); // Добавляем в очередь для дальнейшего обхода
            }
        }
    }
    cout << endl;
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

    // Запрашиваем у пользователя начальную вершину
    int startVertex;
    do {
        cout << "Введите начальную вершину для обхода (от 1 до " << n << "): ";
        cin >> startVertex;

        if (startVertex < 1 || startVertex > n) {
            cout << "Ошибка: индекс вершины должен быть от 1 до " << n << endl;
        }
    } while (startVertex < 1 || startVertex > n);

    // Создаем массив для отслеживания посещенных вершин
    bool* visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Выполняем обход в ширину
    bfs(matrix, visited, n, startVertex - 1); // -1 так как массив индексируется с 0

    // Проверяем, есть ли непосещенные вершины (для несвязного графа)
    bool hasUnvisited = false;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            hasUnvisited = true;
            break;
        }
    }

    if (hasUnvisited) {
        cout << "Внимание: граф несвязный, некоторые вершины недостижимы из выбранной начальной вершины." << endl;
        cout << "Недостижимые вершины: ";
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                cout << i + 1 << " ";
            }
        }
        cout << endl;
    }

    // Освобождаем память
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] visited;

    return 0;
}
//   Принцип FIFO (First In, First Out):
// Элементы обрабатываются в том порядке, в котором были добавлены
// Это обеспечивает обход "по уровням" - сначала все соседи текущей вершины, потом их соседи