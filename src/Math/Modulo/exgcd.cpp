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
