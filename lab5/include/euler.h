#ifndef EULER_H
#define EULER_H

#include <vector>

bool is_eulerian(int** adj, int N);

void make_eulerian(int** adj, int N, std::vector<std::vector<int>>& eul);

std::vector<int> find_eulerian_cycle(const std::vector<std::vector<int>>& eul, int N, int start = 0);

#endif