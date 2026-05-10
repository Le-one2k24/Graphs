#include "articulation.h"
#include <iostream>
using namespace std;

static void ap_dfs(int v,
                   int parent,
                   int** matrix,
                   int N,
                   bool* visited,
                   int* disc,
                   int* low,
                   int& timer,
                   bool* isAP)
{
    visited[v] = true;
    disc[v] = low[v] = ++timer;
    int children = 0;

    for (int u = 0; u < N; ++u) {
        if (u == v) continue;
        if (!(matrix[v][u] == 1 || matrix[u][v] == 1)) continue;

        if (!visited[u]) {
            ++children;
            ap_dfs(u, v, matrix, N, visited, disc, low, timer, isAP);
            low[v] = (low[v] < low[u]) ? low[v] : low[u];

            if (parent != -1 && low[u] >= disc[v]) {
                isAP[v] = true;
            }
        } else if (u != parent) {
            if (disc[u] < low[v]) low[v] = disc[u];
        }
    }

    if (parent == -1 && children > 1) {
        isAP[v] = true;
    }
}

void find_articulation_points_matrix(int** matrix, int N)
{
    if (!matrix || N <= 0) {
        cout << "Граф пуст или некорректен\n";
        return;
    }

    bool* visited = new bool[N];
    int* disc = new int[N];
    int* low = new int[N];
    bool* isAP = new bool[N];

    for (int i = 0; i < N; ++i) {
        visited[i] = false;
        disc[i] = 0;
        low[i] = 0;
        isAP[i] = false;
    }

    int timer = 0;
    
    ap_dfs(0, -1, matrix, N, visited, disc, low, timer, isAP);

    cout << "Точки сочленения: ";
    bool any = false;
    for (int i = 0; i < N; ++i) {
        if (isAP[i]) {
            cout << i << " ";
            any = true;
        }
    }
    if (!any) {
        cout << "(нет)";
    }
    cout << "\n";

    delete[] visited;
    delete[] disc;
    delete[] low;
    delete[] isAP;
}
