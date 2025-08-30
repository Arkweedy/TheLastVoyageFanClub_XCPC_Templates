struct trie {
    struct Node {
        array<int, 3> next;
        int cnt;
        int len;
        int maxlen;
        Node() : next{}, cnt{}, len{}, maxlen{} {};
    };
    vector<Node> t;

    trie() {
        t.assign(2, Node());
        t[0].next.fill(1);
        t[0].cnt = 0;
        t[0].len = 0;
        t[0].maxlen = 0;
    }

    int newNode() {
        t.emplace_back();
        return t.size() - 1;
    }

    int num(char c) { return c - 'a'; }

    void add(const string& s, int weight) {
        int p = 1;
        int len = s.length();
        int maxlen = 0;
        string x;
        for (int i = 0; i < len * 2 || i < t[p].maxlen; i++) {
            if (next(p, s[i % len]) == 0) {
                next(p, s[i % len]) = newNode();
                t[next(p, s[i % len])].len = t[p].len + 1;
            }
            x += s[i % len];
            p = next(p, s[i % len]);
            t[p].cnt += weight;
            maxlen++;
        }
        p = 1;
        for (int i = 0; i < maxlen; i++) {
            p = next(p, s[i % len]);
            t[p].maxlen = max(t[p].maxlen, maxlen);
        }

        return;
    }

    int& next(int p, char c) { return t[p].next[num(c)]; }
};