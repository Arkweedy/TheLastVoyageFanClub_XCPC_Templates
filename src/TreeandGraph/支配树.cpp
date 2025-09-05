#include <algorithm>
#include <iostream>
#include <vector>

const int MAXN = 200005;

std::vector<int> G[MAXN], R[MAXN], son[MAXN];
int ufs[MAXN];
int idom[MAXN], sdom[MAXN], anc[MAXN];
int pr[MAXN], dfn[MAXN], id[MAXN], stamp;

int findufs(int x) {
    if (ufs[x] == x) return x;
    int t = ufs[x];
    ufs[x] = findufs(ufs[x]);
    if (dfn[sdom[anc[x]]] > dfn[sdom[anc[t]]]) anc[x] = anc[t];
    return ufs[x];
}

void dfs(int x) {
    dfn[x] = ++stamp;
    id[stamp] = x;
    sdom[x] = x;
    for (int y : G[x])
        if (!dfn[y]) {
            pr[y] = x;
            dfs(y);
        }
}

void get_dominator(int n) {
    // 构建反图 R
    for (int i = 1; i <= n; i++) {
        for (int neighbor : G[i]) {
            if (dfn[i] && dfn[neighbor])  // 只处理从起点可达的边
                R[neighbor].push_back(i);
        }
    }

    for (int i = 1; i <= n; i++) ufs[i] = anc[i] = i;

    // Lengauer-Tarjan 算法核心
    for (int i = n; i > 1; i--) {
        int x = id[i];
        if (!x) continue;  // 节点不可达
        for (int y : R[x]) {
            if (dfn[y]) {
                findufs(y);
                if (dfn[sdom[x]] > dfn[sdom[anc[y]]]) sdom[x] = sdom[anc[y]];
            }
        }
        son[sdom[x]].push_back(x);
        ufs[x] = pr[x];
        if (pr[x]) {
            for (int u : son[pr[x]]) {
                findufs(u);
                idom[u] = (sdom[u] == sdom[anc[u]] ? pr[x] : anc[u]);
            }
            son[pr[x]].clear();
        }
    }

    // 最后的修正
    for (int i = 2; i <= n; i++) {
        int x = id[i];
        if (!x) continue;
        if (idom[x] != sdom[x]) idom[x] = idom[idom[x]];
    }
}
// --- 模板结束 ---

// --- 解决问题的代码 ---

// ans[i] 用于存储以 i 为根的子树大小
int ans[MAXN];

int main() {
    // 加速输入输出
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        G[u].push_back(v);
    }

    // 1. 从根节点1开始DFS，为建支配树做准备
    // stamp 会记录从1出发能到达的节点数
    dfs(1);

    // 2. 调用模板构建支配树
    get_dominator(stamp);

    // 3. 计算每个节点的子树大小
    // 初始化，每个节点的子树大小至少是1（它自己）
    for (int i = 1; i <= stamp; ++i) {
        ans[id[i]] = 1;
    }

    // 按照DFS序的逆序（id数组的倒序）进行遍历
    // 这样可以保证在计算一个节点时，它的所有子节点都已被计算过
    for (int i = stamp; i > 1; --i) {
        int u = id[i];         // 当前节点
        int p = idom[u];       // u在支配树中的父节点
        if (p) {               // 根节点没有父节点
            ans[p] += ans[u];  // 将子节点的子树大小累加到父节点上
        }
    }

    // 4. 输出结果 (每个点能支配的点数)
    for (int i = 1; i <= n; ++i) {
        // 如果节点 i 从 1 出发不可达，则dfn[i]为0，它支配的点只有自己
        if (dfn[i]) {
            std::cout << ans[i] << " ";
        } else {
            std::cout << 0 << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}