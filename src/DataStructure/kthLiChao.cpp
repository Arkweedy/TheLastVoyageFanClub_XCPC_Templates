// 第 k 大 (可以修改两次变为第 k 小)
struct line {
    ll a, t;
    ll get(ll x) { return a * x + t; }
};

struct LiChao {
    int n;
    int k;
    vector<vector<line>> a;
    LiChao() {}
    LiChao(int n, int k) { init(n, k) };
    void init(int n, int k) {
        this->n = n;
        this->k = k;
        a.assign(4 * n, {});
    }
    void add(int p, int l, int r, line x) {
        if (a[p].size() < 2 * k - 1) {
            a[p].push_back(x);
            return;
        }
        int mid = (l + r) / 2;

        a[p].push_back(x);

        nth_element(a[p].begin(), a[p].begin() + 2 * k - 2, a[p].end(),
                    [&](line a, line b) {
                        return a.get(mid) > b.get(mid);
                    });  // 可以修改符号

        line y = a[p].back();
        a[p].pop_back();

        if (l == r) {
            return;
        }

        int cntl = 0, cntr = 0;
        for (auto t : a[p]) {
            int L = y.get(l) > t.get(l);  // 可以修改符号
            cntl += L, cntr += L ^ 1;
        }

        if (cntl >= cntr) {
            add(2 * p, l, mid, y);
        } else {
            add(2 * p + 1, mid + 1, r, y);
        }
    }

    void get(int p, int l, int r, int x, vector<ll> &rep) {
        for (auto i : a[p]) {
            rep.push_back(i.get(x));
        }
        if (l == r) {
            return;
        }
        int mid = (l + r) / 2;
        if (x <= mid) {
            get(2 * p, l, mid, x, rep);
        } else {
            get(2 * p + 1, mid + 1, r, x, rep);
        }
    }

    ll query(int val, int k) {
        vector<ll> rep;
        get(1, 1, n, val, rep);
        nth_element(rep.begin(), rep.begin() + k - 1, rep.end(),
                    [](ll x, ll y) { return x > y; });
        if (rep.size() >= k)
            return rep[k - 1];
        else
            return 0;
    }
};