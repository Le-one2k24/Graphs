#include "common_structures.h"
#include <iomanip>

int** current_matrix = nullptr;
int current_matrix_N = 0;
bool current_matrix_oriented = false;

int** current_weight_matrix = nullptr;
int current_weight_matrix_N = 0;
bool weight_matrix_exist = false;

int** current_capacity_matrix = nullptr;
int capacity_matrix_N = 0;
bool capacity_matrix_exist = false;

int** current_cost_matrix = nullptr;
int cost_matrix_N = 0;
bool cost_matrix_exist = false;

Graph::Graph(int n) {
    N = n;
    list = new int*[N];
    size = new int[N]();
    for (int i = 0; i < N; i++) {
        list[i] = new int[100];
    }
}

Graph::~Graph() {
    for (int i = 0; i < N; i++) {
        delete[] list[i];
    }
    delete[] list;
    delete[] size;
}

void Graph::edge(int a, int b) {
    list[a][size[a]] = b;
    size[a]++;
    list[b][size[b]] = a;
    size[b]++;
}

void Graph::print() {
    cout << "Граф c " << N << " вершинами" << endl;
    for (int i = 0; i < N; i++) {
        cout << "Вершина " << i << ": ";
        for (int j = 0; j < size[i]; j++) {
            cout << list[i][j] << " ";
        }
        cout << endl;
    }
}

void print_smezhn_matrix(int** matrix, int N){
    if (!matrix) { cout << "(null)\n"; return; }
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void delete_matrix(int** M, int N){
    if (!M) return;
    for (int i = 0; i < N; ++i) {
        delete[] M[i];
    }
    delete[] M;
}

void set_current_matrix(int** matrix, int N, bool oriented){
    if (matrix == nullptr) {
        cout << "Ошибка: попытка сохранить пустую матрицу\n";
        return;
    }
    if (current_matrix != nullptr) {
        for (int i = 0; i < current_matrix_N; i++) delete[] current_matrix[i];
        delete[] current_matrix;
    }
    current_matrix_N = N;
    current_matrix_oriented = oriented;
    current_matrix = new int*[N];
    for (int i = 0; i < N; i++) {
        current_matrix[i] = new int[N];
        for (int j = 0; j < N; j++) {
            current_matrix[i][j] = matrix[i][j];
        }
    }
}

int** get_current_matrix(int& N, bool& oriented){
    if (current_matrix == nullptr) {
        N = 0;
        oriented = false;
        return nullptr;
    }
    N = current_matrix_N;
    oriented = current_matrix_oriented;
    return current_matrix;
}

void set_weight_matrix(int** matrix, int N){
    if (current_weight_matrix != nullptr) {
        for (int i = 0; i < current_weight_matrix_N; i++) {
            delete[] current_weight_matrix[i];
        }
        delete[] current_weight_matrix;
    }
    
    current_weight_matrix_N = N;
    current_weight_matrix = new int*[N];
    for (int i = 0; i < N; i++) {
        current_weight_matrix[i] = new int[N];
        for (int j = 0; j < N; j++) {
            current_weight_matrix[i][j] = matrix[i][j];
        }
    }
    weight_matrix_exist = true;
}

int** get_weight_matrix(int& N){
    if (!weight_matrix_exist) {
        N = 0;
        return nullptr;
    }
    N = current_weight_matrix_N;
    return current_weight_matrix;
}

bool is_weight_matrix_exist() {
    return weight_matrix_exist;
}

void clear_weight_matrix() {
    if (current_weight_matrix != nullptr) {
        for (int i = 0; i < current_weight_matrix_N; i++) {
            delete[] current_weight_matrix[i];
        }
        delete[] current_weight_matrix;
        current_weight_matrix = nullptr;
    }
    weight_matrix_exist = false;
    current_weight_matrix_N = 0;
}

// Печать матрицы в аккуратном табличном виде (общая функция)
static void print_matrix_tabular(int** M, int N, const char* title) {
    if (M == nullptr || N <= 0) {
        cout << title << " отсутствует или некорректного размера\n";
        return;
    }
    cout << "\n " << title << endl;
    cout << "      ";
    for (int j = 0; j < N; j++) {
        cout << setw(6) << j << " ";
    }
    cout << endl;
    cout << "    ";
    for (int j = 0; j < N; j++) {
        cout << "-------";
    }
    cout << endl;
    
    for (int i = 0; i < N; i++) {
        cout << setw(3) << i << " | ";
        for (int j = 0; j < N; j++) {
            cout << setw(6) << M[i][j] << " ";
        }
        cout << endl;
    }
}

void print_weight_matrix(int** weight, int N) {
    print_matrix_tabular(weight, N, "ВЕСОВАЯ МАТРИЦА");
}

// --------- Матрица пропускных способностей ---------

void set_capacity_matrix(int** matrix, int N) {
    if (current_capacity_matrix != nullptr) {
        for (int i = 0; i < capacity_matrix_N; ++i) {
            delete[] current_capacity_matrix[i];
        }
        delete[] current_capacity_matrix;
    }
    capacity_matrix_N = N;
    current_capacity_matrix = new int*[N];
    for (int i = 0; i < N; ++i) {
        current_capacity_matrix[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            current_capacity_matrix[i][j] = matrix[i][j];
        }
    }
    capacity_matrix_exist = true;
}

int** get_capacity_matrix(int& N) {
    if (!capacity_matrix_exist) {
        N = 0;
        return nullptr;
    }
    N = capacity_matrix_N;
    return current_capacity_matrix;
}

bool is_capacity_matrix_exist() {
    return capacity_matrix_exist;
}

void clear_capacity_matrix() {
    if (current_capacity_matrix != nullptr) {
        for (int i = 0; i < capacity_matrix_N; ++i) {
            delete[] current_capacity_matrix[i];
        }
        delete[] current_capacity_matrix;
        current_capacity_matrix = nullptr;
    }
    capacity_matrix_exist = false;
    capacity_matrix_N = 0;
}

void print_capacity_matrix(int** cap, int N) {
    print_matrix_tabular(cap, N, "МАТРИЦА ПРОПУСКНЫХ СПОСОБНОСТЕЙ");
}

void set_cost_matrix(int** matrix, int N) {
    if (current_cost_matrix != nullptr) {
        for (int i = 0; i < cost_matrix_N; ++i) {
            delete[] current_cost_matrix[i];
        }
        delete[] current_cost_matrix;
    }
    cost_matrix_N = N;
    current_cost_matrix = new int*[N];
    for (int i = 0; i < N; ++i) {
        current_cost_matrix[i] = new int[N];
        for (int j = 0; j < N; ++j) {
            current_cost_matrix[i][j] = matrix[i][j];
        }
    }
    cost_matrix_exist = true;
}

int** get_cost_matrix(int& N) {
    if (!cost_matrix_exist) {
        N = 0;
        return nullptr;
    }
    N = cost_matrix_N;
    return current_cost_matrix;
}

bool is_cost_matrix_exist() {
    return cost_matrix_exist;
}

void clear_cost_matrix() {
    if (current_cost_matrix != nullptr) {
        for (int i = 0; i < cost_matrix_N; ++i) {
            delete[] current_cost_matrix[i];
        }
        delete[] current_cost_matrix;
        current_cost_matrix = nullptr;
    }
    cost_matrix_exist = false;
    cost_matrix_N = 0;
}

void print_cost_matrix(int** cost, int N) {
    print_matrix_tabular(cost, N, "МАТРИЦА СТОИМОСТЕЙ");
}
