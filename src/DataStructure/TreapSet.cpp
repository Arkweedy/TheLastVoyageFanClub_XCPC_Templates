mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<typename T = pair<int,int>>
struct Treap
{
    struct Node{
        int ls, rs;
        u32 pri;
        T key;
        int sz;
        int cnt;
        // int res = 0;
        // int ktag = 0, rtag = 0;

        Node():ls(0),rs(0),pri(rng()),key(),sz(0),cnt(0){}
    };

    int rt = 0;
    vector<Node>t;

    Treap():t(1){}
    explicit Treap(size_t n):t(1){t.reserve(n);}

    int newNode()
    {
        t.emplace_back();
        return t.size() - 1;
    }

    int newNode(T key)
    {
        t.emplace_back();
        t.back().key = key;
        t.back().cnt = 1;
        t.back().sz = 1;
        return t.size() - 1;
    }

    int newNode(T key, int cnt)
    {
        t.emplace_back();
        t.back().key = key;
        t.back().cnt = cnt;
        t.back().sz = cnt;
        return t.size() - 1;
    }

    int sz(int p)const
    {
        return t[p].sz;
    }

    // void push(int p)
    // {
    //     if(!p)return;
    //     if(t[p].ktag == 0 && t[p].rtag == 0)return;
    //     if(t[p].ls)apply(t[p].ls, t[p].ktag, t[p].rtag);
    //     if(t[p].rs)apply(t[p].rs, t[p].ktag, t[p].rtag);
    //     t[p].ktag = t[p].rtag = 0;
    // }

    // void apply(int p, int a, int b)
    // {
    //     if(!p)return;
    //     t[p].key.first += a;
    //     t[p].ktag += a;
    //     t[p].res += b;
    //     t[p].rtag += b;
    // }

    void pull(int p)
    {
        if(!p)return;
        t[p].sz = t[p].cnt + sz(t[p].ls) + sz(t[p].rs);
        return;
    }

    int merge(int p1, int p2)
    {
        if(!p1 || !p2)return p1 ? p1 : p2;
        if(t[p1].key > t[p2].key)swap(p1,p2);
        if(t[p1].pri < t[p2].pri){
            // push(p1);
            t[p1].rs = merge(t[p1].rs, p2);
            pull(p1);
            return p1;
        }else{
            // push(p2);
            t[p2].ls = merge(p1,t[p2].ls);
            pull(p2);
            return p2;
        }
    }

    pair<int,int> split_less(int p, const T& key)
    {
        if(!p)return {0,0};
        // push(p);
        if(t[p].key < key){
            auto [l, r] = split_less(t[p].rs, key);
            t[p].rs = l;
            pull(p);
            return {p, r};
        }else{
            auto [l, r] = split_less(t[p].ls, key);
            t[p].ls = r;
            pull(p);
            return {l, p};
        }
    }

    pair<int,int> split_leq(int p, const T& key)
    {
        if(!p)return {0,0};
        // push(p);
        if(!(key < t[p].key)){
            auto [l, r] = split_leq(t[p].rs, key);
            t[p].rs = l;
            pull(p);
            return {p, r};
        }else{
            auto [l, r] = split_leq(t[p].ls, key);
            t[p].ls = r;
            pull(p);
            return {l, p};
        }
    }

    int unite(int a, int b)
    {
        if(!a || !b)return a ? a : b;
        if(t[a].pri > t[b].pri) swap(a, b);
        // push(a);
        // push(b);
        auto [x, z] = split_leq(b, t[a].key);
        auto [l, m] = split_less(x, t[a].key);
        if(m)t[a].cnt += t[m].cnt;
        t[a].ls = unite(t[a].ls, l);
        t[a].rs = unite(t[a].rs, z);
        pull(a);
        return a;
    }

    int insertRoot(int &root, T x)
    {
        auto [a, b] = split_leq(root, x);
        auto [l, m] = split_less(a, x);
        if(m)t[m].cnt++,pull(m);
        else m = newNode(x);
        root = merge(merge(l, m), b);
        return m;
    }

    void insertRoot(int &root, T x, int c)
    {
        auto [a, b] = split_leq(root, x);
        auto [l, m] = split_less(a, x);
        if(m)t[m].cnt += c, pull(m);
        else m = newNode(x, c);
        root = merge(merge(l, m), b);
        return;
    }

    void erase(int &root, T x)
    {
        auto [a, b] = split_leq(root, x);
        auto [l, m] = split_less(a, x);
        root = merge(l, b);
        return;
    }

    void extract(int &root, T x)
    {
        auto [a, b] = split_leq(root, x);
        auto [l, m] = split_less(a, x);
        if(t[m].cnt <= 1)root = merge(l, b);
        else{
            t[m].cnt--;
            t[m].sz--;
            root = merge(merge(l, m), b);
        }
        return;
    }

    int lower_bound(int &root, T x) //0 base
    {
        auto [a, b] = split_less(root, x);
        int res = sz(a);//0 base
        root = merge(a, b);
        return res;
    }

    int upper_bound(int &root, T x)
    {
        auto [a, b] = split_leq(root, x);
        int res = sz(a);//0 base
        root = merge(a, b);
        return res;
    }

    T kth(int &root, int k) // 1-base
    {
        int p = root;
        if(k <= 0 || k > sz(root))return T();
        while(1){
            // push(p);
            if(sz(t[p].ls) >= k)p = t[p].ls;
            else if(sz(t[p].ls) + t[p].cnt >= k)return t[p].key;
            else {k -= sz(t[p].ls) + t[p].cnt; p = t[p].rs;}
        }
    }

    T prev(int &root, T x)
    {
        auto [a, b] = split_less(root, x);
        T res = kth(a, sz(a));
        root = merge(a, b);
        return res;
    }

    T next(int &root, T x)
    {
        auto [a, b] = split_leq(root, x);
        T res = kth(b, 1);
        root = merge(a, b);
        return res;
    }

    //walk query
    // int lower_bound_walk(int root, T x)
    // {
    //     int p = root;
    //     int res = 0;
    //     while(p){
    //         // push(p);
    //         if(t[p].key < x){
    //             res += sz(t[p].ls) + t[p].cnt;
    //             p = t[p].rs;
    //         }else p = t[p].ls;
    //     }
    //     return res;
    // }

    // int upper_bound_walk(int root, T x)
    // {
    //     int p = root;
    //     int res = 0;
    //     while(p){
    //         // push(p);
    //         if(!(x < t[p].key)){
    //             res += sz(t[p].ls) + t[p].cnt;
    //             p = t[p].rs;
    //         }else p = t[p].ls;
    //     }
    //     return res;
    // }

    // T prev_walk(int root, T x)
    // {
    //     int p = root;
    //     T res{};
    //     while(p){
    //         // push(p);
    //         if(t[p].key < x){
    //             res = t[p].key;
    //             p = t[p].rs;
    //         }else p = t[p].ls;
    //     }
    //     return res;
    // }

    // T next_walk(int root, T x)
    // {
    //     int p = root;
    //     T res{};
    //     while(p){
    //         // push(p);
    //         if(x < t[p].key){
    //             res = t[p].key;
    //             p = t[p].ls;
    //         }else p = t[p].rs;
    //     }
    //     return res;
    // }

    void insert(T x)
    {
        insertRoot(rt, x);
    }

    void insert(T x, int c)
    {
        insertRoot(rt, x, c);
    }

    void erase(T x)
    {
        erase(rt, x);
    }

    void extract(T x)
    {
        extract(rt, x);
    }

    int lower_bound(T x)
    {
        return lower_bound(rt, x);
    }

    int upper_bound(T x)
    {
        return upper_bound(rt, x);
    }

    T prev(T x)
    {
        return prev(rt, x);
    }

    T next(T x)
    {
        return next(rt, x);
    }

    T kth(int x)
    {
        return kth(rt, x);
    }

    T operator[](int x) // 0-base
    {
        return kth(x + 1);
    }

};
