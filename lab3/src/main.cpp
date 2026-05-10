#include <iostream>
#include <cstdlib>
#include <ctime>
#include "common_structures.h"
#include "generate_graphs.h"
#include "metrics.h"
#include "shimbell.h"
#include "routes.h"
#include "articulation.h"
#include "shortest_paths.h"
#include "flows.h"

using namespace std;

void print_menu(){
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
    cout << "0. Выход\n";
    cout << "Ваш выбор: ";
}

int main(){
    srand((unsigned)time(nullptr));
    int choice;
    int N = 0;

    do {
        print_menu();
        cin >> choice;
        switch(choice){
            case 1: {
                cout << "Введите количество вершин: ";
                cin >> N;
                if (N <= 0){ 
                    cout << "Неверное N\n"; 
                    break; 
                }

                clear_weight_matrix();
                clear_capacity_matrix();
                clear_cost_matrix();

                int** adj = nullptr;
                for (int attempt = 0; attempt < 20; ++attempt){
                    adj = generate_directed_by_degrees(N);

                    bool* vis = new bool[N];
                    for (int i=0;i<N;++i) vis[i]=false;
                    int* q = new int[N]; int h=0,t=0; q[t++]=0; vis[0]=true;
                    while(h<t){
                        int v=q[h++];
                        for (int u=0;u<N;++u){
                            if ((adj[v][u]==1 || adj[u][v]==1) && !vis[u]){ vis[u]=true; q[t++]=u; }
                        }
                    }
                    bool ok=true; for (int i=0;i<N;++i) if (!vis[i]) { ok=false; break; }
                    delete[] vis; delete[] q;
                    if (ok) break;
                    delete_matrix(adj, N); adj = nullptr;
                }
                if (!adj){ cout << "Не удалось сгенерировать связный ориентированный граф\n"; break; }
                set_current_matrix(adj, N, true);
                delete_matrix(adj, N);
                cout << "Ориентированный связный ациклический граф сгенерирован и сохранён как текущая матрица.\n";
                break;
            }
            case 2: {
                int curN; bool oriented;
                int** cur = get_current_matrix(curN, oriented);
                if (!cur){
                    cout << "Нет текущего графа. Сначала сгенерируйте ориентированный граф в пункте 1.\n";
                    break;
                }
                if (!oriented){
                    cout << "Текущий граф уже неориентированный.\n";
                    break;
                }

                int** und = new int*[curN];
                for (int i = 0; i < curN; ++i){
                    und[i] = new int[curN];
                    for (int j = 0; j < curN; ++j) und[i][j] = 0;
                }
                for (int i = 0; i < curN; ++i){
                    for (int j = 0; j < curN; ++j){
                        if (cur[i][j] == 1 || cur[j][i] == 1){
                            und[i][j] = 1;
                            und[j][i] = 1;
                        }
                    }
                }
                set_current_matrix(und, curN, false);
                delete_matrix(und, curN);

                if (is_weight_matrix_exist()){
                    int wn; int** W = get_weight_matrix(wn);
                    if (wn == curN){
                        int** Wsym = new int*[wn];
                        for (int i = 0; i < wn; ++i){
                            Wsym[i] = new int[wn];
                            for (int j = 0; j < wn; ++j) Wsym[i][j] = 0;
                        }
                        for (int i = 0; i < wn; ++i){
                            for (int j = i; j < wn; ++j){
                                int a = W[i][j];
                                int b = W[j][i];
                                int val = 0;
                                if (a != 0 && b != 0) {
                                    val = a;
                                } else if (a != 0) {
                                    val = a;
                                } else if (b != 0) {
                                    val = b;
                                } else {
                                    val = 0;
                                }
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
                if (!cur){ cout << "Нет текущего графа\n"; break; }
                cout << "Выберите тип весов: 1-положительные, 2-отрицательные, 3-смешанные: ";
                int sign; cin >> sign;

                int** W = new int*[curN];
                for (int i = 0; i < curN; ++i){
                    W[i] = new int[curN];
                    for (int j = 0; j < curN; ++j) W[i][j] = 0;
                }

                for (int i = 0; i < curN; ++i){
                    for (int j = 0; j < curN; ++j){
                        bool has = oriented ? (cur[i][j]==1) : (i<j && (cur[i][j]==1 || cur[j][i]==1));
                        if (!has) continue;
                        int w = 0;
                        while (w == 0) w = generate_random_degree_champernaun();
                        if (sign == 2) w = -w;
                        else if (sign == 3 && rand()%2==0) w = -w;
                        W[i][j] = w;
                        if (!oriented && i<j) W[j][i] = w;
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
                if (!cur){ cout << "Нет текущего графа\n"; break; }
                cout << (oriented ? "Текущий граф: ориентированный\n" : "Текущий граф: неориентированный\n");
                print_smezhn_matrix(cur, curN);

                if (!oriented){
                    Graph* g = new Graph(curN);
                    for (int i = 0; i < curN; ++i){
                        for (int j = i+1; j < curN; ++j){
                            if (cur[i][j]==1 || cur[j][i]==1) g->edge(i,j);
                        }
                    }
                    int* eccs = all_eccentricites(g);
                    cout << "Эксцентриситеты:\n";
                    for (int i = 0; i < curN; ++i) cout << "e("<<i<<")="<<eccs[i]<<"\n";
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
                if (!is_weight_matrix_exist()){
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
                if (!cur){ cout << "Нет текущего графа\n"; break; }
                if (!is_weight_matrix_exist()){ cout << "Нет сохранённой весовой матрицы (сгенерируйте в пункте 3)\n"; break; }
                int Nw; int** W = get_weight_matrix(Nw);
                cout << "Введите максимальное число рёбер в пути: ";
                int maxe; cin >> maxe;
                cout << "1 - минимальные пути, 2 - максимальные, 3 - оба: ";
                int t; cin >> t;
                if (t==1 || t==3){
                    int** minp = shimbell_all_paths(W, Nw, maxe, false);
                    print_matrix_with_inf(minp, Nw, "Минимальные пути");
                    delete_matrix(minp, Nw);
                }
                if (t==2 || t==3){
                    int** maxp = shimbell_all_paths(W, Nw, maxe, true);
                    print_matrix_with_inf(maxp, Nw, "Максимальные пути");
                    delete_matrix(maxp, Nw);
                }
                break;
            }
            case 7: {
                int curN; bool oriented;
                int** cur = get_current_matrix(curN, oriented);
                if (!cur){ 
                    cout << "Нет текущего графа\n"; 
                    break; 
                }
            
                int s, e; 
                cout << "Старт: "; cin >> s; 
                cout << "Финиш: "; cin >> e;
            
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

                int s, e;
                cout << "Стартовая вершина: ";
                cin >> s;
                cout << "Конечная вершина: ";
                cin >> e;

                if (s < 0 || s >= Nw || e < 0 || e >= Nw) {
                    cout << "Некорректные номера вершин\n";
                    break;
                }

                int* dist = new int[Nw];
                int* parent = new int[Nw];
                long long iters = 0;

                dijkstra_shortest(W, Nw, s, dist, parent, iters);

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

                int s, t;
                cout << "Источник (s): ";
                cin >> s;
                cout << "Сток (t): ";
                cin >> t;

                if (s < 0 || s >= Ncap || t < 0 || t >= Ncap) {
                    cout << "Некорректные номера вершин\n";
                    break;
                }

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

                int s, t;
                cout << "Источник (s): ";
                cin >> s;
                cout << "Сток (t): ";
                cin >> t;

                if (s < 0 || s >= Ncap || t < 0 || t >= Ncap) {
                    cout << "Некорректные номера вершин\n";
                    break;
                }

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
                long long cost = min_cost_flow_with_dijkstra(C, Wc, Ncap, s, t, required_flow, achieved_flow);

                cout << "Достигнутый поток: " << achieved_flow << "\n";
                if (achieved_flow < required_flow) {
                    cout << "Предупреждение: не удалось провести требуемый поток, достигнут максимум по пропускным способностям.\n";
                }
                cout << "Минимальная стоимость этого потока: " << cost << "\n";
                break;
            }
            case 0: {
                cout << "Выход\n";
                break;
            }
            default: cout << "Неверный выбор\n";
        }
    } while (choice != 0);

    if (current_matrix){
        for (int i = 0; i < current_matrix_N; ++i) delete[] current_matrix[i];
        delete[] current_matrix;
        current_matrix = nullptr;
    }
    clear_weight_matrix();
    clear_capacity_matrix();
    clear_cost_matrix();
    return 0;
}
