#ifndef SHIMBELL_H
#define SHIMBELL_H

int** shimbell_multiply(int** A, int** B, int N, bool flag);
int** shimbell_all_paths(int** W, int N, int max_edges, bool find_max);
void print_matrix_with_inf(int** M, int N, const char* name);

#endif