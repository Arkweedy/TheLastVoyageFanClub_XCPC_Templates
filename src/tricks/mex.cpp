
struct Node {
    int l, r, v;
    bool operator<(const Node& a) const { return l < a.l; }
};

vector<pair<int, int>> seg[N];  // 按照 mex 值存放极小区间。
void solve() {
    set<int> s;
    for (int i = 1; i <= n; i++) {
        s.insert(a[i]);
        while (s.count(cur)) cur++;
        b[i] = cur;
    }

    set<Node> odt;
    for (int i = 1; i <= n;) {
        int j = i;
        while (j <= n && b[j] == b[i]) j++;
        odt.insert({i, j - 1, b[i]});
        i = j;
    }
    for (int i = 1; i <= n; i++) {
        auto it = odt.lower_bound({lst[i], 0, 0});
        assert(it != odt.begin());
        it = prev(it);
        int x = n + 1, y = 0;
        while (it->v >= a[i]) {
            auto [l, r, v] = *it;

            seg[v].push_back({i, l});
            x = min(x, l), y = max(r, y);
            if (r >= lst[i]) odt.insert({lst[i], r, v});

            if (it == odt.begin()) {
                odt.erase(it);
                break;
            }

            /*
            C++ 17 后可以 odt.erase(it--);
            */

            auto nit = prev(it);
            odt.erase(it);
            it = nit;
        }

        y = min(y, lst[i] - 1);

        if (x <= y) odt.insert({x, y, a[i]});
        seg[a[i] == 0].push_back({i, i});
        it = odt.begin();
        int r = it->r, v = it->v;

        odt.erase(it);
        if (r > i) odt.insert({i + 1, r, v});
    }
}
