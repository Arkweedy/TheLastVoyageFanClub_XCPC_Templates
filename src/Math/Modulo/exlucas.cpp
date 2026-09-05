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

class Calcer {
    int p, a, pa;
    vector<int> f;

    i64 nu(i64 n) {
        i64 cnt = 0;
        do {
            n /= p;
            cnt += n;
        } while (n);
        return cnt;
    }

    i64 fac(i64 n) {
        int neg = p != 2 || pa <= 4;
        i64 res = 1;
        while (n > 1) {
            if ((n / pa) & neg) res = pa - res;
            res = res * f[n % pa] % pa;
            n /= p;
        }
        return res;
    }

public:
    Calcer(int p, int a, int pa)
        : p(p), a(a), pa(pa), f(pa) {
        f[0] = 1;
        for (int i = 1; i < pa; i++) {
            f[i] = i % p ? 1ll * f[i - 1] * i % pa : f[i - 1];
        }
    }

    int binom(i64 n, i64 k) {
        i64 v = nu(n) - nu(k) - nu(n - k);
        if (v >= a) return 0;
        i64 res = fac(n - k) * fac(k) % pa;
        res = fac(n) * inv(res, pa) % pa;
        while (v--) res *= p;
        return res % pa;
    }
};

class MComb {
    int m;
    vector<Calcer> calcer;
    vector<int> crt_coef;

public:
    MComb(int n)
        : m(n) {
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                int x = 0, y = 1;
                while (n % i == 0) {
                    x++;
                    y *= i;
                    n /= i;
                }
                calcer.emplace_back(i, x, y);
                crt_coef.push_back(1ll * m / y * inv(m / y, y) % m);
            }
        }
        if (n > 1) {
            calcer.emplace_back(n, 1, n);
            crt_coef.push_back(1ll * m / n * inv(m / n, n) % m);
        }
    }

    int binom(i64 n, i64 k) {
        i64 res = 0;
        for (int i = 0; i < calcer.size(); i++) {
            res = (res + 1ll * calcer[i].binom(n, k) * crt_coef[i]) % m;
        }
        return res;
    }
};
