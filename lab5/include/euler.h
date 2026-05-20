#ifndef EULER_H
#define EULER_H

#include <vector>

bool is_eulerian(int** adj, int N);

void make_eulerian(int** adj, int N, std::vector<std::vector<int>>& mult);

std::vector<int> find_eulerian_cycle(const std::vector<std::vector<int>>& mult, int N, int start = 0);

#endif