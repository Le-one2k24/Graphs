#include "boruvka.h"
#include "common_structures.h"
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
using namespace std;

static int** last_mst = nullptr;
static int last_mst_N = 0;

struct DSU {
    int n;
    vector<int> p;
    vector<int> r;
    DSU(int n=0): n(n), p(n), r(n,0) { for (int i=0;i<n;++i) p[i]=i; }
    int find(int x){ return p[x]==x?x:p[x]=find(p[x]); }
    bool unite(int a,int b){
        a=find(a); b=find(b);
        if (a==b) return false;
        if (r[a]<r[b]) swap(a,b);
        p[b]=a;
        if (r[a]==r[b]) r[a]++;
        return true;
    }
};

int** boruvka_mst(int** adj, int** weight, int N, int& total_weight) {
    total_weight = 0;
    if (!adj || !weight || N <= 0) return nullptr;

    int** mst = new int*[N];
    for (int i=0;i<N;++i){
        mst[i] = new int[N];
        for (int j=0;j<N;++j) mst[i][j] = 0;
    }

    DSU dsu(N);
    int components = N;
    total_weight = 0;

    while (components > 1) {
        const int INF_W = numeric_limits<int>::max();
        vector<int> best_u(N, -1);
        vector<int> best_v(N, -1);
        vector<int> best_w(N, INF_W);
        vector<int> best_id(N, numeric_limits<int>::max());

        for (int u = 0; u < N; ++u) {
            for (int v = 0; v < N; ++v) {
                if (u == v) continue;
                if (adj[u][v] == 0 && adj[v][u] == 0) continue;
                int w = weight[u][v];
                if (w == 0) continue;
                int cu = dsu.find(u);
                int cv = dsu.find(v);
                if (cu == cv) continue;
                int eid = u * N + v;
                if (w < best_w[cu] || (w == best_w[cu] && eid < best_id[cu])) {
                    best_w[cu] = w;
                    best_u[cu] = u;
                    best_v[cu] = v;
                    best_id[cu] = eid;
                }
            }
        }


        for (int i = 0; i < N; ++i) {
            if (best_u[i] == -1) continue;
            int u = best_u[i];
            int v = best_v[i];
            int cu = dsu.find(u);
            int cv = dsu.find(v);
            if (cu == cv) continue;

            if (dsu.unite(cu, cv)) {
                mst[u][v] = 1;
                mst[v][u] = 1;
                total_weight += weight[u][v];
                components--;
            }
        }
    }

    clear_last_mst();
    last_mst_N = N;
    last_mst = new int*[N];
    for (int i=0;i<N;++i){
        last_mst[i] = new int[N];
        for (int j=0;j<N;++j) last_mst[i][j] = mst[i][j];
    }

    return mst;
}

int** get_last_mst(int& N) {
    if (!last_mst) { N = 0; return nullptr; }
    N = last_mst_N;
    return last_mst;
}

void clear_last_mst() {
    if (!last_mst) return;
    for (int i=0;i<last_mst_N;++i) delete[] last_mst[i];
    delete[] last_mst;
    last_mst = nullptr;
    last_mst_N = 0;
}
