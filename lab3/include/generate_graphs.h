#ifndef GENERATE_GRAPHS_H
#define GENERATE_GRAPHS_H

#include "common_structures.h"

double generate_champernaun_continuous(double mu, double alpha);
int generate_random_degree_champernaun(double mu = 2.0, double alpha = 1.57);
int generate_random_degree();


int** generate_directed_by_degrees(int N);

#endif
