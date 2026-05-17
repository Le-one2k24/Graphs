#include "routes.h"
#include <iostream>

static void dfs_count(int v, int end, int** matrix, int N, bool oriented, bool* visited, int& count) {
    if (v == end) {
        count++;
        return;
    }
    visited[v] = true;
    for (int u = 0; u < N; ++u) {
        if (u == v) continue;
        if (!visited[u]) {
            if (oriented) {
                if (matrix[v][u] == 1)
                    dfs_count(u, end, matrix, N, oriented, visited, count);
            } else {
                if (matrix[v][u] == 1 || matrix[u][v] == 1)
                    dfs_count(u, end, matrix, N, oriented, visited, count);
            }
        }
    }
    visited[v] = false;
}

int count_routes_matrix(int** matrix, int N, bool oriented, int start, int end) {
    if (start < 0 || start >= N || end < 0 || end >= N)
        return -1;

    bool* visited = new bool[N];
    for (int i = 0; i < N; ++i)
        visited[i] = false;

    int count = 0;
    dfs_count(start, end, matrix, N, oriented, visited, count);

    delete[] visited;
    return count;
}