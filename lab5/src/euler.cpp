#include "euler.h"
#include <iostream>
#include <stack>
#include <queue>
#include <algorithm>
#include <set>
#include <map>
#include <cstring>

using namespace std;

bool is_eulerian(int** adj, int N) {
    for (int i = 0; i < N; ++i) {
        int deg = 0;
        for (int j = 0; j < N; ++j) deg += (adj[i][j] != 0);
        if (deg % 2 != 0) return false;
    }
    return true;
}

// Построить матрицу дополнительного графа (1 если ребра нет и i!=j)
static vector<vector<int>> build_auxiliary(int** adj, int N) {
    vector<vector<int>> aux(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (i != j && adj[i][j] == 0)
                aux[i][j] = 1;
    return aux;
}

// Найти все простые пути между s и t в графе aux (BFS с запоминанием путей)
static vector<vector<int>> find_all_paths(const vector<vector<int>>& aux, int s, int t) {
    vector<vector<int>> paths;
    queue<vector<int>> q;
    vector<int> start = {s};
    q.push(start);
    while (!q.empty()) {
        vector<int> path = q.front(); q.pop();
        int last = path.back();
        if (last == t) {
            paths.push_back(path);
            continue;
        }
        for (int u = 0; u < (int)aux.size(); ++u) {
            if (aux[last][u] && find(path.begin(), path.end(), u) == path.end()) {
                vector<int> newpath = path;
                newpath.push_back(u);
                q.push(newpath);
            }
        }
    }
    return paths;
}

// Рекурсивный перебор пар нечётных вершин
// odd_vertices: список вершин с нечётной степенью
// used: какие вершины уже обработаны
// current_edges: множество добавленных рёбер (для проверки конфликтов)
// adj_original: исходная матрица (0/1, без кратностей)
// best_edges: выходной вектор добавленных рёбер (если найдено решение)
// Возвращает true, если удалось разбить все пары
static bool backtrack_pairing(const vector<int>& odd_vertices,
                              vector<bool>& used,
                              set<pair<int,int>>& current_edges,
                              int** adj_original,
                              int N,
                              vector<pair<int,int>>& best_edges) {
    int n = odd_vertices.size();
    // Найти первую неиспользованную вершину
    int first = -1;
    for (int i = 0; i < n; ++i) {
        if (!used[i]) { first = i; break; }
    }
    if (first == -1) {
        // Все вершины разбиты на пары
        best_edges.assign(current_edges.begin(), current_edges.end());
        return true;
    }

    // Перебираем вторую вершину
    for (int j = first + 1; j < n; ++j) {
        if (used[j]) continue;
        int u = odd_vertices[first];
        int v = odd_vertices[j];

        // Если уже есть прямое ребро в исходном графе, то ищем обходной путь
        // В противном случае, можно добавить прямое ребро, если его нет в current_edges
        if (adj_original[u][v] == 0) {
            // Проверяем, не добавлено ли уже это ребро
            if (current_edges.find({u, v}) == current_edges.end() &&
                current_edges.find({v, u}) == current_edges.end()) {
                // Добавляем прямое ребро
                current_edges.insert({u, v});
                used[first] = used[j] = true;
                if (backtrack_pairing(odd_vertices, used, current_edges, adj_original, N, best_edges))
                    return true;
                used[first] = used[j] = false;
                current_edges.erase({u, v});
            }
        }

        // Ищем все пути в дополнительном графе между u и v
        vector<vector<int>> aux = build_auxiliary(adj_original, N);
        // Удаляем из aux рёбра, которые уже добавлены в current_edges (чтобы не создавать кратных)
        for (auto& e : current_edges) {
            aux[e.first][e.second] = 0;
            aux[e.second][e.first] = 0;
        }
        vector<vector<int>> paths = find_all_paths(aux, u, v);
        // Перебираем пути (ограничим длину пути, например, не более N)
        for (const auto& path : paths) {
            if (path.size() < 2) continue;
            // Проверим, что все рёбра пути не конфликтуют с уже добавленными
            bool conflict = false;
            for (size_t k = 0; k + 1 < path.size(); ++k) {
                int a = path[k], b = path[k+1];
                if (current_edges.find({a, b}) != current_edges.end() ||
                    current_edges.find({b, a}) != current_edges.end()) {
                    conflict = true;
                    break;
                }
            }
            if (conflict) continue;
            // Добавляем все рёбра пути
            vector<pair<int,int>> added;
            for (size_t k = 0; k + 1 < path.size(); ++k) {
                int a = path[k], b = path[k+1];
                added.push_back({a, b});
                current_edges.insert({a, b});
            }
            used[first] = used[j] = true;
            if (backtrack_pairing(odd_vertices, used, current_edges, adj_original, N, best_edges))
                return true;
            used[first] = used[j] = false;
            for (auto& e : added) current_edges.erase(e);
        }
    }
    return false;
}

vector<pair<int,int>> make_eulerian(int** adj, int N, vector<vector<int>>& mult) {
    // Копируем исходный граф в mult (матрица кратностей)
    mult.assign(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (adj[i][j] != 0)
                mult[i][j] = adj[i][j]; // обычно 1, но поддерживаем кратные

    // Подсчёт степеней в исходном графе
    vector<int> deg(N, 0);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            deg[i] += (adj[i][j] != 0);

    vector<int> odds;
    for (int i = 0; i < N; ++i)
        if (deg[i] % 2 == 1)
            odds.push_back(i);

    if (odds.empty()) {
        cout << "Граф уже эйлеров.\n";
        return {};
    }

    cout << "Вершины с нечётной степенью: ";
    for (int x : odds) cout << x << " ";
    cout << endl;

    vector<bool> used(odds.size(), false);
    set<pair<int,int>> chosen_edges;
    vector<pair<int,int>> best_edges;

    bool success = backtrack_pairing(odds, used, chosen_edges, adj, N, best_edges);

    if (!success) {
        cout << "Невозможно добавить рёбра между вершинами с нечётной степенью, не создавая кратных рёбер.\n";
        cout << "Граф нельзя сделать эйлеровым без кратных рёбер.\n";
        return {};
    }

    cout << "Добавленные рёбра (в порядке обработки пар):\n";
    for (size_t i = 0; i < best_edges.size(); ++i) {
        int u = best_edges[i].first, v = best_edges[i].second;
        cout << "  " << u << " - " << v << endl;
        // Увеличиваем кратность в mult
        mult[u][v]++;
        mult[v][u]++;
    }

    return best_edges;
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
    // Проверка: если цикл не содержит все рёбра, вернём пустой
    // Но для эйлерова графа это корректно
    return cycle;
}