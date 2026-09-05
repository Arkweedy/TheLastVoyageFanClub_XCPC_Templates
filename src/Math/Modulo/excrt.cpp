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

i64 excrt(const vector<i64> &m, const vector<i64> &r) {
    int n = m.size();
    i128 R = r[0];
    i64 M = m[0];
    for (int i = 1; i < n; i++) {
        i64 x, y;
        i64 g = exgcd(M, m[i], x, y);
        assert((r[i] - R) % g == 0);
        i128 z = x * ((r[i] - R) / g);
        R = z * M + R;
        M = lcm(M, m[i]);
        R = (R % M + M) % M;
    }
    return (i64)R;
}
