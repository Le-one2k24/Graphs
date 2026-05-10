#include "metrics.h"
#include <iostream>
using namespace std;

int* distance(Graph* graph, int start){
    int N = graph->N;
    int* dist = new int[N];
    for (int i = 0; i < N; ++i) dist[i] = -1;
    dist[start] = 0;
    int* q = new int[N];
    int head = 0, tail = 0;
    q[tail++] = start;
    while (head < tail){
        int v = q[head++];
        for (int i = 0; i < graph->size[v]; ++i){
            int u = graph->list[v][i];
            if (dist[u] == -1){ dist[u] = dist[v] + 1; q[tail++] = u; }
        }
    }
    delete[] q;
    return dist;
}

int* all_eccentricites(Graph* graph){
    int N = graph->N;
    int* ecc = new int[N];
    for (int i = 0; i < N; ++i){
        int* d = distance(graph, i);
        int mx = 0;
        for (int j = 0; j < N; ++j) if (d[j] > mx) mx = d[j];
        ecc[i] = mx;
        delete[] d;
    }
    return ecc;
}

void find_center(int* ecc, int N){
    int radius = ecc[0];
    for (int i = 1; i < N; ++i) if (ecc[i] < radius) radius = ecc[i];
    cout << "Радиус графа = " << radius << endl;
    cout << "Центр графа: { ";
    for (int i = 0; i < N; ++i) if (ecc[i] == radius) cout << i << " ";
    cout << "}\n";
}

void find_diametr(int* ecc, int N){
    int diam = ecc[0];
    for (int i = 1; i < N; ++i) if (ecc[i] > diam) diam = ecc[i];
    cout << "Диаметр графа = " << diam << endl;
    cout << "Диаметральные вершины: { ";
    for (int i = 0; i < N; ++i) if (ecc[i] == diam) cout << i << " ";
    cout << "}\n";
}

int* distance_oriented(int** matrix, int N, int start){
    int* dist = new int[N];
    for (int i = 0; i < N; ++i) dist[i] = -1;
    dist[start] = 0;
    int* q = new int[N];
    int head = 0, tail = 0;
    q[tail++] = start;
    while (head < tail){
        int v = q[head++];
        for (int u = 0; u < N; ++u){
            if (matrix[v][u] == 1 && dist[u] == -1){
                dist[u] = dist[v] + 1;
                q[tail++] = u;
            }
        }
    }
    delete[] q;
    return dist;
}

int* eccentricites_oriented(int** matrix, int N){
    int* ecc = new int[N];
    for (int i = 0; i < N; ++i){
        int* d = distance_oriented(matrix, N, i);
        int mx = -1;
        for (int j = 0; j < N; ++j) if (d[j] > mx) mx = d[j];
        ecc[i] = (mx == -1) ? 0 : mx;
        delete[] d;
    }
    return ecc;
}

void print_eccentricities_oriented(int* ecc, int N){
    cout << "Эксцентриситеты:\n";
    for (int i = 0; i < N; ++i) cout << "e(" << i << ") = " << ecc[i] << "\n";
}

void find_center_oriented(int* ecc, int N){ find_center(ecc, N); }
void find_diametr_oriented(int* ecc, int N){ find_diametr(ecc, N); }
