#include <bits/stdc++.h>

#define ll long long
using namespace std;
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

const int N = 1e5 + 10;

struct Node {
    int s[2], tag, fa, sum, key;
} tr[N];

int tot, rt;
#define ls(x) tr[(x)].s[0]
#define rs(x) tr[(x)].s[1]
#define fa(x) tr[(x)].fa

bool get(int x) { return x == rs(fa(x)); }
bool isroot(int x) { return ls(fa(x)) != x && rs(fa(x)) != x; }

void pushup(int x) { tr[x].sum = tr[x].key ^ tr[ls(x)].sum ^ tr[rs(x)].sum; }

/*
当合并需要考虑左右儿子顺序时可能需要这种 tag
void Rev(int rt)
{
	swap(son[rt][0],son[rt][1]);
	swap(lsum[rt],rsum[rt]);
	rev_tag[rt] ^= 1;
}
void Push_Down(int rt)
{
	if(rev_tag[rt])
	{
		Rev(son[rt][0]);
		Rev(son[rt][1]);
		rev_tag[rt] = 0;
	{
}

*/

void pushdown(int x) {
    if (tr[x].tag) {
        swap(ls(x), rs(x));
        tr[ls(x)].tag ^= 1;
        tr[rs(x)].tag ^= 1;
        tr[x].tag = 0;
    }
}

void update(int x) {
    if (!isroot(x)) update(fa(x));
    pushdown(x);
}

void rotate(int x) {
    int y = fa(x), z = fa(y), c = get(x);
    if (!isroot(y)) tr[z].s[get(y)] = x;
    fa(tr[x].s[c ^ 1]) = y, tr[y].s[c] = tr[x].s[c ^ 1], tr[x].s[c ^ 1] = y;
    fa(y) = x, fa(x) = z;
    pushup(y), pushup(x);
}

void splay(int x) {
    update(x);
    for (int f = fa(x); f = fa(x), !isroot(x); rotate(x))
        if (!isroot(f)) rotate(get(f) == get(x) ? f : x);
}

// 把 x 到原树的根这条路径上的边都变成实链
void access(int x) {
    for (int p = 0; x; p = x, x = fa(x)) splay(x), rs(x) = p, pushup(x);
}

// 让 x 成为原树的根
void makeroot(int x) { access(x), splay(x), tr[x].tag ^= 1; }

// 查找 x 所在原树的根
int find(int x) {
    access(x), splay(x);
    while (pushdown(x), ls(x)) x = ls(x);
    return splay(x), x;
}
// 把 x 到 y 的路径拿出来变成一棵 Splay
void split(int x, int y) { makeroot(x), access(y), splay(y); }

// 添加边 x - y ，若以连通无效果
void link(int x, int y) {
    makeroot(x);
    if (find(y) != x) fa(x) = y;
}

// 删边，允许边不存在
void cut(int x, int y) {
    makeroot(x);
    if (find(y) == x && fa(y) == x && !ls(y)) fa(y) = rs(x) = 0;
}

int n, m;

void solve() {
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> tr[i].key;
        tr[i].sum = tr[i].key;
    }
    while (m--) {
        int op, x, y;
        cin >> op >> x >> y;
        if (op == 0) {
            // 链查询
            split(x, y);
            cout << tr[y].sum << '\n';
        } else if (op == 1) {
            // 加边
            link(x, y);
        } else if (op == 2) {
            // 删边
            cut(x, y);
        } else {
            // 单点修改
            makeroot(x);
            tr[x].sum = tr[x].sum ^ tr[x].key ^ y;
            tr[x].key = y;
        }
    }
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int t = 1;
    while (t--) solve();
    return 0;
}