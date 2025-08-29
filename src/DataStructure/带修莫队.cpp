#include <bits/stdc++.h>
#define ll long long

using namespace std;

const int N = 2e5 + 10, M = 1e6 + 10;

struct Q {
    int l, r, t, id;
} q[N], qt[N];
/*
q 表示普通查询
qt 表示修改操作
*/

int n, m, tot, c[N], B, ti;
int cnt[M], res, ans[N];
inline bool cmp(Q a, Q b) {
    if (a.l / B == b.l / B) {
        if (a.r / B == b.r / B) {
            return a.t < b.t;
        }
        return a.r / B < b.r / B;
    }
    return a.l < b.l;
}

int l = 1, r = 0, t = 0;
inline void move(int x, int t) {
    cnt[c[x]] += t;
    if (cnt[c[x]] == 0) res--;
    if (cnt[c[x]] == t) res++;
}

inline void flow(int t) {
    if (!t) return;
    if (qt[t].l >= l && qt[t].l <= r) {
        if (--cnt[c[qt[t].l]] == 0) res--;
        if (++cnt[qt[t].r] == 1) res++;
    }
    swap(c[qt[t].l], qt[t].r);
}

void solve() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", c + i);
    }
    for (int i = 1; i <= m; i++) {
        char op[2];
        int l, r;
        cin >> op >> l >> r;
        if (op[0] == 'Q') {
            q[tot].id = tot, q[tot].t = ti, q[tot].l = l, q[tot++].r = r;
        } else {
            qt[++ti].l = l, qt[ti].r = r;
        }
    }
    B = pow(n, 0.666);
    sort(q, q + tot, cmp);
    for (int i = 0; i < tot; i++) {
        while (q[i].t > t) flow(++t);
        while (q[i].t < t) flow(t--);
        while (q[i].l < l) move(--l, 1);
        while (q[i].r > r) move(++r, 1);
        while (q[i].l > l) move(l++, -1);
        while (q[i].r < r) move(r--, -1);
        ans[q[i].id] = res;
    }
    for (int i = 0; i < tot; i++) printf("%d\n", ans[i]);
}

int main() {
    int t = 1;
    while (t--) solve();
    return 0;
}