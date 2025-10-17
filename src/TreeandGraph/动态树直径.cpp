#include <bits/stdc++.h>

#define ll long long
#define ln (p << 1)
#define rn (p << 1 | 1)
using namespace std;
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

const int N = 2e5 + 10;

int eu[N], n, q, tot, be[N], in[N], out[N];
ll w, val[N];
vector<array<ll, 3>> e[N];

/*
u <= x <= v
d[u] + d[v] - 2 d[x]
*/

struct info {
    ll mi, mx;
    ll ux, vx;
    ll ans;

    info(ll x = 0) {
        mi = mx = x;
        ux = vx = -x;
        ans = 0;
    }
} tr[N << 2];

ll tag[N << 2];

void opt(int p, ll x) {
    tr[p].mi += x, tr[p].mx += x;
    tr[p].ux -= x, tr[p].vx -= x;
    tag[p] += x;
}

void down(int p) {
    if (tag[p]) {
        opt(ln, tag[p]), opt(rn, tag[p]);
        tag[p] = 0;
    }
}

info operator+(const info& a, const info& b) {
    info c;
    c.mi = min(a.mi, b.mi);
    c.mx = max(a.mx, b.mx);
    c.ux = max({a.ux, b.ux, a.mx - 2 * b.mi});
    c.vx = max({a.vx, b.vx, b.mx - 2 * a.mi});
    c.ans = max({a.ans, b.ans, a.mx + b.vx, b.mx + a.ux});
    return c;
}

void modify(int p, int l, int r, int x, ll v) {
    if (l == r) {
        tr[p] = info(v);
        return;
    }
    down(p);
    int mid = l + r >> 1;
    if (x > mid)
        modify(rn, mid + 1, r, x, v);
    else
        modify(ln, l, mid, x, v);
    tr[p] = tr[ln] + tr[rn];
}

void change(int p, int l, int r, int x, int y, ll v) {
    if (l > y || r < x) return;
    if (l >= x && r <= y) {
        opt(p, v);
        return;
    }
    down(p);
    int mid = l + r >> 1;
    change(ln, l, mid, x, y, v), change(rn, mid + 1, r, x, y, v);
    tr[p] = tr[ln] + tr[rn];
}

void dfs(int u, int f, ll dist) {
    eu[++tot] = u;
    modify(1, 1, 2 * n - 1, tot, dist);
    in[u] = tot;
    for (auto [v, id, w] : e[u]) {
        if (v == f) continue;
        be[id] = v;
        dfs(v, u, dist + w);
        eu[++tot] = u;
        modify(1, 1, 2 * n - 1, tot, dist);
    }
    out[u] = tot;
}

void solve() {
    cin >> n >> q >> w;
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        e[u].push_back({v, i, w});
        e[v].push_back({u, i, w});

        val[i] = w;
    }
    dfs(1, 0, 0);
    ll last = 0;
    while (q--) {
        ll d, e;
        cin >> d >> e;
        d = (d + last) % (n - 1);
        e = (e + last) % w;

        int u = be[d];
        change(1, 1, 2 * n - 1, in[u], out[u], -val[d] + e);
        val[d] = e;

        cout << tr[1].ans << '\n';

        last = tr[1].ans;
    }
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int t = 1;
    while (t--) solve();
    return 0;
}
