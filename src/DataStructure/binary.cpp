// 整体二分+树状数组 O(n*logn*logV)
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;

const int N = 300005;
int n, m, cnt, a[N];
int ans[N], s[N];

struct Q {
    //  数: x位置，y值，k贡献，opt=0
    // 查询: [x,y]第k小，id编号，opt=1
    int x, y, k, id, opt;
} q[N << 1], q1[N << 1], q2[N << 1];

void add(int x, int v) {  // 加入贡献
    while (x <= n) s[x] += v, x += x & (-x);
}
int sum(int x) {  // 前缀和
    int t = 0;
    while (x) t += s[x], x -= x & (-x);
    return t;
}
void solve(int l, int r, int L, int R) {
    if (l > r) return;  //[l,r]数据个数域 [L,R]值域
    if (L == R) {
        for (int i = l; i <= r; i++)
            if (q[i].opt) ans[q[i].id] = L;  // 记录答案
        return;
    }
    int mid = (L + R) >> 1, p1 = 0, p2 = 0;
    for (int i = l; i <= r; i++) {
        if (!q[i].opt) {  // 若是数，按值分流
            if (q[i].y <= mid)
                add(q[i].x, q[i].k),  // 加入贡献
                    q1[++p1] = q[i];  // 分流到左边
            else
                q2[++p2] = q[i];  // 分流到右边
        } else {                  // 若是查询，按个数分流
            int s = sum(q[i].y) - sum(q[i].x - 1);
            if (s >= q[i].k)
                q1[++p1] = q[i];  // 分流到左边
            else
                q[i].k -= s, q2[++p2] = q[i];  // 分流到右边
        }
    }
    for (int i = 1; i <= p1; i++)
        if (!q1[i].opt) add(q1[i].x, -q1[i].k);  // 减去贡献
    for (int i = 1; i <= p1; i++) q[i + l - 1] = q1[i];
    for (int i = 1; i <= p2; i++) q[i + l + p1 - 1] = q2[i];  // 合并数组
    solve(l, l + p1 - 1, L, mid);
    solve(l + p1, r, mid + 1, R);  // 分治
}
int main() {
    scanf("%d%d", &n, &m);
    char ch[2];
    int x, y, k;
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), q[++cnt] = {i, a[i], 1, 0, 0};
    for (int i = 1; i <= m; i++) {
        scanf("%s%d%d", ch, &x, &y);
        if (ch[0] == 'C')
            q[++cnt] = {x, a[x], -1, 0, 0},         // 旧数贡献-1
                q[++cnt] = {x, a[x] = y, 1, 0, 0};  // 新数贡献+1
        else
            scanf("%d", &k), q[++cnt] = {x, y, k, i, 1};
    }
    memset(ans, -1, sizeof ans);
    solve(1, cnt, 0, 1e9);  // 整体二分
    for (int i = 1; i <= m; i++)
        if (~ans[i]) printf("%d\n", ans[i]);
}