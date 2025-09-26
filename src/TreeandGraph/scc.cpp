struct SCC {
    int n;
    vector<vector<int> > e;
    vector<int> stk;
    vector<int> dfn, low, bel;
    int cur = 0, cnt = 0;  // 标号，强连通分量个数
    SCC() {}
    SCC(int n) : n(n), e(n), dfn(n, -1), low(n), bel(n, -1) {}
    void add(int u, int v) { e[u].push_back(v); }

    void dfs(int x) {
        dfn[x] = low[x] = cur++;
        stk.push_back(x);

        for (auto y : e[x]) {
            if (dfn[y] == -1) {
                dfs(y);
                low[x] = min(low[x], low[y]);
            } else if (bel[y] == -1) {
                low[x] = min(low[x], dfn[y]);
            }
        }
        if (dfn[x] == low[x]) {
            int y;
            do {
                y = stk.back();
                bel[y] = cnt;
                stk.pop_back();
            } while (y != x);
            cnt++;
        }
    }
    void build() {
        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) {
                dfs(i);
            }
        }
    }

    void init(int n) {
        this->n = n;
        e.assign(n, {});
        dfn.assign(n, -1);
        low.resize(n);
        bel.assign(n, -1);
        stk.clear();
        cur = cnt = 0;
    }
};
