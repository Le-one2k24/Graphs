#include "flows.h"
#include "shortest_paths.h"
#include "generate_graphs.h"
#include <iostream>
#include <queue>
#include <vector>
#include <cstring>
using namespace std;


void generate_capacity_and_cost_for_current_graph() {
    int N; bool oriented;
    int** adj = get_current_matrix(N, oriented);
    if (!adj) {
        cout << "Нет текущего графа. Сначала сгенерируйте ориентированный граф (пункт 1).\n";
        return;
    }
    if (!oriented) {
        cout << "Матрицы пропускных способностей и стоимостей генерируются для ориентированного графа.\n";
        cout << "Сначала сгенерируйте ориентированный граф (пункт 1).\n";
        return;
    }

    clear_capacity_matrix();
    clear_cost_matrix();

    int** cap = new int*[N];
    int** cost = new int*[N];
    for (int i = 0; i < N; ++i) {
        cap[i] = new int[N];
        cost[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            cap[i][j] = 0;
            cost[i][j] = 0;
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (adj[i][j] == 1) {

                int c = generate_random_degree_champernaun();
                int w = generate_random_degree_champernaun();

                cap[i][j] = c;
                cost[i][j] = w;
            }
        }
    }

    set_capacity_matrix(cap, N);
    set_cost_matrix(cost, N);

    delete_matrix(cap, N);
    delete_matrix(cost, N);

    cout << "Матрицы пропускных способностей и стоимостей сгенерированы по распределению Чампернауна.\n";
}


static int bfs_for_flow(int** residual, int N, int s, int t, int* parent) {
    for (int i = 0; i < N; ++i) parent[i] = -1;
    parent[s] = -2; // источник
    queue<pair<int,int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int v = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int u = 0; u < N; ++u) {
            if (parent[u] == -1 && residual[v][u] > 0) {
                parent[u] = v;
                int new_flow = (flow < residual[v][u]) ? flow : residual[v][u];
                if (u == t) {
                    return new_flow;
                }
                q.push({u, new_flow});
            }
        }
    }
    return 0;
}

int max_flow_edmonds_karp(int** capacity, int N, int s, int t) {
    if (!capacity || N <= 0) return 0;
    if (s < 0 || s >= N || t < 0 || t >= N) return 0;

    int** residual = new int*[N];
    for (int i = 0; i < N; ++i) {
        residual[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            residual[i][j] = capacity[i][j];
        }
    }

    int* parent = new int[N];
    int max_flow = 0;

    while (true) {
        int flow = bfs_for_flow(residual, N, s, t, parent);
        if (flow == 0) break;
        max_flow += flow;

        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            residual[prev][cur] -= flow;
            residual[cur][prev] += flow;
            cur = prev;
        }
    }

    delete[] parent;
    delete_matrix(residual, N);
    return max_flow;
}


static void dijkstra_reduced(
    int** cap,
    int** cost,
    int N,
    int s,
    const vector<int>& potential,
    vector<int>& dist,
    vector<int>& parent
) {
    dist.assign(N, INF);
    parent.assign(N, -1);
    vector<bool> used(N, false);

    dist[s] = 0;

    for (int it = 0; it < N; ++it) {
        int v = -1;
        for (int i = 0; i < N; ++i) {
            if (!used[i] && (v == -1 || dist[i] < dist[v])) {
                v = i;
            }
        }
        if (v == -1 || dist[v] == INF) break;
        used[v] = true;

        for (int u = 0; u < N; ++u) {
            if (cap[v][u] > 0) {
                int w = cost[v][u];
                int rc = w + potential[v] - potential[u];
                if (dist[v] + rc < dist[u]) {
                    dist[u] = dist[v] + rc;
                    parent[u] = v;
                }
            }
        }
    }
}

long long min_cost_flow_with_dijkstra(
    int** capacity,
    int** cost,
    int N,
    int s,
    int t,
    int required_flow,
    int& achieved_flow
) {
    achieved_flow = 0;
    if (!capacity || !cost || N <= 0) return 0;
    if (s < 0 || s >= N || t < 0 || t >= N) return 0;
    if (required_flow <= 0) return 0;

    int** cap = new int*[N];
    for (int i = 0; i < N; ++i) {
        cap[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            cap[i][j] = capacity[i][j];
        }
    }

    int** c = new int*[N];
    for (int i = 0; i < N; ++i) {
        c[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            c[i][j] = cost[i][j];
        }
    }

    vector<int> potential(N, 0);
    vector<int> dist(N);
    vector<int> parent(N);

    long long total_cost = 0;

    while (achieved_flow < required_flow) {
        dijkstra_reduced(cap, c, N, s, potential, dist, parent);
        if (dist[t] == INF) break;

        for (int i = 0; i < N; ++i) {
            if (dist[i] < INF) {
                potential[i] += dist[i];
            }
        }

        int add_flow = required_flow - achieved_flow;
        int v = t;
        while (v != s) {
            int u = parent[v];
            if (u == -1) { add_flow = 0; break; }
            if (cap[u][v] < add_flow) add_flow = cap[u][v];
            v = u;
        }
        if (add_flow <= 0) break;

        v = t;
        while (v != s) {
            int u = parent[v];
            if (capacity[u][v] > 0 && c[u][v] == cost[u][v]) {
                cap[u][v] -= add_flow;
                cap[v][u] += add_flow;
                total_cost += 1LL * add_flow * cost[u][v];
            } else {
                cap[u][v] -= add_flow;
                cap[v][u] += add_flow;
                total_cost -= 1LL * add_flow * cost[v][u];
            }
            v = u;
        }

        achieved_flow += add_flow;
    }

    for (int i = 0; i < N; ++i) {
        delete[] cap[i];
        delete[] c[i];
    }
    delete[] cap;
    delete[] c;

    return total_cost;
}
