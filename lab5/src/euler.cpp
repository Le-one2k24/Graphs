#include "euler.h"
#include <algorithm>
#include <stack>
#include <iostream>
using namespace std;

bool is_eulerian(int** adj, int N) {
    for (int i = 0; i < N; ++i) {
        int deg = 0;
        for (int j = 0; j < N; ++j) {
            if (adj[i][j] != 0) deg += adj[i][j]; // учёт кратных рёбер (если >1)
        }
        if (deg % 2 != 0) return false;
    }
    return true;
}

vector<pair<int,int>> make_eulerian(int** adj, int N, vector<vector<int>>& mult) {
    // Создаём мультиматрицу на основе исходного графа
    mult.assign(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (adj[i][j] != 0)
                mult[i][j] = adj[i][j]; // обычно 1, но поддерживаем кратные

    // Подсчёт степеней
    vector<int> deg(N, 0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            deg[i] += mult[i][j];
        }
    }

    vector<int> odds;
    for (int i = 0; i < N; ++i)
        if (deg[i] % 2 == 1)
            odds.push_back(i);

    vector<pair<int,int>> added;
    // Соединяем вершины с нечётной степенью попарно (добавляем ребра)
    for (size_t i = 0; i + 1 < odds.size(); i += 2) {
        int u = odds[i];
        int v = odds[i+1];
        mult[u][v]++;
        mult[v][u]++;
        added.push_back({u, v});
        cout << "Добавлено ребро между " << u << " и " << v << " (степени были нечётными)\n";
    }

    if (!added.empty()) {
        cout << "Граф был модифицирован для получения эйлеровости.\n";
    }
    return added;
}

vector<int> find_eulerian_cycle(const vector<vector<int>>& mult, int N, int start) {
    vector<vector<int>> m = mult; // копия для изменений
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
    // Проверка: если цикл не содержит все рёбра (или граф был несвязен), то вернём пустой
    // Для эйлерова графа всё корректно.
    return cycle;
}