#include <bits/stdc++.h>

using namespace std;
const int mod = 1e9 + 9;

using poly = vector<int>;
#define mut(a, b) (1ll * (a) * (b) % mod)
#define add(a, b) (((a) += (b)) >= mod ? (a) -= mod : 0)
#define del(a, b) (((a) -= (b)) < 0 ? (a) += mod : 0)

namespace FWT {  // 多项式位运算卷积
void FWTor(poly& a, bool rev) {
    int n = a.size();
    for (int len = 2, m = 1; len <= n; len <<= 1, m <<= 1)
        for (int j = 0; j < n; j += len)
            for (int i = 0; i < m; i++) {
                if (!rev)
                    add(a[i + j + m], a[i + j]);
                else
                    del(a[i + j + m], a[i + j]);
            }
}
void FWTand(poly& a, bool rev) {
    int n = a.size();
    for (int len = 2, m = 1; len <= n; len <<= 1, m <<= 1)
        for (int j = 0; j < n; j += len)
            for (int i = 0; i < m; i++) {
                if (!rev)
                    add(a[i + j], a[i + j + m]);
                else
                    del(a[i + j], a[i + j + m]);
            }
}
void FWTxor(poly& a, bool rev) {
    int n = a.size(), inv2 = (mod + 1) >> 1;
    for (int len = 2, m = 1; len <= n; len <<= 1, m <<= 1)
        for (int j = 0; j < n; j += len)
            for (int i = 0; i < m; i++) {
                int x = a[i + j], y = a[i + j + m];
                int p = (x + y) % mod, q = (x - y + mod) % mod;
                if (!rev)
                    a[i + j] = p, a[i + j + m] = q;
                else
                    a[i + j] = mut(p, inv2), a[i + j + m] = mut(q, inv2);
            }
}
}  // namespace FWT
// Binary convolution for | , & , ^
namespace Poly {
int norm(int n) { return 1 << (__lg(n - 1) + 1); }
poly operator|(poly a, poly b) {
    int n = std::max(a.size(), b.size()), nn = norm(n);
    a.resize(nn);
    FWT::FWTor(a, 0);
    b.resize(nn);
    FWT::FWTor(b, 0);
    poly A(nn);
    for (int i = 0; i < nn; i++) A[i] = mut(a[i], b[i]);
    FWT::FWTor(A, 1);
    return A;
}
poly operator&(poly a, poly b) {
    int n = std::max(a.size(), b.size()), nn = norm(n);
    a.resize(nn);
    FWT::FWTand(a, 0);
    b.resize(nn);
    FWT::FWTand(b, 0);
    poly A(nn);
    for (int i = 0; i < nn; i++) A[i] = mut(a[i], b[i]);
    FWT::FWTand(A, 1);
    return A;
}
poly operator^(poly a, poly b) {
    int n = std::max(a.size(), b.size()), nn = norm(n);
    a.resize(nn);
    FWT::FWTxor(a, 0);
    b.resize(nn);
    FWT::FWTxor(b, 0);
    poly A(nn);
    for (int i = 0; i < nn; i++) A[i] = mut(a[i], b[i]);
    FWT::FWTxor(A, 1);
    return A;
}

poly subset(int n, poly& p, poly& q)  // 子集卷积,n代表集合大小
{
    vector<vector<int>> a(n + 1, vector<int>(1 << n)),
        b(n + 1, vector<int>(1 << n)), ans(n + 1, vector<int>(1 << n));
    for (int i = 0; i < 1 << n; i++) a[__builtin_popcount(i)][i] = p[i];
    for (int i = 0; i < 1 << n; i++) b[__builtin_popcount(i)][i] = q[i];
    for (int i = 0; i <= n; i++) {
        FWT::FWTor(a[i], 0);
        FWT::FWTor(b[i], 0);
    }
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= i; j++)
            for (int k = 0; k < 1 << n; k++)
                ans[i][k] =
                    (ans[i][k] + 1ll * a[j][k] * b[i - j][k] % mod) % mod;
    for (int i = 0; i <= n; i++) FWT::FWTor(ans[i], 1);
    poly h(1 << n);
    for (int i = 0; i < 1 << n; i++) h[i] = ans[__builtin_popcount(i)][i];
    return h;
}
}  // namespace Poly
using namespace Poly;
