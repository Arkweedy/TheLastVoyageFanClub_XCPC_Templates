struct Node {
    int s[2], siz, tag, key, fa;
} tr[N];

int tot, rt;
#define ls(x) tr[(x)].s[0]
#define rs(x) tr[(x)].s[1]
#define fa(x) tr[(x)].fa

int newnode(int key) {
    tr[++tot].key = key, tr[tot].siz = 1;
    return tot;
}
void maintain(int x) { tr[x].siz = tr[ls(x)].siz + tr[rs(x)].siz + 1; }
void clear(int x) { ls(x) = rs(x) = fa(x) = tr[x].siz = tr[x].key = 0; }
bool get(int x) { return x == rs(fa(x)); }

void rotate(int x) {
    int y = fa(x), z = fa(y);
    int c = get(x);
    if (tr[x].s[c ^ 1]) fa(tr[x].s[c ^ 1]) = y;
    tr[y].s[c] = tr[x].s[c ^ 1], tr[x].s[c ^ 1] = y, fa(y) = x, fa(x) = z;
    if (z) tr[z].s[y == tr[z].s[1]] = x;
    maintain(y), maintain(x);
}

// 假设每次操作最后访问到的节点是 x 则为了保证复杂度需要将 x splay 到根，也就是
// splay(x,0)
void splay(int x, int y = 0) {
    for (int f = fa(x); f = fa(x), f != y; rotate(x))
        if (fa(f) != y) rotate(get(f) == get(x) ? f : x);
    if (!y) rt = x;
}

void ins(int key) {
    int cur = rt, f = 0;
    while (cur) f = cur, cur = tr[cur].s[key > tr[cur].key];
    cur = newnode(key), fa(cur) = f, tr[f].s[key > tr[f].key] = cur, splay(cur);
}

void del(int key) {
    int now = rt, p = 0;
    while (tr[now].key != key && now)
        p = now, now = tr[now].s[key > tr[now].key];
    if (!now) {
        splay(p);
        return;
    }
    splay(now);
    int cur = ls(now);
    if (!cur) {
        rt = rs(now), fa(rs(now)) = 0, clear(now);
        return;
    }
    while (rs(cur)) cur = rs(cur);
    rs(cur) = rs(now), fa(rs(now)) = cur, fa(ls(now)) = 0, clear(now);
    maintain(cur), splay(cur);
}
int rnk(int key) {
    int res = 1, cur = rt, p;
    while (cur)
        if (p = cur, tr[cur].key < key)
            res += tr[ls(cur)].siz + 1, cur = rs(cur);
        else
            cur = ls(cur);
    return splay(p), res;
}

int kth(int rk) {
    int cur = rt;
    while (cur) {
        int sz = tr[ls(cur)].siz + 1;
        if (sz > rk)
            cur = ls(cur);
        else if (sz == rk)
            break;
        else
            rk -= sz, cur = rs(cur);
    }
    return splay(cur), tr[cur].key;
}

int pre(int key) {
    int cur = rt, ans = 0, p;
    while (cur)
        if (p = cur, tr[cur].key >= key)
            cur = ls(cur);
        else
            ans = tr[cur].key, cur = rs(cur);
    return splay(p), ans;
}
int nxt(int key) {
    int cur = rt, ans = 0, p;
    while (cur)
        if (p = cur, tr[cur].key <= key)
            cur = rs(cur);
        else
            ans = tr[cur].key, cur = ls(cur);
    return splay(p), ans;
}