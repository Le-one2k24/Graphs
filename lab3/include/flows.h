#ifndef FLOWS_H
#define FLOWS_H

#include "common_structures.h"

// Генерация матриц пропускных способностей и стоимостей
// на основе текущего ориентированного графа (матрицы смежности).
// Пропускные способности и стоимости задаются случайно (положительные целые).
void generate_capacity_and_cost_for_current_graph();

// Максимальный поток (алгоритм Эдмондса-Карпа, O(V * E^2))
// capacity — матрица пропускных способностей (0 — нет дуги)
// Не изменяет исходную матрицу capacity.
int max_flow_edmonds_karp(int** capacity, int N, int s, int t);

// Поток минимальной стоимости заданного объёма required_flow.
// Используется метод последовательных кратчайших путей с потенциалами,
// внутри — Дейкстра по приведённым стоимостям.
// capacity и cost — исходные матрицы (не изменяются).
// Возвращает суммарную стоимость, achieved_flow — реально достигнутый поток.
long long min_cost_flow_with_dijkstra(
    int** capacity,
    int** cost,
    int N,
    int s,
    int t,
    int required_flow,
    int& achieved_flow
);

#endif
