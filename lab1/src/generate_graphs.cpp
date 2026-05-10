#include "generate_graphs.h"
#include "common_structures.h"
#include "shimbell.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

// Параметры распределения Чампернауна
static const double CHAMP_MU = 2.0;      // μ (математическое ожидание)
static const double CHAMP_ALPHA = 1.57;  // α > 0 (параметр масштаба)

// Генерация случайного числа по распределению Чампернауна
static double generate_champernaun_continuous() {
    double r;
    do {
        r = (rand() + 0.5) / (RAND_MAX + 1.0);
    } while (r <= 0.0 || r >= 1.0);
    return CHAMP_MU + (1.0 / CHAMP_ALPHA) * log(tan(M_PI * r / 2.0));
}

int generate_random_degree_champernaun() {
    double x = generate_champernaun_continuous();
    int deg = (int)round(x);
    if (deg < 0) deg = 0;
    return deg;
}

int generate_random_degree() {
    return generate_random_degree_champernaun();
}

int** generate_directed_by_degrees(int N) {
    int* out_degree = new int[N];

    int max_deg = 0;
    for (int i = 0; i < N; ++i) {
        out_degree[i] = generate_random_degree();
        if (out_degree[i] < 0) out_degree[i] = 0;
        if (out_degree[i] > max_deg) max_deg = out_degree[i];
    }

    if (max_deg == 0) {
        for (int i = 0; i < N; ++i) out_degree[i] = 1;
        max_deg = 1;
    }

    double k = double(N - 1) / double(max_deg);

    for (int i = 0; i < N; ++i) {
        out_degree[i] = int(round(out_degree[i] * k));
    }

    cout << "Исходящие степени (после нормировки): ";
    for (int i = 0; i < N; ++i) cout << out_degree[i] << " ";
    cout << endl;

    vector<int> order(N);
    for (int i = 0; i < N; i++) order[i] = i;
    random_shuffle(order.begin(), order.end());

    vector<int> position(N);
    for (int i = 0; i < N; i++) {
        position[order[i]] = i;
    }

    int** adj = new int*[N];
    for (int i = 0; i < N; ++i) {
        adj[i] = new int[N];
        for (int j = 0; j < N; ++j) adj[i][j] = 0;
    }

    for (int i = 0; i < N; ++i) {
        int deg = out_degree[i];
        vector<int> candidates;

        for (int j = 0; j < N; ++j) {
            if (j != i && position[j] > position[i]) {
                candidates.push_back(j);
            }
        }

        if (candidates.empty()) continue;

        random_shuffle(candidates.begin(), candidates.end());
        int edges_to_add = min(deg, (int)candidates.size());

        for (int k2 = 0; k2 < edges_to_add; ++k2) {
            adj[i][candidates[k2]] = 1;
        }
    }

    delete[] out_degree;
    return adj;
}