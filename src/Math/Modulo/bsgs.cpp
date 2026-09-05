int power(int a, int p, int P) {
    int res = 1;
    while (p) {
        if (p & 1) res = 1ll * res * a % P;
        a = 1ll * a * a % P;
        p >>= 1;
    }
    return res;
}

int bsgs(int a, int b, int p) {
    a %= p;
    b %= p;
    if (p == 1) return 0;
    if (b == 1) return 0;

    int sq = ceil(sqrt(p));

    unordered_map<int, int> mp;
    int x = 1;
    for (int i = 0; i <= sq; i++) {
        mp[1ll * x * b % p] = i;
        x = 1ll * x * a % p;
    }

    int asq = power(a, sq, p);
    int y = asq;
    for (int i = 1; i <= sq; i++) {
        if (mp.find(y) != mp.end()) {
            return i * sq - mp[y];
        }
        y = 1ll * y * asq % p;
    }
    return -1;
}
