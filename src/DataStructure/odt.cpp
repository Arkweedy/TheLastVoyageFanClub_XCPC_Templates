struct ODT {
    struct Node_t {
        int l, r;
        mutable int v;
        // mutable关键字允许在 const 成员函数或 const对象中修改

        Node_t(const int &il, const int &ir, const int &iv)
            : l(il), r(ir), v(iv) {}

        bool operator<(const Node_t &o) const { return l < o.l; }
    };

    set<Node_t> s;
    /*
    将原本包含点 x 的区间（设为 [l, r]）分裂为两个区间 [l, x) 和
    [x,r]，并返回指向后者的迭代器。
    */
    auto split(int x) {
        auto it = s.lower_bound(Node_t(x, 0, 0));
        if (it != s.end() && it->l == x) return it;
        --it;
        int l = it->l, r = it->r, v = it->v;
        s.erase(it);
        s.insert(Node_t(l, x - 1, v));
        return s.insert(Node_t(x, r, v)).first;
    }

    // 区间赋值
    void assign(int l, int r, int v) {
        auto itr = split(r + 1);  // 必须先分裂 r + 1 否则迭代器可能失效。
        auto itl = split(l);
        s.erase(itl, itr);
        s.insert(Node_t(l, r, v));
    }

    /*
    提取一段区间进行操作。
    要保证均摊复杂度需要在调用 perform(l,r) 后立刻 assign(l,r,v)
    */
    void perform(int l, int r) {
        auto itr = split(r + 1), itl = split(l);
        for (; itl != itr; ++itl) {
                }
    }
};