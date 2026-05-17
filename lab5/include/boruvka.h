#ifndef BORUVKA_H
#define BORUVKA_H


int** boruvka_mst(int** adj, int** weight, int N, int& total_weight);

int** get_last_mst(int& N);

void clear_last_mst();

#endif
