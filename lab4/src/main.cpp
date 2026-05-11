#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <climits>
#include "common_structures.h"
#include "generate_graphs.h"
#include "metrics.h"
#include "shimbell.h"
#include "routes.h"
#include "articulation.h"
#include "shortest_paths.h"
#include "flows.h"
#include "boruvka.h"
#include "coloring.h"
#include "kirchhoff.h"
#include "prufer.h"

using namespace std;

void print_menu() {
    cout << "\nМЕНЮ\n";
    cout << "1. Сгенерировать ориентированный связный ациклический граф (по степеням Чампернауна)\n";
    cout << "2. Сгенерировать неориентированный граф из текущего ориентированного (симметризация матриц)\n";
    cout << "3. Сгенерировать весовую матрицу для текущего графа (положит/отриц/смеш)\n";
    cout << "4. Показать текущий граф и метрики (эксцентриситеты, центр, диаметр)\n";
    cout << "5. Показать сохранённую весовую матрицу\n";
    cout << "6. Метод Шимбелла (минимум/максимум путей с ограничением по числу рёбер)\n";
    cout << "7. Подсчитать количество маршрутов между двумя вершинами\n";
    cout << "8. Найти точки сочленения в текущем графе (только для неориентированного)\n";
    cout << "9. Найти кратчайший путь между двумя вершинами (Классический алгоритм Дейкстры)\n";
    cout << "10. Сгенерировать матрицы пропускных способностей и стоимостей для текущего графа\n";
    cout << "11. Показать матрицы пропускных способностей и стоимостей\n";
    cout << "12. Найти максимальный поток (алгоритм Эдмондса-Карпа)\n";
    cout << "13. Найти поток минимальной стоимости объёма [2/3 * max]\n";
    cout << "14. Число остовных деревьев (теорема Кирхгофа) для текущего неориентированного графа\n";
    cout << "15. Построить минимальное остовное дерево (алгоритм Борувки) и сохранить его как последний MST\n";
    cout << "16. Код Прюфера: закодировать последний MST и декодировать обратно (с сохранением весов)\n";
    cout << "17. Минимальная раскраска графа (на исходном графе или на последнем MST)\n";
    cout << "0. Выход\n";
    cout << "Ваш выбор: ";
}

// Вспомогательная функция для ввода целого числа с проверкой диапазона и возможностью выхода (-1)
int input_int(const string& prompt, int min_val, int max_val, bool allow_cancel = true) {
    int val;
    while (true) {
        cout << prompt;
        if (!(cin >> val)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка ввода. Введите целое число.\n";
            continue;
        }
        if (allow_cancel && val == -1) {
            cout << "Отмена операции.\n";
            return -1;
        }
        if (val >= min_val && val <= max_val) {
            return val;
        }
        cout << "Значение должно быть в диапазоне [" << min_val << ", " << max_val << "]. Повторите.\n";
    }
}

int main() {
    srand((unsigned)time(nullptr));
    int choice;
    int N = 0;

    do {
        print_menu();
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Некорректный ввод. Повторите.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                // Ввод N
                int n = input_int("Введите количество вершин (или -1 для отмены): ", 1, INT_MAX, true);
                if (n == -1) break;
                N = n;

                clear_weight_matrix();
                clear_capacity_matrix();
                clear_cost_matrix();

                int** adj = nullptr;
                for (int attempt = 0; attempt < 20; ++attempt) {
                    adj = generate_directed_by_degrees(N);

                    bool* vis = new bool[N];
                    for (int i = 0; i < N; ++i) vis[i] = false;
                    int* q = new int[N]; int h = 0, t = 0;
                    q[t++] = 0; vis[0] = true;
                    while (h < t) {
                        int v = q[h++];
                        for (int u = 0; u < N; ++u) {
                            if ((adj[v][u] == 1 || adj[u][v] == 1) && !vis[u]) {
                                vis[u] = true; q[t++] = u;
                            }
                        }
                    }
                    bool ok = true;
                    for (int i = 0; i < N; ++i) if (!vis[i]) { ok = false; break; }
                    delete[] vis; delete[] q;
                    if (ok) break;
                    delete_matrix(adj, N); adj = nullptr;
                }
                if (!adj) {
                    cout << "Не удалось сгенерировать связный ориентированный граф\n";
                    break;
                }
                set_current_matrix(adj, N, true);
                delete_matrix(adj, N);
                cout << "Ориентированный связный ациклический граф сгенерирован и сохранён как текущая матрица.\n";
                break;
            }
            case 2: {
                int curN; bool oriented;
                int** cur = get_current_matrix(curN, oriented);
                if (!cur) {
                    cout << "Нет текущего графа. Сначала сгенерируйте ориентированный граф в пункте 1.\n";
                    break;
                }
                if (!oriented) {
                    cout << "Текущий граф уже неориентированный.\n";
                    break;
                }

                int** und = new int* [curN];
                for (int i = 0; i < curN; ++i) {
                    und[i] = new int[curN];
                    for (int j = 0; j < curN; ++j) und[i][j] = 0;
                }
                for (int i = 0; i < curN; ++i) {
                    for (int j = 0; j < curN; ++j) {
                        if (cur[i][j] == 1 || cur[j][i] == 1) {
                            und[i][j] = 1;
                            und[j][i] = 1;
                        }
                    }
                }
                set_current_matrix(und, curN, false);
                delete_matrix(und, curN);

                if (is_weight_matrix_exist()) {
                    int wn; int** W = get_weight_matrix(wn);
                    if (wn == curN) {
                        int** Wsym = new int* [wn];
                        for (int i = 0; i < wn; ++i) {
                            Wsym[i] = new int[wn];
                            for (int j = 0; j < wn; ++j) Wsym[i][j] = 0;
                        }
                        for (int i = 0; i < wn; ++i) {
                            for (int j = i; j < wn; ++j) {
                                int a = W[i][j];
                                int b = W[j][i];
                                int val = 0;
                                if (a != 0 && b != 0) val = a;
                                else if (a != 0) val = a;
                                else if (b != 0) val = b;
                                else val = 0;
                                Wsym[i][j] = val;
                                Wsym[j][i] = val;
                            }
                        }
                        set_weight_matrix(Wsym, wn);
                        delete_matrix(Wsym, wn);
                    } else {
                        cout << "Размер сохранённой весовой матрицы не совпадает с текущим графом — весовая матрица не симметризована.\n";
                    }
                }
                cout << "Преобразование выполнено: текущая матрица теперь неориентированная. Весовая матрица симметризована (если была сохранена и совпадала по размеру).\n";
                break;
            }
            case 3: {
                int curN; bool oriented;
                int** cur = get_current_matrix(curN, oriented);
                if (!cur) {
                    cout << "Нет текущего графа\n";
                    break;
                }
                int sign = input_int("Выберите тип весов: 1-положительные, 2-отрицательные, 3-смешанные (или -1 для отмены): ", 1, 3, true);
                if (sign == -1) break;

                int** W = new int* [curN];
                for (int i = 0; i < curN; ++i) {
                    W[i] = new int[curN];
                    for (int j = 0; j < curN; ++j) W[i][j] = 0;
                }

                double weight_mu = 8.0;
                double weight_alpha = 0.9;

                for (int i = 0; i < curN; ++i) {
                    for (int j = 0; j < curN; ++j) {
                        bool has = oriented ? (cur[i][j] == 1) : (i < j && (cur[i][j] == 1 || cur[j][i] == 1));
                        if (!has) continue;
                        int w = 0;
                        while (w == 0) w = generate_random_degree_champernaun(weight_mu, weight_alpha);
                        if (sign == 2) w = -w;
                        else if (sign == 3 && rand() % 2 == 0) w = -w;
                        W[i][j] = w;
                        if (!oriented && i < j) W[j][i] = w;
                    }
                }
                set_weight_matrix(W, curN);
                cout << "Весовая матрица сгенерирована и сохранена.\n";
                delete_matrix(W, curN);
                break;
            }
            case 4: {
                int curN; bool oriented;
                int** cur = get_current_matrix(curN, oriented);
                if (!cur) {
                    cout << "Нет текущего графа\n";
                    break;
                }
                cout << (oriented ? "Текущий граф: ориентированный\n" : "Текущий граф: неориентированный\n");
                print_smezhn_matrix(cur, curN);

                if (!oriented) {
                    Graph* g = new Graph(curN);
                    for (int i = 0; i < curN; ++i) {
                        for (int j = i + 1; j < curN; ++j) {
                            if (cur[i][j] == 1 || cur[j][i] == 1) g->edge(i, j);
                        }
                    }
                    int* eccs = all_eccentricites(g);
                    cout << "Эксцентриситеты:\n";
                    for (int i = 0; i < curN; ++i) cout << "e(" << i << ")=" << eccs[i] << "\n";
                    find_center(eccs, curN);
                    find_diametr(eccs, curN);
                    delete[] eccs;
                    delete g;
                } else {
                    int* eccs = eccentricites_oriented(cur, curN);
                    print_eccentricities_oriented(eccs, curN);
                    find_center_oriented(eccs, curN);
                    find_diametr_oriented(eccs, curN);
                    delete[] eccs;
                }
                break;
            }
            case 5: {
                if (!is_weight_matrix_exist()) {
                    cout << "Весовая матрица не сохранена. Сгенерируйте её в пункте 3.\n";
                    break;
                }
                int wn; int** W = get_weight_matrix(wn);
                cout << "Сохранённая весовая матрица:\n";
                print_weight_matrix(W, wn);
                break;
            }
            case 6: {
                int curN; bool oriented;
                int** cur = get_current_matrix(curN, oriented);
                if (!cur) {
                    cout << "Нет текущего графа\n";
                    break;
                }
                if (!is_weight_matrix_exist()) {
                    cout << "Нет сохранённой весовой матрицы (сгенерируйте в пункте 3)\n";
                    break;
                }
                int Nw; int** W = get_weight_matrix(Nw);
                int maxe = input_int("Введите максимальное число рёбер в пути (0 - только диагональ, -1 для отмены): ", 0, INT_MAX, true);
                if (maxe == -1) break;
                int t = input_int("1 - минимальные пути, 2 - максимальные, 3 - оба (или -1 для отмены): ", 1, 3, true);
                if (t == -1) break;
                if (t == 1 || t == 3) {
                    int** minp = shimbell_all_paths(W, Nw, maxe, false);
                    print_matrix_with_inf(minp, Nw, "Минимальные пути");
                    delete_matrix(minp, Nw);
                }
                if (t == 2 || t == 3) {
                    int** maxp = shimbell_all_paths(W, Nw, maxe, true);
                    print_matrix_with_inf(maxp, Nw, "Максимальные пути");
                    delete_matrix(maxp, Nw);
                }
                break;
            }
            case 7: {
                int curN; bool oriented;
                int** cur = get_current_matrix(curN, oriented);
                if (!cur) {
                    cout << "Нет текущего графа\n";
                    break;
                }

                int s = input_int("Стартовая вершина (или -1 для отмены): ", 0, curN - 1, true);
                if (s == -1) break;
                int e = input_int("Конечная вершина (или -1 для отмены): ", 0, curN - 1, true);
                if (e == -1) break;

                int cnt = count_routes_matrix(cur, curN, oriented, s, e);
                if (cnt < 0) {
                    cout << "Ошибка: некорректные номера вершин\n";
                } else if (cnt == 0) {
                    cout << "Маршрут из вершины " << s << " в вершину " << e << " отсутствует\n";
                } else {
                    cout << "Количество маршрутов: " << cnt << "\n";
                }
                break;
            }
            case 8: {
                int curN; bool oriented;
                int** cur = get_current_matrix(curN, oriented);
                if (!cur) {
                    cout << "Нет текущего графа\n";
                    break;
                }
                if (oriented) {
                    cout << "Точки сочленения определяются для неориентированных графов.\n";
                    cout << "Сначала преобразуйте граф в неориентированный (пункт 2).\n";
                    break;
                }
                cout << "Текущий неориентированный граф (матрица смежности):\n";
                print_smezhn_matrix(cur, curN);
                find_articulation_points_matrix(cur, curN);
                break;
            }
            case 9: {
                if (!is_weight_matrix_exist()) {
                    cout << "Нет весовой матрицы. Сгенерируйте её в пункте 3.\n";
                    break;
                }
                int Nw;
                int** W = get_weight_matrix(Nw);

                int s = input_int("Стартовая вершина (или -1 для отмены): ", 0, Nw - 1, true);
                if (s == -1) break;
                int e = input_int("Конечная вершина (или -1 для отмены): ", 0, Nw - 1, true);
                if (e == -1) break;

                int* dist = new int[Nw];
                int* parent = new int[Nw];
                long long iters = 0;

                bool ok = dijkstra_shortest(W, Nw, s, dist, parent, iters);
                if (!ok) {
                    delete[] dist;
                    delete[] parent;
                    break;
                }

                cout << "Вектор расстояний (Дейкстра):\n";
                for (int i = 0; i < Nw; ++i) {
                    cout << "dist[" << i << "] = ";
                    if (dist[i] >= INF) cout << "INF";
                    else cout << dist[i];
                    cout << "\n";
                }

                cout << "Кратчайший путь " << s << " -> " << e << ": ";
                if (dist[e] >= INF) {
                    cout << "пути нет\n";
                } else {
                    print_path_from_parents(s, e, parent);
                    cout << "\nДлина пути: " << dist[e] << "\n";
                }
                cout << "Количество итераций: " << iters << "\n";

                delete[] dist;
                delete[] parent;
                break;
            }
            case 10: {
                generate_capacity_and_cost_for_current_graph();
                break;
            }
            case 11: {
                if (!is_capacity_matrix_exist()) {
                    cout << "Матрица пропускных способностей не сохранена. Сгенерируйте её в пункте 10.\n";
                } else {
                    int nc; int** C = get_capacity_matrix(nc);
                    print_capacity_matrix(C, nc);
                }
                if (!is_cost_matrix_exist()) {
                    cout << "Матрица стоимостей не сохранена. Сгенерируйте её в пункте 10.\n";
                } else {
                    int nw; int** Wc = get_cost_matrix(nw);
                    print_cost_matrix(Wc, nw);
                }
                break;
            }
            case 12: {
                if (!is_capacity_matrix_exist()) {
                    cout << "Нет матрицы пропускных способностей. Сгенерируйте её в пункте 10.\n";
                    break;
                }
                int Ncap; int** C = get_capacity_matrix(Ncap);
                int s = input_int("Источник (s) (или -1 для отмены): ", 0, Ncap - 1, true);
                if (s == -1) break;
                int t = input_int("Сток (t) (или -1 для отмены): ", 0, Ncap - 1, true);
                if (t == -1) break;

                int maxflow = max_flow_edmonds_karp(C, Ncap, s, t);
                cout << "Максимальный поток из " << s << " в " << t << " = " << maxflow << "\n";
                break;
            }
            case 13: {
                if (!is_capacity_matrix_exist() || !is_cost_matrix_exist()) {
                    cout << "Нет матриц пропускных способностей и/или стоимостей. Сгенерируйте их в пункте 10.\n";
                    break;
                }
                int Ncap, Ncost;
                int** C = get_capacity_matrix(Ncap);
                int** Wc = get_cost_matrix(Ncost);
                if (Ncap != Ncost) {
                    cout << "Размеры матриц пропускных способностей и стоимостей не совпадают.\n";
                    break;
                }
                int s = input_int("Источник (s) (или -1 для отмены): ", 0, Ncap - 1, true);
                if (s == -1) break;
                int t = input_int("Сток (t) (или -1 для отмены): ", 0, Ncap - 1, true);
                if (t == -1) break;

                int maxflow = max_flow_edmonds_karp(C, Ncap, s, t);
                cout << "Максимальный поток из " << s << " в " << t << " = " << maxflow << "\n";
                if (maxflow == 0) {
                    cout << "Максимальный поток равен 0, поток минимальной стоимости не имеет смысла.\n";
                    break;
                }

                int required_flow = (2 * maxflow) / 3;
                if (required_flow <= 0) required_flow = 1;
                cout << "Требуемый объём потока для минимальной стоимости: " << required_flow << " ( [2/3 * max] )\n";

                int achieved_flow = 0;
                int** flow = new int* [Ncap];
                for (int i = 0; i < Ncap; ++i) {
                    flow[i] = new int[Ncap];
                    for (int j = 0; j < Ncap; ++j) flow[i][j] = 0;
                }

                long long cost = min_cost_flow_with_dijkstra(C, Wc, Ncap, s, t, required_flow, achieved_flow, flow);
                cout << "Достигнутый поток: " << achieved_flow << "\n";
                cout << "Минимальная стоимость этого потока: " << cost << "\n";

                vector<vector<int>> paths;
                vector<int> path_flow;
                vector<long long> path_cost;
                decompose_flow_into_paths(flow, Ncap, s, t, paths, path_flow, path_cost, Wc);

                cout << "\nНайдено маршрутов: " << paths.size() << "\n";
                int sumFlows = 0;
                long long sumCosts = 0;
                for (size_t i = 0; i < paths.size(); ++i) {
                    sumFlows += path_flow[i];
                    sumCosts += path_cost[i];
                }
                cout << "Суммарный поток по маршрутам: " << sumFlows << "\n";
                cout << "Суммарная стоимость по маршрутам: " << sumCosts << "\n";

                cout << "\nИспользованные маршруты:\n";
                if (paths.empty()) {
                    cout << "(нет использованных маршрутов)\n";
                } else {
                    for (size_t i = 0; i < paths.size(); ++i) {
                        for (size_t j = 0; j < paths[i].size(); ++j) {
                            cout << paths[i][j];
                            if (j + 1 < paths[i].size()) cout << " -> ";
                        }
                        long long flow_val = path_flow[i];
                        long long unit_cost = (flow_val == 0 ? 0 : path_cost[i] / flow_val);
                        cout << " : " << flow_val << " * " << unit_cost << " = " << path_cost[i] << "\n";
                    }
                }

                for (int i = 0; i < Ncap; ++i) delete[] flow[i];
                delete[] flow;
                break;
            }
            case 14: {
                int curN; bool oriented;
                int** cur = get_current_matrix(curN, oriented);
                if (!cur) {
                    cout << "Нет текущего графа. Сгенерируйте его в пункте 1.\n";
                    break;
                }
                if (oriented) {
                    cout << "Теорема Кирхгофа применяется к неориентированным графам. Сначала преобразуйте (пункт 2).\n";
                    break;
                }
                long long trees = count_spanning_trees(cur, curN);
                if (trees < 0) {
                    cout << "Ошибка при подсчёте\n";
                } else {
                    cout << "Число остовных деревьев (по теореме Кирхгофа): " << trees << "\n";
                }
                break;
            }
            case 15: {
                int curN; bool oriented;
                int** cur = get_current_matrix(curN, oriented);
                if (!cur) {
                    cout << "Нет текущего графа\n";
                    break;
                }
                if (oriented) {
                    cout << "Для Борувки нужен неориентированный граф. Сначала преобразуйте (пункт 2).\n";
                    break;
                }
                if (!is_weight_matrix_exist()) {
                    cout << "Нет весовой матрицы. Сгенерируйте её в пункте 3.\n";
                    break;
                }
                int wn; int** W = get_weight_matrix(wn);
                if (wn != curN) {
                    cout << "Размер весовой матрицы не совпадает с текущим графом\n";
                    break;
                }

                int total_weight = 0;
                int** mst = boruvka_mst(cur, W, curN, total_weight);
                if (!mst) {
                    cout << "Не удалось построить MST (возможно граф несвязен)\n";
                    break;
                }

                cout << "MST (алгоритм Борувки) построено. Суммарный вес = " << total_weight << "\n";
                cout << "Матрица смежности MST:\n";
                print_smezhn_matrix(mst, curN);

                cout << "Весовая матрица MST:\n";
                for (int i = 0; i < curN; ++i) {
                    for (int j = 0; j < curN; ++j) {
                        if (mst[i][j] == 1)
                            cout << W[i][j] << " ";
                        else
                            cout << "0 ";
                    }
                    cout << "\n";
                }

                for (int i = 0; i < curN; ++i) delete[] mst[i];
                delete[] mst;
                break;
            }
            case 16: {
                int mstN;
                int** mst = get_last_mst(mstN);
                if (!mst) {
                    cout << "Последний MST отсутствует. Сначала постройте MST (пункт 15).\n";
                    break;
                }
                if (!is_weight_matrix_exist()) {
                    cout << "Нет весовой матрицы. Сгенерируйте её в пункте 3.\n";
                    break;
                }
                int wn;
                int** W = get_weight_matrix(wn);
                if (wn != mstN) {
                    cout << "Размер весовой матрицы не совпадает с MST\n";
                    break;
                }

                cout << "Кодирование MST в код Прюфера (с сохранением весов)...\n";

                vector<int> code_nodes;
                vector<int> code_weights;
                int last_u = -1, last_v = -1;

                bool ok = encode_prufer_with_weights(mst, W, mstN, code_nodes, code_weights, last_u, last_v);
                if (!ok) {
                    cout << "Ошибка при кодировании Прюфера\n";
                    break;
                }

                cout << "Код Прюфера (вершины): ";
                for (size_t i = 0; i < code_nodes.size(); ++i)
                    cout << code_nodes[i] << " ";
                cout << "\nСоответствующие веса удаляемых рёбер: ";
                for (size_t i = 0; i < code_weights.size(); ++i)
                    cout << code_weights[i] << " ";
                cout << "\nПоследнее ребро: " << last_u << " - " << last_v
                     << " (вес " << code_weights.back() << ")\n";

                cout << "\nДекодирование кода Прюфера обратно в дерево...\n";

                int** decoded_weights = nullptr;
                int** decoded = decode_prufer_with_weights(code_nodes, code_weights, mstN, decoded_weights);
                if (!decoded) {
                    cout << "Ошибка при декодировании Прюфера\n";
                    break;
                }

                cout << "\nВосстановленное дерево (матрица смежности):\n";
                print_smezhn_matrix(decoded, mstN);

                cout << "\nВосстановленная весовая матрица:\n";
                for (int i = 0; i < mstN; ++i) {
                    for (int j = 0; j < mstN; ++j) {
                        cout << decoded_weights[i][j] << " ";
                    }
                    cout << "\n";
                }

                for (int i = 0; i < mstN; ++i) {
                    delete[] decoded[i];
                    delete[] decoded_weights[i];
                }
                delete[] decoded;
                delete[] decoded_weights;
                break;
            }
            case 17: {
                int opt = input_int("Выберите граф для раскраски: 1 - текущий граф, 2 - последний MST (если есть) (или -1 для отмены): ", 1, 2, true);
                if (opt == -1) break;
                int** target = nullptr;
                int TN = 0;
                if (opt == 1) {
                    int curN; bool oriented;
                    int** cur = get_current_matrix(curN, oriented);
                    if (!cur) {
                        cout << "Нет текущего графа\n";
                        break;
                    }
                    if (oriented) {
                        cout << "Раскраска реализована для неориентированных графов. Сначала преобразуйте (пункт 2).\n";
                        break;
                    }
                    target = cur;
                    TN = curN;
                } else {
                    int mstN;
                    int** mst = get_last_mst(mstN);
                    if (!mst) {
                        cout << "Последний MST отсутствует. Сначала постройте MST (пункт 15).\n";
                        break;
                    }
                    target = mst;
                    TN = mstN;
                }

                int colors_used = 0;
                int* colors = dsatur_coloring(target, TN, colors_used);
                if (!colors) {
                    cout << "Ошибка при раскраске\n";
                    break;
                }

                cout << "Раскраска выполнена. Использовано цветов: " << colors_used << "\n";
                for (int i = 0; i < TN; ++i) {
                    cout << "Вершина " << i << " : цвет " << colors[i] << "\n";
                }
                delete[] colors;
                break;
            }
            case 0:
                cout << "Выход\n";
                break;
            default:
                cout << "Неверный выбор\n";
        }
    } while (choice != 0);

    if (current_matrix) {
        for (int i = 0; i < current_matrix_N; ++i) delete[] current_matrix[i];
        delete[] current_matrix;
        current_matrix = nullptr;
    }
    clear_weight_matrix();
    clear_capacity_matrix();
    clear_cost_matrix();
    clear_last_mst();
    return 0;
}