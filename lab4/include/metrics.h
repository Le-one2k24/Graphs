#ifndef METRICS_H
#define METRICS_H

#include "common_structures.h"

int* all_eccentricites(Graph* graph);
void find_center(int* ecc, int N);
void find_diametr(int* ecc, int N);

int* eccentricites_oriented(int** matrix, int N);
void print_eccentricities_oriented(int* ecc, int N);
void find_center_oriented(int* ecc, int N);
void find_diametr_oriented(int* ecc, int N);

#endif
