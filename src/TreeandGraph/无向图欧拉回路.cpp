vector<int> res;
int h[N], e[N << 1], ne[N << 1], idx = 2, cur[N];
void add(int u, int v) { e[idx] = v, ne[idx] = h[u], h[u] = idx++; }

bool vis[N];
void dfs(int x) {
    for (int& i = cur[x]; i;) {
        if (!vis[i]) {
            vis[i] = vis[i ^ 1] = 1;
            dfs(e[i]);
        }
        i = ne[i];
    }
    res.push_back(x);
}