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

void make_eulerian(int** adj, int N, vector<vector<int>>& eul) {
    eul.assign(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            eul[i][j] = adj[i][j];

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

    sort(odds.begin(), odds.end(), [&](int a, int b) {
        return deg[a] < deg[b];
    });

    cout << "Вершины с нечётной степенью (отсортированы по степени): ";
    for (int x : odds) cout << x << " ";
    cout << endl;

    vector<bool> used(odds.size(), false);
    int remaining = odds.size();

    while (remaining > 0) {
        int i = 0;
        while (i < (int)odds.size() && used[i]) ++i;
        if (i == (int)odds.size()) break;

        int u = odds[i];
        int j;
        for (int k = i + 1; k < (int)odds.size(); ++k) {
            if (used[k]) continue;
            int v = odds[k];
            if (deg[u] == 1 || deg[v] == 1) {
                if (eul[u][v] == 0) {
                    j = k;
                    break;
                }
            } else {
                j = k;
                break;
            }
        }

        int v = odds[j];
        if (eul[u][v] > 0) {
            eul[u][v]--;
            eul[v][u]--;
            cout << "Удалено ребро " << u << " - " << v << endl;
        } else {
            eul[u][v]++;
            eul[v][u]++;
            cout << "Добавлено ребро " << u << " - " << v << endl;
        }
        used[i] = used[j] = true;
        remaining -= 2;
    }
}

vector<int> find_eulerian_cycle(const vector<vector<int>>& eul, int N, int start) {
    vector<vector<int>> m = eul;
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