// 边 i -> a[i]
int n, a[N], deg[N];
int id[N];           // id[u] 表示 u 在环上离散化后的编号 0-base
int be[N];           // be[u] 表示 u 所在环的编号
int len[N];          // len[i] 表示 编号为 i 的环长度
int rt[N];           // rt[u] 表示 u 所在树的根
bool st[N], vis[N];  // st[u] = 1 表示 u 不在环上
vector<int> e[N];

void dfs(int u, int r) {
    rt[u] = r;
    for (auto v : e[u]) {
        dfs(v, r);
    }
}

void solve() {
    for (int i = 1; i <= n; i++) {
        e[i].clear(), st[i] = vis[i] = deg[i] = 0;
    }
    for (int i = 1; i <= n; i++) {
        deg[a[i]]++;
    }

    queue<int> q;
    for (int i = 1; i <= n; i++)
        if (deg[i] == 0) q.push(i);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        st[u] = 1;
        deg[a[u]]--;
        if (deg[a[u]] == 0) q.push(a[u]);
        e[a[u]].push_back(u);
    }

    int pos = 0;
    for (int i = 1; i <= n; i++) {
        if (st[i]) continue;
        dfs(i, i);
        if (vis[i]) continue;
        pos++;
        int p = i, tot = 0;
        do {
            be[p] = pos;
            id[p] = tot++;
            vis[p] = 1;
            p = a[p];
        } while (p != i);
        len[pos] = tot;
    }
}
