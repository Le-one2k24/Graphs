#ifndef GENERATE_GRAPHS_H
#define GENERATE_GRAPHS_H

#include "common_structures.h"

// генерация степеней по распределению Чампернауна
int generate_random_degree_champernaun();
int generate_random_degree();

// генерация ориентированного связного ациклического графа по степеням
int** generate_directed_by_degrees(int N);

#endif