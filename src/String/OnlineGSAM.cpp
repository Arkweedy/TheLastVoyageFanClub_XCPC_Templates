struct OnlineGSAM {
    static constexpr int M = 26;
    static constexpr int root = 1;

    struct Node {
        int len = 0, link = 0;
        array<int, M> nxt{};
    };
    vector<Node> t;

    OnlineGSAM() { init(); }

    void init() {
        t.assign(2, Node{});
        t[0].len = -1;
        t[0].nxt.fill(root);
    }

    int newNode() {
        t.emplace_back();
        return (int)t.size() - 1;
    }

    // p 已有 x 转移，返回代表长度 len[p] + 1 的状态
    int split(int p, int x) {
        int q = t[p].nxt[x];
        if (t[q].len == t[p].len + 1) return q;

        int clone = newNode();
        t[clone] = t[q];
        t[clone].len = t[p].len + 1;
        t[q].link = clone;
        while (p != 0 && t[p].nxt[x] == q) {
            t[p].nxt[x] = clone;
            p = t[p].link;
        }
        return clone;
    }

    int extend(int last, int x) {
        if (t[last].nxt[x] != 0) return split(last, x);

        int cur = newNode();
        t[cur].len = t[last].len + 1;
        int p = last;
        while (p != 0 && t[p].nxt[x] == 0) {
            t[p].nxt[x] = cur;
            p = t[p].link;
        }
        int suffix = split(p, x);
        t[cur].link = suffix;
        return cur;
    }

    void insert(const string& s) {
        int last = root;
        for (char c : s) last = extend(last, c - 'a');
    }
};
