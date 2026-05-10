#include "common_structures.h"
#include <iomanip>

int** current_matrix = nullptr;
int current_matrix_N = 0;
bool current_matrix_oriented = false;

int** current_weight_matrix = nullptr;
int current_weight_matrix_N = 0;
bool weight_matrix_exist = false;

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

// Печать весовой матрицы в аккуратном табличном виде
void print_weight_matrix(int** weight, int N) {
    if (weight == nullptr || N <= 0) {
        cout << "Весовая матрица отсутствует или некорректного размера\n";
        return;
    }
    cout << "\n ВЕСОВАЯ МАТРИЦА" << endl;
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
            cout << setw(6) << weight[i][j] << " ";
        }
        cout << endl;
    }
}
