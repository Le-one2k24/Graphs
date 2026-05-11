#include "prufer.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

bool encode_prufer_with_weights(
    int** adj,
    int** weight,
    int N,
    vector<int>& code_nodes,
    vector<int>& code_weights,
    int& last_u,
    int& last_v
) {
    if (!adj || !weight || N <= 0) return false;

    int** tmp_adj = new int* [N];
    for (int i = 0; i < N; ++i) {
        tmp_adj[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            tmp_adj[i][j] = adj[i][j];
        }
    }

    vector<int> deg(N, 0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (tmp_adj[i][j] == 1) deg[i]++;
        }
    }

    priority_queue<int, vector<int>, greater<int>> leaves;
    for (int i = 0; i < N; ++i) {
        if (deg[i] == 1) leaves.push(i);
    }

    code_nodes.clear();
    code_weights.clear();
    code_nodes.reserve((N > 2) ? (N - 2) : 0);
    code_weights.reserve((N > 1) ? (N - 1) : 0);

    for (int step = 0; step < N - 2; ++step) {
        int v = -1;
        while (!leaves.empty()) {
            int x = leaves.top();
            leaves.pop();
            if (deg[x] == 1) { v = x; break; }
        }
        if (v == -1) {
            for (int i = 0; i < N; ++i) delete[] tmp_adj[i];
            delete[] tmp_adj;
            return false;
        }

        int u = -1;
        for (int j = 0; j < N; ++j) {
            if (tmp_adj[v][j] == 1) {
                u = j;
                break;
            }
        }
        if (u == -1) {
            for (int i = 0; i < N; ++i) delete[] tmp_adj[i];
            delete[] tmp_adj;
            return false;
        }

        code_nodes.push_back(u);
        code_weights.push_back(weight[v][u]);

        tmp_adj[v][u] = tmp_adj[u][v] = 0;
        deg[v]--;
        deg[u]--;

        if (deg[u] == 1) {
            leaves.push(u);
        }
    }

    last_u = -1;
    last_v = -1;
    for (int i = 0; i < N; ++i) {
        if (deg[i] == 1) {
            if (last_u == -1) last_u = i;
            else last_v = i;
        }
    }
    if (last_u == -1 || last_v == -1) {
        for (int i = 0; i < N; ++i) delete[] tmp_adj[i];
        delete[] tmp_adj;
        return false;
    }

    code_weights.push_back(weight[last_u][last_v]);

    for (int i = 0; i < N; ++i) delete[] tmp_adj[i];
    delete[] tmp_adj;
    return true;
}

int** decode_prufer_with_weights(
    const vector<int>& code_nodes,
    const vector<int>& code_weights,
    int N,
    int**& decoded_weights
) {
    decoded_weights = nullptr;
    if (N <= 0) return nullptr;
    if ((int)code_nodes.size() != N - 2) return nullptr;
    if ((int)code_weights.size() != N - 1) return nullptr;

    int** adj = new int* [N];
    for (int i = 0; i < N; ++i) {
        adj[i] = new int[N];
        for (int j = 0; j < N; ++j) adj[i][j] = 0;
    }

    decoded_weights = new int* [N];
    for (int i = 0; i < N; ++i) {
        decoded_weights[i] = new int[N];
        for (int j = 0; j < N; ++j) decoded_weights[i][j] = 0;
    }

    vector<int> deg(N, 1);
    for (int x : code_nodes) {
        if (x >= 0 && x < N) deg[x]++;
    }

    priority_queue<int, vector<int>, greater<int>> leaves;
    for (int i = 0; i < N; ++i) {
        if (deg[i] == 1) leaves.push(i);
    }

    for (int i = 0; i < N - 2; ++i) {
        int u = code_nodes[i];
        int w = code_weights[i];

        int v = -1;
        while (!leaves.empty()) {
            int x = leaves.top();
            leaves.pop();
            if (deg[x] == 1) { v = x; break; }
        }
        if (v == -1) {
            for (int r = 0; r < N; ++r) delete[] adj[r];
            delete[] adj;
            for (int r = 0; r < N; ++r) delete[] decoded_weights[r];
            delete[] decoded_weights;
            decoded_weights = nullptr;
            return nullptr;
        }

        adj[v][u] = adj[u][v] = 1;
        decoded_weights[v][u] = decoded_weights[u][v] = w;

        deg[v]--;
        deg[u]--;

        if (deg[u] == 1) {
            leaves.push(u);
        }
    }

    int a = -1, b = -1;
    for (int i = 0; i < N; ++i) {
        if (deg[i] == 1) {
            if (a == -1) a = i;
            else b = i;
        }
    }
    if (a != -1 && b != -1) {
        int last_weight = code_weights.back();
        adj[a][b] = adj[b][a] = 1;
        decoded_weights[a][b] = decoded_weights[b][a] = last_weight;
    } else {
        for (int r = 0; r < N; ++r) delete[] adj[r];
        delete[] adj;
        for (int r = 0; r < N; ++r) delete[] decoded_weights[r];
        delete[] decoded_weights;
        decoded_weights = nullptr;
        return nullptr;
    }

    return adj;
}