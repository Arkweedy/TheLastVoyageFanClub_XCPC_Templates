mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<typename T>
struct Treap
{
    struct Node{
        int ls, rs;
        u32 pri;
        T val;
        int sz;
        int fa;

        //sums
        // i64 w = 0, sum = 0;

        //tags
        // int rev = 0;

        Node():ls(0),rs(0),pri(rng()),val(),sz(0),fa(0){};
    };

    int rt = 0;
    vector<Node>t;

    Treap():t(1){};

    int newNode()
    {
        t.emplace_back();
        return t.size() - 1;
    }

    int newNode(T val)
    {
        t.emplace_back();
        t.back().val = val;
        t.back().sz = 1;
        // t.back().w = 1;
        // t.back().sum = 1;
        return t.size() - 1;
    }

    // int newNode(T val, i64 w)
    // {
    //     t.emplace_back();
    //     t.back().val = val;
    //     t.back().sz = 1;
    //     t.back().w = w;
    //     t.back().sum = w;
    //     return t.size() - 1;
    // }

    int sz(int p)const
    {
        return t[p].sz;
    }

    // i64 sum(int p)const
    // {
    //     return t[p].sum;
    // }

    void setfa(int s, int p)
    {
        if(s)t[s].fa = p;
    }

    // void push(int p)
    // {
    //     if(p && t[p].rev){
    //         swap(t[p].ls, t[p].rs);
    //         add_tag(t[p].ls);
    //         add_tag(t[p].rs);
    //         t[p].rev = 0;
    //     }
    // }

    // void push_path(int p)
    // {
    //     static stack<int> stk;
    //     while(p){
    //         stk.push(p);
    //         p = t[p].fa;
    //     }
    //     while(!stk.empty()){
    //         push(stk.top());
    //         stk.pop();
    //     }
    // }

    void pull(int p)
    {
        if(!p)return;
        t[p].sz = sz(t[p].ls) + sz(t[p].rs) + 1;
        // t[p].sum = sum(t[p].ls) + t[p].w + sum(t[p].rs);
        setfa(t[p].ls, p);
        setfa(t[p].rs, p);
    }

    // void add_tag(int p)
    // {
    //     if(p)t[p].rev ^= 1;
    // }

    int merge(int l, int r) // [l, r]
    {
        if(!l || !r){
            int p = l ? l : r;
            setfa(p, 0);
            return p;
        }
        if(t[l].pri < t[r].pri){
            // push(l);
            t[l].rs = merge(t[l].rs, r);
            pull(l);
            setfa(l, 0);
            return l;
        }else{
            // push(r);
            t[r].ls = merge(l, t[r].ls);
            pull(r);
            setfa(r, 0);
            return r;
        }
    }

    pair<int,int> split(int p, int rk) //rk base 0, split to [0, rk], [rk + 1, sz - 1]
    {
        if(!p)return {0,0};
        if(rk < 0){
            setfa(p, 0);
            return {0, p};
        }
        if(rk >= sz(p) - 1){
            setfa(p, 0);
            return {p, 0};
        }
        // push(p);
        if(sz(t[p].ls) + 1 <= rk + 1){
            auto [l, r] = split(t[p].rs, rk - sz(t[p].ls) - 1);
            t[p].rs = l;
            pull(p);
            setfa(p, 0);
            setfa(r, 0);
            return {p, r};
        }else{
            auto [l, r] = split(t[p].ls, rk);
            t[p].ls = r;
            pull(p);
            setfa(l, 0);
            setfa(p, 0);
            return {l, p};
        }
    }

    void push_back(T val)
    {
        int c = newNode(val);
        rt = merge(rt, c);
        return;
    }

    void insert(T val, int rk) // insert as rk , base 0
    {
        auto [l, r] = split(rt, rk);
        int c = newNode(val);
        rt = merge(merge(l, c), r);
        return;
    }

    void erase(int rk){
        auto [c, d] = split(rt, rk);
        auto [a, b] = split(c, rk - 1); // b is single node
        rt = merge(a, d);
    }

    void erase(int l, int r){
        if(l > r) return;
        auto [c, d] = split(rt, r);
        auto [a, b] = split(c, l - 1); // b is the range
        rt = merge(a, d);
    }

    // void apply(int l, int r)
    // {
    //     if(r < l)return;
    //     auto [c, d] = split(rt, r);
    //     auto [a, b] = split(c, l - 1);
    //     add_tag(b);
    //     rt = merge(merge(a, b), d);
    // }

    T query(int rk)//base 0
    {
        auto [c, d] = split(rt, rk);
        auto [a, b] = split(c, rk - 1);
        T res = t[b].val;
        rt = merge(merge(a, b), d);
        return res;
    }

    // pair<int,i64> kth_sum(int p, i64 k)//k 1-base, offset 0-base
    // {
    //     while(1){
    //         // push(p);
    //         i64 left = sum(t[p].ls);
    //         if(k <= left)p = t[p].ls;
    //         else if(k <= left + t[p].w)return {p, k - left - 1};
    //         else{
    //             k -= left + t[p].w;
    //             p = t[p].rs;
    //         }
    //     }
    // }

    // pair<int,i64> kth_sum(i64 k)
    // {
    //     return kth_sum(rt, k);
    // }

    int rank(int p)// node i rank
    {
        if(!p)return -1;
        // push_path(p);
        int res = sz(t[p].ls) + 1;
        while(p != 0){
            if(t[t[p].fa].rs == p)res += sz(t[t[p].fa].ls) + 1;
            p = t[p].fa;
        }
        return res - 1;
    }

};
