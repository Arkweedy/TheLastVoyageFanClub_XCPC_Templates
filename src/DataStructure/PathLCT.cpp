#include <algorithm>
#include <optional>
#include <vector>

struct LCTEmpty {
    void pull(const LCTEmpty&, const LCTEmpty&) {}
    void flip() {}
};

// 点编号 1..n，0 为空节点。LCT<> 只维护森林连通性。
template<class Info = LCTEmpty>
struct LCT {
protected:
    struct Node {
        int fa = 0, ch[2]{};
        bool rev = false;
        Info info{};
    };
    std::vector<Node> t;

    void pull(int x) {
        const auto& ch = t[x].ch;
        t[x].info.pull(t[ch[0]].info, t[ch[1]].info);
    }

private:
    std::vector<int> stk;

    bool isRoot(int x) const {
        const auto& ch = t[t[x].fa].ch;
        return ch[0] != x && ch[1] != x;
    }
    int side(int x) const { return t[t[x].fa].ch[1] == x; }

    void flip(int x) {
        if (!x) return;
        std::swap(t[x].ch[0], t[x].ch[1]);
        t[x].rev ^= 1;
        t[x].info.flip();
    }
    void push(int x) {
        if (!t[x].rev) return;
        flip(t[x].ch[0]);
        flip(t[x].ch[1]);
        t[x].rev = false;
    }
    void rotate(int x) {
        int y = t[x].fa, z = t[y].fa, d = side(x);
        int b = t[x].ch[d ^ 1];
        if (!isRoot(y)) t[z].ch[side(y)] = x;
        t[x].fa = z;
        t[y].ch[d] = b;
        if (b) t[b].fa = y;
        t[x].ch[d ^ 1] = y;
        t[y].fa = x;
        pull(y);
        pull(x);
    }
    void splay(int x) {
        stk.clear();
        for (int p = x; ; p = t[p].fa) {
            stk.push_back(p);
            if (isRoot(p)) break;
        }
        for (auto it = stk.rbegin(); it != stk.rend(); ++it) push(*it);
        while (!isRoot(x)) {
            int y = t[x].fa;
            if (!isRoot(y)) rotate(side(x) == side(y) ? y : x);
            rotate(x);
        }
    }

protected:
    void access(int x) {
        int last = 0;
        for (int p = x; p; p = t[p].fa) {
            splay(p);
            t[p].ch[1] = last; // 原右儿子保留 fa，转为虚边
            pull(p);
            last = p;
        }
        splay(x);
    }
    bool exposePath(int u, int v) {
        access(u);
        flip(u); // 换根到 u
        access(v);
        // 连通时 v 是整条路径的辅助树根；否则 u 仍是另一棵树的根。
        return u == v || t[u].fa != 0;
    }

public:
    explicit LCT(int n = 0) { init(n); }
    void init(int n) {
        t.assign(n + 1, Node{});
        stk.clear();
    }
    bool connected(int u, int v) { return exposePath(u, v); }
    bool link(int u, int v) {
        if (exposePath(u, v)) return false;
        t[u].fa = v;
        return true;
    }
    bool cut(int u, int v) {
        if (!exposePath(u, v)) return false;
        if (t[v].ch[0] != u || t[u].ch[1]) return false;
        t[v].ch[0] = t[u].fa = 0;
        pull(v);
        return true;
    }
};

// Op::join(a, b) 表示先经过 a 再经过 b，Op::id() 为空路径的单位元。
template<class Op>
struct LCTPathInfo {
    using T = typename Op::T;
    T val = Op::id(), fwd = Op::id(), bwd = Op::id();

    void pull(const LCTPathInfo& l, const LCTPathInfo& r) {
        fwd = Op::join(Op::join(l.fwd, val), r.fwd);
        bwd = Op::join(Op::join(r.bwd, val), l.bwd);
    }
    void flip() { std::swap(fwd, bwd); }
};

template<class Op>
struct PathLCT : LCT<LCTPathInfo<Op>> {
    using Base = LCT<LCTPathInfo<Op>>;
    using Base::Base;
    using T = typename Op::T;

    void set(int u, const T& val) {
        this->access(u);
        this->t[u].info.val = val;
        this->pull(u);
    }
    // 两端都计入；不连通返回 nullopt。
    std::optional<T> pathQuery(int u, int v) {
        if (!this->exposePath(u, v)) return std::nullopt;
        return this->t[v].info.fwd;
    }
};

struct XorOp {
    using T = long long;
    static T id() { return 0; }
    static T join(T a, T b) { return a ^ b; }
};
