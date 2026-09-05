template <typename T>
T exgcd(T a, T b, T &x, T &y) {
    bool neg_a = a < 0, neg_b = b < 0;
    if (neg_a) a = -a;
    if (neg_b) b = -b;

    array<T, 4> m = {1, 0, 0, 1};
    while (b != 0) {
        T q = a / b;
        m = {m[1], m[0] - q * m[1],
             m[3], m[2] - q * m[3]};
        T r = a % b;
        a = b;
        b = r;
    }
    x = neg_a ? -m[0] : m[0];
    y = neg_b ? -m[2] : m[2];
    return a;
}

i64 inv(i64 a, i64 m) {
    assert(m >= 2);
    a %= m;
    if (a < 0) a += m;

    i64 x, y;
    i64 g = exgcd(a, m, x, y);
    assert(g == 1);
    x %= m;
    if (x < 0) x += m;
    return x;
}

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
    mp.reserve(sq * 2);
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

int exbsgs(int a, int b, int p) {
    a %= p;
    b %= p;
    if (p == 1 || b == 1) return 0;
    if (a == 0) {
        if (b == 0) return 1; // 0^0 = 1
        return -1;
    }

    int k = 0;
    int d = 1;
    while (1) {
        int g = gcd(a, p);
        if (g == 1) break;
        if (b % g != 0) return -1;
        p /= g;
        b /= g;
        d = 1ll * d * (a / g) % p;
        k++;
        if (d == b) return k;
    }

    int r = 1ll * b * inv(d, p) % p;
    int res = bsgs(a, r, p);
    if (res == -1) return -1;
    return res + k;
}
