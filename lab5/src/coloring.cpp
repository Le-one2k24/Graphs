#include "coloring.h"
#include <vector>
#include <algorithm>

int* dsatur_coloring(int** adj, int N, int& colors_used) {

    std::vector<int> degree(N, 0);
    for (int i = 0; i < N; ++i) {
        int deg = 0;
        for (int j = 0; j < N; ++j) {
            if (adj[i][j] != 0) ++deg;
        }
        degree[i] = deg;
    }

    std::vector<int> vertices(N);
    for (int i = 0; i < N; ++i) vertices[i] = i;
    std::sort(vertices.begin(), vertices.end(),
              [&](int a, int b) { return degree[a] > degree[b]; });

    std::vector<int> color(N, 0);
    int uncolored = N;
    int c = 1;
    colors_used = 0;

    while (uncolored > 0) {
        for (int idx = 0; idx < N; ++idx) {
            int v = vertices[idx];
            if (color[v] != 0) continue;

            bool conflict = false;
            for (int u = 0; u < N; ++u) {
                if (adj[v][u] != 0 && color[u] == c) {
                    conflict = true;
                    break;
                }
            }
            if (!conflict) {
                color[v] = c;
                --uncolored;
            }
        }
        if (c > colors_used) colors_used = c;
        ++c;
    }

    int* result = new int[N];
    for (int i = 0; i < N; ++i) result[i] = color[i];
    return result;
}