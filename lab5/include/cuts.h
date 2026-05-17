#ifndef CUTS_H
#define CUTS_H

#include <vector>
#include <utility>

// Представление разреза как набор рёбер (u, v) с u < v
using Cut = std::vector<std::pair<int,int>>;

// Получить фундаментальную систему разрезов для остова mst (матрица смежности 0/1)
// в исходном графе adj (неориентированный, матрица смежности 0/1 или с кратностями).
// Возвращает вектор разрезов (по одному для каждого ребра mst).
std::vector<Cut> get_fundamental_cuts(int** adj, int** mst, int N);

// Вывести разрез на экран (список рёбер)
void print_cut(const Cut& cut, const char* name);

// Симметрическая разность двух разрезов (объединение минус пересечение)
Cut symmetric_difference(const Cut& a, const Cut& b);

// Симметрическая разность нескольких разрезов (последовательное применение)
Cut symmetric_difference_multiple(const std::vector<Cut>& cuts);

#endif