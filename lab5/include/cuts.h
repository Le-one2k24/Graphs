#ifndef CUTS_H
#define CUTS_H

#include <vector>
#include <utility>

using Cut = std::vector<std::pair<int,int>>;

std::vector<Cut> get_fundamental_cuts(int** adj, int** mst, int N);

void print_cut(const Cut& cut, const char* name);

Cut symmetric_difference(const Cut& a, const Cut& b);

Cut symmetric_difference_multiple(const std::vector<Cut>& cuts);

#endif