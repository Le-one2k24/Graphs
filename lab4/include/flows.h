#ifndef FLOWS_H
#define FLOWS_H

#include "common_structures.h"
#include <vector>

 // Генерация матриц пропускных способностей и стоимостей
 // на основе текущего ориентированного графа (матрицы смежности).
 // Пропускные способности и стоимости задаются случайно (положительные целые).
void generate_capacity_and_cost_for_current_graph();

// Максимальный поток (алгоритм Эдмондса-Карпа, O(V * E^2))
// capacity — матрица пропускных способностей (0 — нет дуги)
// Не изменяет исходную матрицу capacity.
int max_flow_edmonds_karp(int** capacity, int N, int s, int t);


long long min_cost_flow_with_dijkstra(
    int** capacity,
    int** cost,
    int N,
    int s,
    int t,
    int required_flow,
    int& achieved_flow,
    int** flow
);


void decompose_flow_into_paths(int** flow, int N, int s, int t,
                               std::vector<std::vector<int>>& paths,
                               std::vector<int>& path_flow,
                               std::vector<long long>& path_cost,
                               int** cost_matrix);

#endif
