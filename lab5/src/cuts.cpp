#include "cuts.h"
#include <iostream>
#include <set>
#include <queue>
#include <algorithm>
using namespace std;

static void get_component(int** mst, int N, int start, int forbidden_u, int forbidden_v, vector<bool>& comp) {
    comp.assign(N, false);
    queue<int> q;
    q.push(start);
    comp[start] = true;
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int u = 0; u < N; ++u) {
            if (mst[v][u] && !comp[u]) {
                if ((v == forbidden_u && u == forbidden_v) || (v == forbidden_v && u == forbidden_u))
                    continue;
                comp[u] = true;
                q.push(u);
            }
        }
    }
}

Cut get_fundamental_cut_for_edge(int** adj, int** mst, int N, int u_edge, int v_edge) {
    vector<bool> comp1;
    get_component(mst, N, u_edge, u_edge, v_edge, comp1);
    Cut cut;
    if (u_edge < v_edge) cut.push_back({u_edge, v_edge});
    else cut.push_back({v_edge, u_edge});

    for (int i = 0; i < N; ++i) {
        for (int j = i+1; j < N; ++j) {
            if (adj[i][j] == 0) continue;
            if (comp1[i] != comp1[j]) {
                cut.push_back({i, j});
            }
        }
    }
    sort(cut.begin(), cut.end());
    cut.erase(unique(cut.begin(), cut.end()), cut.end());
    return cut;
}

vector<Cut> get_fundamental_cuts(int** adj, int** mst, int N) {
    vector<Cut> result;
    for (int i = 0; i < N; ++i) {
        for (int j = i+1; j < N; ++j) {
            if (mst[i][j] == 1) {
                Cut c = get_fundamental_cut_for_edge(adj, mst, N, i, j);
                result.push_back(c);
            }
        }
    }
    return result;
}

void print_cut(const Cut& cut, const char* name) {
    cout << name << ": { ";
    for (size_t i = 0; i < cut.size(); ++i) {
        cout << "(" << cut[i].first << "," << cut[i].second << ")";
        if (i+1 < cut.size()) cout << ", ";
    }
    cout << " }\n";
}

Cut symmetric_difference(const Cut& a, const Cut& b) {
    set<pair<int,int>> sa(a.begin(), a.end());
    set<pair<int,int>> sb(b.begin(), b.end());
    Cut res;
    for (auto& p : sa) {
        if (sb.find(p) == sb.end()) res.push_back(p);
    }
    for (auto& p : sb) {
        if (sa.find(p) == sa.end()) res.push_back(p);
    }
    sort(res.begin(), res.end());
    return res;
}

Cut symmetric_difference_multiple(const vector<Cut>& cuts) {
    if (cuts.empty()) return Cut();
    Cut res = cuts[0];
    for (size_t i = 1; i < cuts.size(); ++i) {
        res = symmetric_difference(res, cuts[i]);
    }
    return res;
}