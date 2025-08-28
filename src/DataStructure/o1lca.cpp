int dfn[N], mi[19][N], tot;
vector<int> e[N];
int get(int x, int y) { return dfn[x] < dfn[y] ? x : y; }
void dfs(int u, int f) {
    dfn[u] = ++tot;
    mi[0][dfn[u]] = f;
    for (int v : e[u])
        if (v != f) dfs(v, u);
}
int lca(int u, int v) {
    if (u == v) return u;
    u = dfn[u], v = dfn[v];
    if (u > v) swap(u, v);
    int d = __lg(v - u);
    u++;
    return get(mi[d][u], mi[d][v - (1 << d) + 1]);
}
