#include<bits/stdc++.h>
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;
using i128 = __int128;

using namespace std;

//DCtree.cpp Create time : 2026.11.11 18:23



struct DivideCombineTree{
    struct node{
        int l, r, mi, ma, fa;
        int type;// 0 prime 1 inc 2 dec -1 leaf
        vector<int>son;
        node(){}
        node(int a, int b, int c, int d, int f):l(a),r(b),mi(c),ma(d),fa(-1),type(f),son{}{}
    };

    int n;
    vector<node>t;

    struct segtree // f(l) = max {l, r} - min{l, r} - (r - l) >= 0 , f(l) = 0 -> Consecutive
    {
        int n;
        vector<int>tr;
        vector<int>lz;

        segtree(int n_)
        {
            n = n_;
            tr.resize(n * 4, 0);
            lz.resize(n * 4, 0);
        };

        void pull(int p)
        {
            tr[p] = min(tr[p * 2], tr[p * 2 + 1]);
        }

        void push(int p)
        {
            if(lz[p] != 0){
                tr[p * 2] += lz[p];
                tr[p * 2 + 1] += lz[p];
                lz[p * 2] += lz[p];
                lz[p * 2 + 1] += lz[p];
                lz[p] = 0;
            }
        }

        void build(int l, int r, int p)
        {
            if(l == r){
                return;
            }
            int m = l + r >> 1;
            build(l, m, p * 2);
            build(m + 1, r, p * 2 + 1);
            pull(p);
        }

        void add(int s, int t, int v, int l, int r, int p)
        {
            if(s <= l && r <= t){
                tr[p] += v;
                lz[p] += v;
                return;
            }
            int m = l + r >> 1;
            push(p);
            if(s <= m)add(s, t, v, l, m, p * 2);
            if(t > m)add(s, t, v, m + 1, r, p * 2 + 1);
            pull(p);
        }

        int findlast(int R, int l, int r, int p)// [0, R] last 0
        {
            if(l > R || tr[p] > 0)return -1;
            if(l == r)return l;
            int m = l + r >> 1;
            push(p);
            int res =  findlast(R, m + 1, r, p * 2 + 1);
            if(res != -1)return res;
            else return findlast(R, l, m, p * 2);
        }

        int findlast(int R)
        {
            return findlast(R, 0, n - 1, 1);
        }

        void add(int l, int r, int v)
        {
            if(l > r)return;
            add(l, r, v, 0, n - 1, 1);
        }
    };

    void addson(int u, int v)
    {
        t[u].son.push_back(v);
        t[v].fa = u;
        t[u].mi = min(t[u].mi, t[v].mi);
        t[u].ma = max(t[u].ma, t[v].ma);
        //t[u].l = min(t[u].l, t[v].l);     only expand right
        t[u].r = t[v].r;
    }


    void build(vector<int>&a)
    {
        int n = a.size();
        t.reserve(n * 2);
        int rt = -1;
        for(int i = 0;i < n;i++){
            t.emplace_back(i, i, a[i], a[i], -1);
        }

        vector<int>stmin, stmax;
        segtree sgt(n);
        vector<int>stk;

        for(int i = 0;i < n;i++){
            sgt.add(0, i - 1, -1);
            int R = i;
            while(!stmin.empty() && a[stmin.back()] > a[i]){
                int x = stmin.back();
                stmin.pop_back();
                int L = stmin.empty() ? 0 : stmin.back() + 1;
                sgt.add(L, R - 1, a[x]); // 撤掉旧的 -min 贡献
                R = L;
            }
            sgt.add(R, i, -a[i]); // 统一加入新的 -min 贡献
            stmin.push_back(i);

            R = i;
            while(!stmax.empty() && a[stmax.back()] < a[i]){
                int x = stmax.back();
                stmax.pop_back();
                int L = stmax.empty() ? 0 : stmax.back() + 1;
                sgt.add(L, R - 1, -a[x]); // 撤掉旧的 +max 贡献
                R = L;
            }
            sgt.add(R, i, a[i]); // 统一加入新的 +max 贡献
            stmax.push_back(i);

            int p = i;
            while(!stk.empty()){
                int q = stk.back();
                
                // check as son
                if((t[q].type == 1) && (t[q].ma + 1 == t[p].mi)){
                    stk.pop_back();
                    addson(q, p);
                    p = q;
                    continue;
                }
                else if((t[q].type == 2) && (t[q].mi - 1 == t[p].ma)){
                    stk.pop_back();
                    addson(q, p);
                    p = q;
                    continue;
                }
                
                // check 
                int L = sgt.findlast(t[p].l - 1);
                if(L == -1)break;
                int type = 0;
                if(L == t[q].l)type = t[q].mi < t[p].mi ? 1 : 2;

                int k = stk.size() - 1;
                while(t[stk[k]].l > L)k--;
                t.emplace_back(L, t[p].r, t[p].mi, t[p].ma, type);
                int np = t.size() - 1;
                for(int j = k;j < stk.size();j++){
                    addson(np, stk[j]);
                }
                stk.resize(k);
                addson(np, p);
                p = np;
            }
            stk.push_back(p);
        }
    }  
};

void solve()
{
    
}

int main()
{
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    int tt = 1;
    cin >> tt;
    while(tt--){
        solve();
    }
    return 0;
}
