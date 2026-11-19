#include <utility>
#include <vector>

// 动态森林，点编号 1..n，0 为空点。
class ForestLCT {
    struct Node {
        int fa = 0, ch[2]{};
        bool rev = false;
    };
    std::vector<Node> t;

    int dir(int x) const { // 左儿子 0，右儿子 1，辅助树根 -1
        const auto& ch = t[t[x].fa].ch;
        if (ch[0] == x) return 0;
        return ch[1] == x ? 1 : -1;
    }
    void flip(int x) {
        if (!x) return;
        std::swap(t[x].ch[0], t[x].ch[1]);
        t[x].rev ^= 1;
    }
    void push(int x) {
        if (!t[x].rev) return;
        flip(t[x].ch[0]);
        flip(t[x].ch[1]);
        t[x].rev = false;
    }
    void rotate(int x) {
        int y = t[x].fa, z = t[y].fa, d = dir(x), k = dir(y);
        int b = t[x].ch[d ^ 1];
        if (k != -1) t[z].ch[k] = x;
        t[x].fa = z;
        t[y].ch[d] = b;
        if (b) t[b].fa = y;
        t[x].ch[d ^ 1] = y;
        t[y].fa = x;
    }
    void splay(int x) {
        while (dir(x) != -1) {
            int y = t[x].fa;
            bool twice = dir(y) != -1;
            if (twice) push(t[y].fa);
            push(y);
            push(x);
            if (twice) rotate(dir(x) == dir(y) ? y : x);
            rotate(x);
        }
        push(x);
    }
    void access(int x) {
        int last = 0;
        for (int p = x; p; p = t[p].fa) {
            splay(p);
            t[p].ch[1] = last;
            last = p;
        }
        splay(x);
    }
    void split(int u, int v) {
        access(u);
        flip(u);
        access(v);
    }

public:
    explicit ForestLCT(int n) : t(n + 1) {}
    bool connected(int u, int v) {
        split(u, v);
        return u == v || t[u].fa != 0;
    }
    bool link(int u, int v) {
        if (connected(u, v)) return false;
        t[u].fa = v; // 此时 u 已换根
        return true;
    }
    bool cut(int u, int v) {
        split(u, v);
        if (t[v].ch[0] != u || t[u].ch[1]) return false;
        t[v].ch[0] = t[u].fa = 0;
        return true;
    }
};
