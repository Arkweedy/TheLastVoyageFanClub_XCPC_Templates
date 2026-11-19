#include <bits/stdc++.h>
using namespace std;

namespace sps {
// 数组长度为 2^n，二元运算两数组等长，系数取 [0, P)。
constexpr int P = 998244353;

// 求标量 a 的 k 次方模 P，k 为非负整数。
int power(int a, uint64_t k) {
    int res = 1;
    for (; k; k >>= 1, a = 1LL * a * a % P)
        if (k & 1) res = 1LL * res * a % P;
    return res;
}
// 原地将所有系数乘 c，要求 0 <= c < P。
void scale(vector<int> &a, int c) {
    if (c == 1) return;
    for (int &x : a) x = 1LL * x * c % P;
}

// 原地求子集和，每项变为其所有子集的系数和。
void subSum(vector<int> &a) {
    int m = a.size();
    for (int b = 1; b < m; b <<= 1)
        for (int s = 0; s < m; s++) if (s & b) {
            a[s] += a[s ^ b];
            if (a[s] >= P) a[s] -= P;
        }
}
// 原地做子集反演，从子集和恢复原系数。
void subDiff(vector<int> &a) {
    int m = a.size();
    for (int b = 1; b < m; b <<= 1)
        for (int s = 0; s < m; s++) if (s & b) {
            a[s] -= a[s ^ b];
            if (a[s] < 0) a[s] += P;
        }
}
// 原地求超集和，每项变为其所有超集的系数和。
void supSum(vector<int> &a) {
    int m = a.size();
    for (int b = 1; b < m; b <<= 1)
        for (int s = 0; s < m; s++) if (s & b) {
            a[s ^ b] += a[s];
            if (a[s ^ b] >= P) a[s ^ b] -= P;
        }
}
// 原地做超集反演，从超集和恢复原系数。
void supDiff(vector<int> &a) {
    int m = a.size();
    for (int b = 1; b < m; b <<= 1)
        for (int s = 0; s < m; s++) if (s & b) {
            a[s ^ b] -= a[s];
            if (a[s ^ b] < 0) a[s ^ b] += P;
        }
}
// 分层子集和，a[k][s] 为 s 的所有 k 元子集的系数和。
vector<vector<int>> lift(const vector<int> &f) {
    int m = f.size(), n = __lg(m);
    vector a(n + 1, vector<int>(m));
    for (int s = 0; s < m; s++)
        a[popcount((unsigned)s)][s] = f[s];
    for (auto &row : a) subSum(row);
    return a;
}

// 不交子集卷积，按 S 的所有有序不交拆分累加系数乘积。
vector<int> mul(const vector<int> &f, const vector<int> &g) {
    assert(f.size() == g.size());
    auto a = lift(f), b = lift(g);
    int m = f.size(), n = __lg(m);
    // 倒序写回，低层仍保留原来的子集和。
    for (int k = n; k >= 0; k--)
        for (int s = 0; s < m; s++) {
            int sum = 0;
            for (int i = 0; i <= k; i++)
                sum = (sum + 1LL * a[i][s] * b[k - i][s]) % P;
            a[k][s] = sum;
        }
    for (auto &row : a) subDiff(row);
    vector<int> res(m);
    for (int s = 0; s < m; s++)
        res[s] = a[popcount((unsigned)s)][s];
    return res;
}

// 按集合大小求导，每项乘其元素个数，空集项变为 0。
vector<int> deriv(vector<int> f) {
    int m = f.size();
    for (int s = 0; s < m; s++)
        f[s] = 1LL * popcount((unsigned)s) * f[s] % P;
    return f;
}
// 积分要求 f[0] = 0，非空项除以其元素个数，常数项取 c。
vector<int> integr(vector<int> f, int c = 0) {
    assert(f[0] == 0);
    int m = f.size(), n = __lg(m);
    vector<int> iv(n + 1, 1);
    for (int i = 2; i <= n; i++)
        iv[i] = P - 1LL * (P / i) * iv[P % i] % P;
    for (int s = 1; s < m; s++)
        f[s] = 1LL * f[s] * iv[popcount((unsigned)s)] % P;
    f[0] = c % P;
    if (f[0] < 0) f[0] += P;
    return f;
}

// 集合幂级数求逆，要求 f[0] != 0。
vector<int> inv(const vector<int> &f) {
    assert(f[0] != 0);
    vector<int> g{power(f[0], P - 2)};
    // g 是低半段的逆，新接入的高半段为 -g * hi * g。
    while (g.size() < f.size()) {
        int m = g.size();
        vector<int> hi(f.begin() + m, f.begin() + 2 * m);
        hi = mul(mul(g, hi), g);
        for (int x : hi) g.push_back(x ? P - x : 0);
    }
    return g;
}
// 返回 f 乘 g 的集合逆元，要求 g[0] != 0。
vector<int> div(const vector<int> &f, const vector<int> &g) {
    return mul(f, inv(g));
}

// 集合指数，由单块权值求无序划分总权，要求 f[0] = 0。
vector<int> exp(const vector<int> &f) {
    assert(f[0] == 0);
    vector<int> g{1};
    while (g.size() < f.size()) {
        int m = g.size();
        vector<int> hi(f.begin() + m, f.begin() + 2 * m);
        hi = mul(g, hi);
        g.insert(g.end(), hi.begin(), hi.end());
    }
    return g;
}
// 集合对数，由划分总权恢复单块权值，要求 f[0] = 1。
vector<int> log(const vector<int> &f) {
    assert(f[0] == 1);
    return integr(mul(deriv(f), inv(f)));
}
// 规范化平方根，要求 f[0] = 1，返回的根也满足空集项为 1。
vector<int> sqrt(const vector<int> &f) {
    auto g = log(f);
    for (int &x : g) {
        if (x & 1) x += P;
        x /= 2;
    }
    return exp(g);
}
// 集合幂级数的非负整数次幂，零次幂返回单位元。
vector<int> pow(vector<int> f, uint64_t k) {
    int m = f.size(), n = __lg(m), c = f[0];
    vector<int> res(m);
    res[0] = 1;
    if (!k) return res;
    if (c) {
        scale(f, power(c, P - 2));
        f = log(f);
        scale(f, k % P);
        f = exp(f);
        scale(f, power(c, k));
        return f;
    }
    if (k > (uint64_t)n) return vector<int>(m);
    while (k) {
        if (k & 1) res = mul(res, f);
        k >>= 1;
        if (k) f = mul(f, f);
    }
    return res;
}

// 原地点乘，a 的每个系数乘 b 的同下标系数。
void pointMul(vector<int> &a, const vector<int> &b) {
    assert(a.size() == b.size());
    int m = a.size();
    for (int i = 0; i < m; i++)
        a[i] = 1LL * a[i] * b[i] % P;
}
// 按位 OR 卷积，将每对系数的积计入两掩码的并集。
vector<int> orConv(vector<int> a, vector<int> b) {
    subSum(a);
    subSum(b);
    pointMul(a, b);
    subDiff(a);
    return a;
}
// 按位 AND 卷积，将每对系数的积计入两掩码的交集。
vector<int> andConv(vector<int> a, vector<int> b) {
    supSum(a);
    supSum(b);
    pointMul(a, b);
    supDiff(a);
    return a;
}
// 原地 XOR 和差变换，连续变换两次等于整体乘数组长度。
void fwt(vector<int> &a) {
    int m = a.size();
    for (int b = 1; b < m; b <<= 1)
        for (int s = 0; s < m; s++) if (s & b) {
            int x = a[s ^ b], y = a[s];
            a[s ^ b] = x + y;
            if (a[s ^ b] >= P) a[s ^ b] -= P;
            a[s] = x - y;
            if (a[s] < 0) a[s] += P;
        }
}
// 按位 XOR 卷积，将每对系数的积计入两掩码的异或。
vector<int> xorConv(vector<int> a, vector<int> b) {
    fwt(a);
    fwt(b);
    pointMul(a, b);
    fwt(a);
    scale(a, power((int)a.size(), P - 2));
    return a;
}
} // namespace sps
