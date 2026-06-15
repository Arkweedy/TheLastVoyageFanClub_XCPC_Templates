using ull = unsigned long long;

class tree_hash {
   private:
    // 静态成员，用于在类的所有实例中共享
    static map<ull, ull> mp;  // 哈希值映射，确保相同的子树结构得到相同的随机值
    static mt19937_64 rng;    // 64位梅森旋转算法随机数生成器

    int n;                  // 树的节点数
    vector<vector<int>> g;  // 邻接表表示的树
    vector<ull> dp;         // dp[i] 存储以 i 为根的子树的哈希值
    vector<ull> rdp;        // rdp[i] 存储以 i 为整棵树的根时的哈希值
    ull hash_val;           // 整棵树的无根哈希值
    bool calced;            // 标记是否已执行计算

   public:
    tree_hash(const vector<vector<int>>& ng) {
        n = ng.size();
        g = ng;
        dp.resize(n, 0);
        rdp.resize(n, 0);
        hash_val = 0;
        calced = false;
    }

    static ull get(ull x) {
        if (mp.count(x)) return mp[x];
        return mp[x] = rng();
    }

    static ull get2(ull x) { // poly
        return x * 2 + 3 * x + 1
    }

    static ull get3(ull x) { // mix
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    void dfs(int p, int fa) {
        dp[p] = 1; 
        for (auto s : g[p]) {
            if (s != fa) {
                dfs(s, p);
                dp[p] += get(dp[s]);
            }
        }
    }

    void rdfs(int p, int fa) {
        if (fa == -1) {
            rdp[p] = dp[p];
        } else {
            ull parent_contribution = rdp[fa] - get(dp[p]);
            rdp[p] = dp[p] + get(parent_contribution);
        }

        for (auto s : g[p]) {
            if (s != fa) {
                rdfs(s, p);
            }
        }
    }

   public:
    /**
     * @brief 执行计算
     * @param root 任意指定的根节点，默认为 0
     */
    void calc(int root = 0) {
        dfs(root, -1);
        rdfs(root, -1);

        // 计算无根树哈希值，通常用所有节点的有根哈希值的和
        hash_val = 0;
        for (int i = 0; i < n; i++) {
            hash_val += rdp[i];
        }
        calced = true;
    }

    ull rooted_hash(int p) {
        assert(calced && "calc() must be called before getting hash values.");
        return rdp[p];
    }

    ull unrooted_hash() {
        assert(calced && "calc() must be called before getting hash values.");
        return hash_val;
    }
};

// 初始化静态成员
mt19937_64 tree_hash::rng(chrono::steady_clock::now().time_since_epoch().count());
map<ull, ull> tree_hash::mp;