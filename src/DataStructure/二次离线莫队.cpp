#include <bits/stdc++.h>

#define ll long long
#define lowbit(x) (x & (-x))
using namespace std;
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

const int N = 1e5 + 10;

int n, m, a[N], B, s[N];
int l[510], r[510], pos[N];

void add(int x, int t) {
    while (x <= n) {
        s[x] += t;
        x += lowbit(x);
    }
}

int query(int x) {
    int res = 0;
    while (x) {
        res += s[x];
        x -= lowbit(x);
    }
    return res;
}

struct Q {
    int l, r, id;
    bool operator<(const Q &W) const {
        if (pos[l] != pos[W.l])
            return pos[l] < pos[W.l];
        else
            return r < W.r;
    }
} q[N];

struct seg {
    int x, y, op, id;
};

vector<seg> lq[N], rq[N];

ll ans[N], g[N], f[N], tag[510], val[N], cnt[N];

void solve() {
    cin >> n >> m;
    vector<int> v;
    for (int i = 1; i <= n; i++) cin >> a[i], v.push_back(a[i]);

    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    for (int i = 1; i <= n; i++)
        a[i] = lower_bound(v.begin(), v.end(), a[i]) - v.begin() + 1;

    B = sqrt(n * 2);
    int tot = (n - 1) / B + 1;
    for (int i = 1; i <= tot; i++) l[i] = r[i - 1] + 1, r[i] = min(n, i * B);
    for (int i = 1; i <= tot; i++) {
        for (int j = l[i]; j <= r[i]; j++) pos[j] = i;
    }
    for (int i = 1; i <= m; i++) {
        cin >> q[i].l >> q[i].r;
        q[i].id = i;
    }

    for (int i = 1; i <= n; i++) {
        add(a[i], 1);
        f[i] = query(n) - query(a[i]);
        g[i] = query(a[i] - 1);
        f[i] += f[i - 1];
        g[i] += g[i - 1];
    }
    sort(q + 1, q + m + 1);

    for (int i = 1, l = 1, r = 0; i <= m; i++) {
        int id = q[i].id;
        if (l > q[i].l) {
            // l->q[i].l
            // [l,r] 中比 a[x] 小的  g(l,r,a[x])
            // g(1,r) - g(1,l-1) l-1
            ans[id] += g[q[i].l - 1] - g[l - 1];
            rq[r].push_back({q[i].l, l - 1, 1, id});
            l = q[i].l;
        }

        if (r < q[i].r) {
            // [l,r] 中比 a[x] 大的
            // f(1,r) - f(1,l-1)
            ans[id] += f[q[i].r] - f[r];
            lq[l - 1].push_back({r + 1, q[i].r, -1, id});
            r = q[i].r;
        }
        if (l < q[i].l) {
            ans[id] += g[q[i].l - 1] - g[l - 1];
            rq[r].push_back({l, q[i].l - 1, -1, id});
            l = q[i].l;
        }
        if (r > q[i].r) {
            ans[id] += f[q[i].r] - f[r];
            lq[l - 1].push_back({q[i].r + 1, r, 1, id});
            r = q[i].r;
        }
    }

    for (int i = 1; i <= n; i++) {
        cnt[a[i]]++;
        for (int j = pos[a[i]] + 1; j <= tot; j++) tag[j]++;
        for (int j = a[i] + 1; j <= r[pos[a[i]]]; j++) val[j]++;
        for (auto [x, y, op, id] : rq[i]) {
            for (int j = x; j <= y; j++)
                ans[id] += (val[a[j]] + tag[pos[a[j]]]) * op;
        }
        for (auto [x, y, op, id] : lq[i]) {
            for (int j = x; j <= y; j++) {
                ans[id] += (i - (val[a[j]] + tag[pos[a[j]]]) - cnt[a[j]]) * op;
            }
        }
    }
    q[0].id = 0;
    for (int i = 1; i <= m; i++) ans[q[i].id] += ans[q[i - 1].id];
    for (int i = 1; i <= m; i++) cout << ans[i] << '\n';
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int t = 1;
    while (t--) solve();
    return 0;
}