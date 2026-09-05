#include<bits/stdc++.h>
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;
using i128 = __int128;

using namespace std;

//P3834.cpp Create time : 2026.02.08 23:34
struct segtree
{
    int n;
    int vc;
    vector<int>a;
    vector<int>rt, ls, rs;
    vector<int>t;
    

    segtree(int n_):n(n_), vc(0)
    {
        rt.reserve(n * 5);
        ls.reserve(n * 30);
        rs.reserve(n * 30);
        t.reserve(n * 30);
    }

    segtree(int _n, vector<int> _a):n(_n),vc(0), a(_a)
    {
        rt.reserve(n);
        ls.reserve(n * 20);
        rs.reserve(n * 20);
        t.reserve(n * 20);
        int p = newNode();
        newVersion(p);
        build(0, n - 1, rt[0]);
    }

    void build(int l, int r, int p)
    {
        if(l == r){
            t[p] = a[l];
            return;
        }
        int m = l + r >> 1;
        ls[p] = newNode();
        rs[p] = newNode();
        build(l, m, ls[p]);
        build(m + 1, r, rs[p]);
        push_up(p);
        return;
    }

    void newVersion(int root)
    {
        rt.push_back(root);
        vc++;
    }

    int newNode()
    {
        ls.emplace_back();
        rs.emplace_back();
        t.emplace_back();
        return t.size() - 1;
    }

    int query(int ver, int x)
    {
        return query(x, 0, n - 1, rt[ver]);
    }

    int query(int x, int l, int r, int p)
    {
        if(l == r)return t[p];
        int m = l + r >> 1;
        if(x <= m) return query(x, l, m, ls[p]);
        else return query(x, m + 1, r, rs[p]);
    }

    void apply(int ver, int x, int val)
    {
        newVersion(apply(x, val, 0, n - 1, rt[ver]));
        return;
    }

    void push_up(int p)
    {
        t[p] = t[ls[p]] + t[rs[p]];
    }

    int apply(int x, int val, int l, int r, int p)
    {
        if(l == r){
            int np = newNode();
            t[np] = t[p] + val;
            return np;
        }
        int m = l + r >> 1;
        int np = newNode();
        if(x <= m){
            int s = apply(x, val, l, m, ls[p]);
            ls[np] = s;
            rs[np] = rs[p];
        }
        else{
            int s = apply(x, val, m + 1, r, rs[p]);
            ls[np] = ls[p];
            rs[np] = s;
        }
        push_up(np);
        return np;
    }

    int kth(int l, int r, int k)
    {
        int lp = rt[l], rp = rt[r + 1];
        return binary_find(lp, rp, 0, n - 1, k);
    }

    int binary_find(int lp, int rp,int l, int r, int k)
    {
        if(l == r)return l;
        int m = l + r >> 1;
        int lc = t[ls[rp]] - t[ls[lp]];
        if(lc >= k)return binary_find(ls[lp], ls[rp], l, m, k);
        else return binary_find(rs[lp],rs[rp], m + 1, r, k - lc);
    }
};


void solve()
{
    int n, q;
    cin >> n >> q;
    vector<int>a(n);
    for(int i = 0;i < n;i++){
        cin >> a[i];
    }
    auto b = a;
    sort(b.begin(),b.end());
    b.erase(unique(b.begin(),b.end()),b.end());
    for(int i = 0;i < n;i++){
        a[i] = lower_bound(b.begin(),b.end(), a[i]) - b.begin();
    }

    segtree t(n, vector<int>(n, 0));
    for(int i = 0;i < n;i++){
        t.apply(i, a[i], 1);
    }

    while(q--){
        int l, r, k;
        cin >> l >> r >> k;
        l--,r--;
        cout << b[t.kth(l, r, k)] << "\n";
    }
    return;
}

int main()
{
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    int tt = 1;
    //cin >> tt;
    while(tt--){
        solve();
    }
    return 0;
}
