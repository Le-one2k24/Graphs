#include "shimbell.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <climits>
using namespace std;

const int INF = INT_MAX / 2; 

int** shimbell_multiply(int** A, int** B, int N, bool max_min) {
    int** C = new int*[N];
    for (int i = 0; i < N; i++) {
        C[i] = new int[N];
        for (int j = 0; j < N; j++) {
            bool found = false;
            int best = (max_min) ? -INF : INF;
            
            for (int k = 0; k < N; k++) {
                if (A[i][k] == INF || B[k][j] == INF) continue;
                if (A[i][k] == 0 || B[k][j] == 0) continue;
                
                int sum = A[i][k] + B[k][j];
                if (!found) {
                    best = sum;
                    found = true;
                } else {
                    if (!max_min) {
                        if (sum < best) best = sum;
                    } else {
                        if (sum > best) best = sum;
                    }
                }
            }
            
            C[i][j] = found ? best : INF;
        }
    }
    return C;
}

int** shimbell_all_paths(int** W, int N, int max_edges, bool find_max) {
    if (max_edges == 0) {
        int** result = new int*[N];
        for (int i = 0; i < N; i++) {
            result[i] = new int[N];
            for (int j = 0; j < N; j++) {
                result[i][j] = (i == j) ? 0 : INF;
            }
        }
        return result;
    }
    
    int** current = new int*[N];
    for (int i = 0; i < N; i++) {
        current[i] = new int[N];
        for (int j = 0; j < N; j++) {
            current[i][j] = (W[i][j] != 0) ? W[i][j] : INF;
        }
    }
    
    if (max_edges == 1) {
        int** result = new int*[N];
        for (int i = 0; i < N; i++) {
            result[i] = new int[N];
            for (int j = 0; j < N; j++) {
                result[i][j] = current[i][j];
            }
        }
        for (int i = 0; i < N; i++) delete[] current[i];
        delete[] current;
        return result;
    }
    
    for (int step = 2; step <= max_edges; step++) {
        int** next = shimbell_multiply(current, W, N, find_max);
        
        for (int i = 0; i < N; i++) delete[] current[i];
        delete[] current;
        
        current = next;
    }
    
    return current;
}

void print_matrix_with_inf(int** M, int N, const char* name) {
    cout << "\n" << name << ":\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (M[i][j] == INF) {
                cout << "-\t";
            } else {
                cout << M[i][j] << "\t";
            }
        }
        cout << endl;
    }
}