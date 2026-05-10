#include "shortest_paths.h"
#include <iostream>
using namespace std;

static bool has_negative_weights(int** W, int N) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (i != j && W[i][j] < 0)
                return true;
    return false;
}

bool dijkstra_shortest(int** W, int N, int start, int* dist, int* parent, long long& iters)
{
    iters = 0;

    if (has_negative_weights(W, N)) {
        // Заполняем выходные массивы "безопасными" значениями и возвращаем false
        for (int i = 0; i < N; ++i) {
            dist[i] = INF;
            parent[i] = -1;
        }
        cout << "Ошибка: алгоритм Дейкстры не работает с отрицательными весами.\n";
        return false;
    }

    bool* used = new bool[N];
    for (int i = 0; i < N; ++i) {
        dist[i] = INF;
        parent[i] = -1;
        used[i] = false;
    }
    dist[start] = 0;

    for (int step = 0; step < N; ++step) {
        int v = -1;

        for (int i = 0; i < N; ++i) {
            ++iters;
            if (!used[i] && (v == -1 || dist[i] < dist[v]))
                v = i;
        }

        if (v == -1 || dist[v] == INF) break;

        used[v] = true;

        for (int u = 0; u < N; ++u) {
            ++iters;
            if (W[v][u] == 0) continue;
            if (dist[v] + W[v][u] < dist[u]) {
                dist[u] = dist[v] + W[v][u];
                parent[u] = v;
            }
        }
    }

    delete[] used;
    return true;
}

void print_path_from_parents(int start, int end, int* parent)
{
    if (start == end) {
        cout << start;
        return;
    }

    int cur = end;
    int stackSize = 0;
    int* stack = new int[10000];

    while (cur != -1) {
        stack[stackSize++] = cur;
        if (cur == start) break;
        cur = parent[cur];
    }

    if (stackSize == 0 || stack[stackSize - 1] != start) {
        cout << "пути нет";
        delete[] stack;
        return;
    }

    for (int i = stackSize - 1; i >= 0; --i) {
        cout << stack[i];
        if (i > 0) cout << " -> ";
    }

    delete[] stack;
}
