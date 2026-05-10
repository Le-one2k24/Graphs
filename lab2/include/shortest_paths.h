#ifndef SHORTEST_PATHS_H
#define SHORTEST_PATHS_H

#include "common_structures.h"
#include <climits>

const int INF = INT_MAX / 2;

// Классический алгоритм Дейкстры (O(N^2))
// W — весовая матрица (0 означает отсутствие ребра, кроме диагонали)
// start — стартовая вершина
// dist[i] — итоговые расстояния
// parent[i] — предок в кратчайшем пути
// iters — счётчик итераций
void dijkstra_shortest(int** W, int N, int start, int* dist, int* parent, long long& iters);

void print_path_from_parents(int start, int end, int* parent);

#endif
