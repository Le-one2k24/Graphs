#ifndef COMMON_STRUCTURES_H
#define COMMON_STRUCTURES_H

#include <iostream>
using namespace std;

struct Graph {
    int N;
    int** list;
    int* size;

    Graph(int n);
    ~Graph();
    void edge(int a, int b);
    void print();
};

extern int** current_matrix;
extern int current_matrix_N;
extern bool current_matrix_oriented;

extern int** current_weight_matrix;
extern int current_weight_matrix_N;
extern bool weight_matrix_exist;

void print_smezhn_matrix(int** matrix, int N);
void delete_matrix(int** M, int N);
void set_current_matrix(int** matrix, int N, bool oriented);
int** get_current_matrix(int& N, bool& oriented);

void set_weight_matrix(int** matrix, int N);
int** get_weight_matrix(int& N);
bool is_weight_matrix_exist();
void clear_weight_matrix();

// Печать весовой матрицы
void print_weight_matrix(int** weight, int N);

#endif
