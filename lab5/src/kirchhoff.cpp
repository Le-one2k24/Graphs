#include "kirchhoff.h"
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

static long double determinant_ld(vector<vector<long double>>& A, int n) {
    long double det = 1.0L;
    for (int i = 0; i < n; ++i) {
        int pivot = i;
        long double maxv = fabsl(A[i][i]);
        for (int r = i+1; r < n; ++r) {
            long double val = fabsl(A[r][i]);
            if (val > maxv) { maxv = val; pivot = r; }
        }
        if (fabsl(maxv) < 1e-18L) {
            return 0.0L;
        }
        if (pivot != i) {
            swap(A[pivot], A[i]);
            det = -det;
        }
        long double diag = A[i][i];
        det *= diag;
        for (int r = i+1; r < n; ++r) {
            long double factor = A[r][i] / diag;
            if (fabsl(factor) < 1e-18L) continue;
            for (int c = i; c < n; ++c) {
                A[r][c] -= factor * A[i][c];
            }
        }
    }
    return det;
}

long long count_spanning_trees(int** adj, int N) {
    if (!adj || N <= 0) return -1;

    vector<vector<long double>> L(N, vector<long double>(N, 0.0L));
    for (int i = 0; i < N; ++i) {
        int deg = 0;
        for (int j = 0; j < N; ++j) {
            if (i == j) continue;
            if (adj[i][j] == 1 || adj[j][i] == 1) deg++;
        }
        L[i][i] = deg;
        for (int j = 0; j < N; ++j) {
            if (i == j) continue;
            if (adj[i][j] == 1 || adj[j][i] == 1) L[i][j] = -1.0L;
        }
    }

    int n = N - 1;
    vector<vector<long double>> M(n, vector<long double>(n, 0.0L));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            M[i][j] = L[i][j];

    long double det = determinant_ld(M, n);
    if (det < 0) det = -det;
    long long result = (long long) llround(det);
    return result;
}
