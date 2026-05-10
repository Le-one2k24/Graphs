#ifndef SHORTEST_PATHS_H
#define SHORTEST_PATHS_H

#include "common_structures.h"
#include <climits>

const int INF = INT_MAX / 2;

void dijkstra_shortest(int** W, int N, int start, int* dist, int* parent, long long& iters);

void print_path_from_parents(int start, int end, int* parent);

#endif
