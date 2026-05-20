#include "euler.h"
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

bool is_eulerian(int** adj, int N) {
    for (int i = 0; i < N; ++i) {
        int deg = 0;
        for (int j = 0; j < N; ++j) deg += (adj[i][j] != 0);
        if (deg % 2 != 0) return false;
    }
    return true;
}

void make_eulerian(int** adj, int N, vector<vector<int>>& mult) {
    mult.assign(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            mult[i][j] = adj[i][j];

    vector<int> deg(N, 0);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (adj[i][j] != 0) deg[i]++;

    vector<int> odds;
    for (int i = 0; i < N; ++i)
        if (deg[i] % 2 == 1)
            odds.push_back(i);

    if (odds.empty()) {
        cout << "Граф уже эйлеров.\n";
        return;
    }

    cout << "Вершины с нечётной степенью: ";
    for (int x : odds) cout << x << " ";
    cout << endl;

    for (size_t i = 0; i < odds.size(); i += 2) {
        int u = odds[i];
        int v = odds[i+1];
        if (mult[u][v] > 0) {
            mult[u][v]--;
            mult[v][u]--;
            cout << "Удалено ребро " << u << " - " << v << endl;
        } else {
            mult[u][v]++;
            mult[v][u]++;
            cout << "Добавлено ребро " << u << " - " << v << endl;
        }
    }
}

vector<int> find_eulerian_cycle(const vector<vector<int>>& mult, int N, int start) {
    vector<vector<int>> m = mult;
    vector<int> cycle;
    stack<int> st;
    st.push(start);

    while (!st.empty()) {
        int v = st.top();
        bool found = false;
        for (int u = 0; u < N; ++u) {
            if (m[v][u] > 0) {
                m[v][u]--;
                m[u][v]--;
                st.push(u);
                found = true;
                break;
            }
        }
        if (!found) {
            cycle.push_back(v);
            st.pop();
        }
    }

    reverse(cycle.begin(), cycle.end());
    return cycle;
}