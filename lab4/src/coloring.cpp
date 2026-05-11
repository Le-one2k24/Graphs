#include "coloring.h"
#include <vector>
#include <set>
#include <algorithm>
#include <limits>
#include <iostream>
using namespace std;

int* dsatur_coloring(int** adj, int N, int& colors_used) {
    colors_used = 0;
    if (!adj || N <= 0) return nullptr;

    vector<int> color(N, 0);
    vector<int> degree(N, 0);
    for (int i = 0; i < N; ++i) {
        int d = 0;
        for (int j = 0; j < N; ++j) if (adj[i][j] != 0) d++;
        degree[i] = d;
    }

    vector< set<int> > neigh_colors(N);

    auto select_vertex = [&](void)->int {
        int best = -1;
        int best_sat = -1;
        int best_deg = -1;
        for (int i = 0; i < N; ++i) {
            if (color[i] != 0) continue;
            int sat = (int)neigh_colors[i].size();
            if (sat > best_sat || (sat == best_sat && degree[i] > best_deg)) {
                best = i;
                best_sat = sat;
                best_deg = degree[i];
            }
        }
        return best;
    };

    int uncolored = N;
    while (uncolored > 0) {
        int v = select_vertex();
        if (v == -1) break;

        int c = 1;
        while (neigh_colors[v].count(c)) ++c;
        color[v] = c;
        if (c > colors_used) colors_used = c;

        for (int u = 0; u < N; ++u) {
            if (adj[v][u] != 0) {
                neigh_colors[u].insert(c);
            }
        }

        uncolored--;
    }

    int* res = new int[N];
    for (int i = 0; i < N; ++i) res[i] = color[i];
    return res;
}
