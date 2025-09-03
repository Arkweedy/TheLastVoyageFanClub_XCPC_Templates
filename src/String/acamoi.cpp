int cnt = 0;
#define K 26
struct node {
    int son[K], num, fail;
    void clear() {
        num = fail = 0;
        memset(son, 0, sizeof(son));
    }
} tr[N];
void insert(string s) {
    int p = 0;
    for (int i = 0; i < s.length(); i++) {
        int ch = s[i] - 'a';
        if (!tr[p].son[ch]) tr[p].son[ch] = ++cnt, tr[cnt].clear();
        p = tr[p].son[ch];
    }
    tr[p].num++;
}
vector<int> e[N];
void getfail() {
    queue<int> q;
    for (int i = 0; i < K; i++)
        if (tr[0].son[i]) q.push(tr[0].son[i]), e[0].push_back(tr[0].son[i]);
    while (q.size()) {
        int p = q.front();
        q.pop();
        for (int i = 0; i < K; i++) {
            if (tr[p].son[i]) {
                int a = tr[p].son[i], b = tr[tr[p].fail].son[i];
                tr[a].fail = b, q.push(a);
                e[b].push_back(a);
            } else
                tr[p].son[i] = tr[tr[p].fail].son[i];
        }
    }
}
// 清空：cnt=0,tr[0].clear();
// for(int i=1;i<=n;i++) insert(s[i]);
// for(int i=0;i<=cnt;i++) e[i].clear();