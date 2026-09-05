#include<bits/stdc++.h>
using i64 = long long;
using ll = long long;
using uint = unsigned int;
using ull = unsigned long long;

using namespace std;

//polys.cpp Create time : 2026.01.13 22:21
constexpr int P = 998244353;
constexpr int G = 3;

int power(int a, int p)
{
    int res = 1;
    while(p){
        if(p & 1)res = 1ll * res * a % P;
        a = 1ll * a * a % P;
        p >>= 1;
    }
    return res;
}

int power(int a, int p, int M)
{
    int res = 1;
    while(p){
        if(p & 1)res = 1ll * res * a % M;
        a = 1ll * a * a % M;
        p >>= 1;
    }
    return res;
}

int inv(int a)
{
    return power(a, P - 2);
}

struct Comb {
    int n;
    std::vector<int> _fac;
    std::vector<int> _invfac;
    std::vector<int> _inv;
     
    Comb() : n{0}, _fac{1}, _invfac{1}, _inv{0} {}
    Comb(int n) : Comb() {
        init(n);
    }
     
    void init(int m) {
        if (m <= n) return;
        _fac.resize(m + 1);
        _invfac.resize(m + 1);
        _inv.resize(m + 1);
         
        for (int i = n + 1; i <= m; i++) {
            _fac[i] = 1ll * _fac[i - 1] * i % P;
        }
        _invfac[m] = power(_fac[m], P - 2);
        for (int i = m; i > n; i--) {
            _invfac[i - 1] = 1ll * _invfac[i] * i % P;
            _inv[i] = 1ll * _invfac[i] * _fac[i - 1] % P;
        }
        n = m;
    }
     
    int fac(int m) {
        if (m > n) init(2 * m);
        return _fac[m];
    }
    int invfac(int m) {
        if (m > n) init(2 * m);
        return _invfac[m];
    }
    int inv(int m) {
        if (m > n) init(2 * m);
        return _inv[m];
    }
    int binom(int n, int m) {
        if (n < m || m < 0) return 0;
        return 1ll * fac(n) * invfac(m) % P * invfac(n - m) % P;
    }
} comb;


int powerF(int a, int b, int d, int p) //power(a + bw, p), w^2 = d, d/p = -1
{
    int ra = 1, rb = 0;
    while(p){
        if(p & 1){
            int nra = (1ll * a * ra + 1ll * b * rb % P * d) % P;
            int nrb = (1ll * a * rb + 1ll * b * ra) % P;
            ra = nra;
            rb = nrb;
        }
        int na = (1ll * a * a + 1ll * b * b % P * d) % P;
        int nb = 2ll * a * b % P;
        a = na;
        b = nb;
        p >>= 1;
    }
    return ra;
}

int Cipolla(int a)
{
    if(a == 0)return 0;
    int ea = power(a, (P - 1) / 2);
    if(ea == P - 1)return -1;
    int r, d, ed;
    mt19937 rng(random_device{}());
    do{
        r = rng() % P;
        d = (1ll * r * r - a + P) % P;
        ed = power(d, (P - 1) / 2);
    }while(ed == 1);
    int x = powerF(r, 1, d, (P + 1) / 2);
    return x;
}

constexpr int Q = 23; //998244353 = K 2^Q + 1
constexpr int MAX_LEN = 22;

array<int, 1 << MAX_LEN> omega()
{
    array<int, 1 << MAX_LEN> res;
    res.fill(1);
    for (int i = 1; i <= (1 << MAX_LEN - 1); i <<= 1) {
        int wi = power(G, (P - 1) / (i * 2));
        for (int j = 1; j < i; j++) {
            res[i + j] = 1ll * res[i + j - 1] * wi % P;
        }
    }
    return res;
}

const array<int, 1 << MAX_LEN> W = omega();

void dft(vector<int>& f)
{
    int n = (int)f.size();

    // DIF: input natural order, output bit-reversed order
    for (int k = n >> 1; k; k >>= 1) {
        for (int i = 0; i < n; i += k << 1) {
            for (int j = 0; j < k; j++) {
                int x = f[i + j];
                int y = f[i + j + k];

                // f[i+j] = x + y
                int s = x + y;
                if (s >= P) s -= P;
                f[i + j] = s;

                // f[i+j+k] = (x - y) * W[k+j]
                int d = x - y;
                if (d < 0) d += P;
                f[i + j + k] = 1ll * d * W[k + j] % P;
            }
        }
    }
    return;
}

void idft(vector<int>& f)
{
    int n = (int)f.size();
    int invn = inv(n);

    // IDIT: take bit-reversed order (from DIF), output natural order
    for (int k = 1; k < n; k <<= 1) {
        for (int i = 0; i < n; i += k << 1) {
            for (int j = 0; j < k; j++) {
                int x = f[i + j];
                int y = 1ll * f[i + j + k] * W[k + j] % P;

                int s = x + y;
                if (s >= P) s -= P;

                int d = x - y;
                if (d < 0) d += P;

                f[i + j] = s;
                f[i + j + k] = d;
            }
        }
    }

    // scale by 1/n
    for (int i = 0; i < n; i++) {
        f[i] = 1ll * f[i] * invn % P;
    }

    // reverse except f[0]  (invNTT trick with +roots)
    reverse(f.begin() + 1, f.end());
    return;
}

class poly : public vector<int>
{
public:
    poly() : vector<int>(){}

    template<class F>
    explicit poly(int n, F f) : vector<int>(n)
    {
        for(int i = 0;i < n;i++){
            (*this)[i] = f(i);
        }
    }
    template<class InputIt, class = std::_RequireInputIter<InputIt>>
    explicit poly(InputIt first, InputIt last) : vector<int>(first, last) {}
    
    explicit poly(int n, int val) : vector<int>(n, val){}
    explicit poly(int n) : vector<int>(n){}
    explicit poly(const vector<int> &a) : vector<int>(a) {}
    poly(const std::initializer_list<int> &a) : vector<int>(a){}

    friend poly operator+(const poly &a, const poly &b) {
        poly res(std::max(a.size(), b.size()));
        for (int i = 0; i < a.size(); i++) {
            res[i] = a[i];
        }
        for (int i = 0; i < b.size(); i++) {
            res[i] = res[i] + b[i];
            if(res[i] >= P)res[i] -= P;
        }
        return res;
    }
    friend poly operator-(const poly &a, const poly &b) {
        poly res(std::max(a.size(), b.size()));
        for (int i = 0; i < a.size(); i++) {
            res[i] = a[i];
        }
        for (int i = 0; i < b.size(); i++) {
            res[i] = res[i] - b[i];
            if(res[i] < 0)res[i] += P;
        }
        return res;
    }
    friend poly operator-(const poly &a) {
        std::vector<int> res(a.size());
        for (int i = 0; i < int(res.size()); i++) {
            res[i] = a[i] ? P - a[i] : 0;
        }
        return poly(res);
    }
    friend poly operator*(int a, poly b) {
        a = (a % P + P) % P;
        for (int i = 0; i < int(b.size()); i++) {
            b[i] = 1ll * b[i] * a % P;
        }
        return b;
    }
    friend poly operator*(poly a, int b) {
        b = (b % P + P) % P;
        for (int i = 0; i < int(a.size()); i++) {
            a[i] = 1ll * a[i] * b % P;
        }
        return a;
    }
    friend poly operator/(poly a, int b) {
        b = (b % P + P) % P;
        int invb = power(b, P - 2);
        for (int i = 0; i < int(a.size()); i++) {
            a[i] = 1ll * a[i] * invb % P;
        }
        return a;
    }

    friend poly operator/(const poly &a, const poly &b) {
        if(a.size() < b.size())return poly();
        int k = a.size() - b.size() + 1;
        return (a.rev().trunc(k) * b.rev().inv(k).trunc(k)).trunc(k).rev();
    }
    friend poly operator%(const poly &a, const poly &b) {
        return (a - a / b * b).trunc(min(a.size(), b.size() - 1));
    }

    poly &operator+=(poly b) {
        return (*this) = (*this) + b;
    }
    poly &operator-=(poly b) {
        return (*this) = (*this) - b;
    }
    poly &operator*=(poly b) {
        return (*this) = (*this) * b;
    }
    poly &operator*=(int b) {
        return (*this) = (*this) * b;
    }
    poly &operator/=(int b) {
        return (*this) = (*this) / b;
    }

    friend poly operator*(poly a, poly b) {
        if (a.size() == 0 || b.size() == 0) {
            return poly();
        }
        if (a.size() < b.size()) {
            std::swap(a, b);
        }
        int n = 1, tot = a.size() + b.size() - 1;
        while (n < tot) {
            n *= 2;
        }
        if (((P - 1) & (n - 1)) != 0 || b.size() < 128) {
            poly c(a.size() + b.size() - 1);
            for (int i = 0; i < a.size(); i++) {
                for (int j = 0; j < b.size(); j++) {
                    c[i + j] = (c[i + j] + 1ll * a[i] * b[j]) % P;
                }
            }
            return c;
        }
        a.resize(n);
        b.resize(n);
        dft(a);
        dft(b);
        for (int i = 0; i < n; ++i) {
            a[i] = 1ll * a[i] * b[i] % P; 
        }
        idft(a);
        a.resize(tot);
        return a;
    }

    poly shift(int k) const {
        if (k >= 0) {
            auto b = *this;
            b.insert(b.begin(), k, 0);
            return b;
        } else if (this->size() <= -k) {
            return poly();
        } else {
            return poly(this->begin() + (-k), this->end());
        }
    }

    poly trunc(int k) const {
        poly f = *this;
        f.resize(k);
        return f;
    }

    poly deriv() const {
        if (this->empty()) {
            return poly();
        }
        poly res(this->size() - 1);
        for (int i = 0; i < this->size() - 1; ++i) {
            res[i] = 1ll * (i + 1) * (*this)[i + 1] % P;
        }
        return res;
    }

    poly integr() const {
        poly res(this->size() + 1);
        comb.init(this->size() + 1);
        for (int i = 0; i < this->size(); ++i) {
            res[i + 1] = 1ll * (*this)[i] * comb.inv(i + 1) % P;
        }
        return res;
    }

    poly rev() const{
        if(this->empty()){
            return poly();
        }
        poly res = *this;
        while(!res.empty() && res.back() == 0)res.pop_back();
        reverse(res.begin(),res.end());
        return res;
    }

    poly inv(int m) const {
        poly x{power((*this)[0], P - 2)};
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x * (poly{2} - trunc(k) * x)).trunc(k);
        }
        return x.trunc(m);
    }

    poly log(int m) const {
        return (deriv() * inv(m)).integr().trunc(m);
    }

    poly exp(int m) const {
        poly x{1};
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x * (poly{1} - x.log(k) + trunc(k))).trunc(k);
        }
        return x.trunc(m);
    }

    poly exp2(int m) const {
        poly f(m), h = deriv();
        h.resize(m);
        f[0] = 1;
        // fn = 1/n(\sum h_k f_{n - 1 - k})
        auto cdq = [&](auto&&self, int l ,int r)->void
        {
            if(l == r){
                if(l == 0)return;
                f[l] = 1ll * f[l] * comb.inv(l) % P;
                return;
            }
            int m = l + r >> 1;
            self(self, l, m);
            poly a(m - l + 1),b(r - l);
            for(int i = 0;i <= m - l;i++){
                a[i] = f[i + l];
            }
            for(int i = 0;i < r - l;i++){
                b[i] = h[i];
            }
            auto c = a * b;
            for(int i = 0;i < r - m;i++){
                f[m + i + 1] = f[m + i + 1] + c[i + m - l];
                if(f[m + i + 1] >= P)f[m + i + 1] -= P;
            }
            self(self, m + 1, r);
            return;
        };
        cdq(cdq, 0, m - 1);
        return f;
    }

    poly pow(int k, int m) const {
        int i = 0;
        while (i < this->size() && (*this)[i] == 0) {
            i++;
        }
        if (i == this->size() || 1LL * i * k >= m) {
            return poly(m);
        }
        int v = (*this)[i];
        auto f = shift(-i) * power(v, P - 2);
        return ((f.log(m - i * k) * k).exp2(m - i * k).shift(i * k)) * power(v, k);
    }

    poly pow(int k, int kmodphiP,int big, int m) const {
        int i = 0;
        while (i < this->size() && (*this)[i] == 0) {
            i++;
        }
        if (i == this->size() || 1LL * i * k >= m) {
            return poly(m);
        }
        if(big && i > 0)return poly(m);//x^{big number} f(x)^{k}
        int v = (*this)[i];
        auto f = shift(-i) * power(v, P - 2);
        return ((f.log(m - i * k) * k).exp2(m - i * k).shift(i * k)) * power(v, kmodphiP);
    }

    poly sqrt(int m) const {
        poly x{Cipolla((*this)[0])};
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x + (trunc(k) * x.inv(k)).trunc(k)) * power(2, P - 2);
        }
        return x.trunc(m);
    }

    poly mulT(poly b) const {
        if (b.size() == 0) {
            return poly();
        }
        int n = b.size();
        std::reverse(b.begin(), b.end());
        return ((*this) * b).shift(-(n - 1));
    }

    vector<int> eval(vector<int> x)const 
    {
        if(this->size() == 0){
            return vector<int>(x.size(), 0);
        }
        if(x.size() == 0){
            return vector<int>(0);
        }
        int query_count = x.size();
        int n = max(x.size(), this->size()); // !!!
        x.resize(n);
        for(int i = 0;i < n;i++){ //norm P - x[i]
            if(x[i] <= 0)x[i] += P;
            if(x[i] > P)x[i] -= P;
        }
        vector<poly>m(n * 4);//segtree build Q(x)
        auto build = [&](auto&&self, int l, int r, int p)->void
        {
            if(l == r){
                m[p] = poly{1, P - x[l]};
                return;
            }
            int mid = l + r >> 1;
            self(self, l, mid , p * 2);
            self(self, mid + 1, r, p * 2 + 1);
            m[p] = m[p * 2] * m[p * 2 + 1];
            return;
        };
        build(build, 0, n - 1, 1);
        vector<int>ans(n);
        auto work = [&](auto&&self,const poly &f, int l, int r, int p)->void
        {
            if(l == r){
                ans[l] = f[0];
                return;
            }
            int mid = l + r >> 1;
            self(self, f.mulT(m[p * 2 + 1]).trunc(mid - l + 1), l, mid, p * 2);
            self(self, f.mulT(m[p * 2]).trunc(r - mid), mid + 1, r, p * 2 + 1);
            return;
        };
        work(work, mulT(m[1].inv(n)).trunc(n), 0, n - 1, 1);
        ans.resize(query_count);
        return ans;
    }

};

poly lagrange(vector<int>x, vector<int>y)
{
    assert(x.size() == y.size());
    if(x.size() == 0)return poly();
    int n = x.size();
    for(int i = 0;i < n;i++){ //norm P - x[i]
        if(x[i] <= 0)x[i] += P;
        if(x[i] > P)x[i] -= P;
    }
    for(int i = 0;i < n;i++){ //norm P - x[i]
        if(y[i] <= 0)y[i] += P;
        if(y[i] > P)y[i] -= P;
    }
    vector<poly>m(n * 4);//segtree build Q(x)
    auto build = [&](auto&&self, int l, int r, int p)->void
    {
        if(l == r){
            m[p] = poly{P - x[l], 1};
            return;
        }
        int mid = l + r >> 1;
        self(self, l, mid , p * 2);
        self(self, mid + 1, r, p * 2 + 1);
        m[p] = m[p * 2] * m[p * 2 + 1];
        return;
    };
    build(build, 0, n - 1, 1);
    auto dM = m[1].deriv();
    auto v = dM.eval(x);
    for(int i = 0;i < n;i++){
        v[i] = 1ll * power(v[i], P - 2) * y[i] % P;
    }

    auto work = [&](auto&&self, int l, int r, int p)->poly
    {
        if(l == r){
            return poly{v[l]};
        }
        int mid = l + r >> 1;
        return m[p * 2] * self(self, mid + 1, r, p * 2 + 1) + m[p * 2 + 1] * self(self, l, mid, p * 2);
    };
    return work(work, 0, n - 1, 1);
}

vector<int> valueShift(const vector<int>&v, int c)
{
    int n = v.size() - 1;
    poly a(n + 1),b(n * 2 + 1);
    //fac, invfac
    vector<int>fac(n + 1), invfac(n + 1);
    vector<int>cfac(n * 2 + 2), invcfac(n + 1); // base c - n - 1
    int cbase = c - n - 1;
    cfac[0] = fac[0] = 1;
    for(int i = 1;i <= n;i++){
        fac[i] = 1ll * fac[i - 1] * i % P;
    }
    invfac[n] = inv(fac[n]);
    for(int i = n - 1;i >= 0;i--){
        invfac[i] = 1ll * invfac[i + 1] * (i + 1) % P;
    }

    for(int i = 1;i <= n * 2 + 1;i++){
        cfac[i] = 1ll * cfac[i - 1] * (i + cbase) % P;
    }
    invcfac[n] = inv(cfac[n]);
    for(int i = n - 1;i >= 0;i--){
        invcfac[i] = 1ll * invcfac[i + 1] * (i + 1 + cbase) % P;
    }

    for(int i = 0;i <= n;i++){
        a[i] = 1ll * v[i] * invfac[i] % P * invfac[n - i] % P;
        if((n - i) % 2 == 1)a[i] = a[i] ? P - a[i] : 0;
    }
    for(int i = 0;i <= n * 2;i++){
        b[i] = inv(c - n + i);
    }
    auto h = a * b;
    vector<int>vc(n + 1);
    for(int i = 0;i <= n;i++){
        vc[i] = 1ll * h[i + n] * cfac[n + i + 1] % P * invcfac[i] % P;
    }
    return vc;
}

poly taylorShift(poly& f, int c)
{
    int n = f.size();
    poly a(n), b(n * 2 - 1);
    vector<int>fac(n + 1), invfac(n + 1);
    fac[0] = 1;
    for(int i = 1;i <= n;i++){
        fac[i] = 1ll * fac[i - 1] * i % P;
    }
    invfac[n] = inv(fac[n]);
    for(int i = n - 1;i >= 0;i--){
        invfac[i] = 1ll * invfac[i + 1] * (i + 1) % P;
    }
    for(int i = 0;i < n;i++){
        a[i] = 1ll * f[i] * fac[i] % P;
    }
    int cp = 1;
    for(int i = 0;i < n;i++){
        b[i] = 1ll * cp * invfac[i] % P;
        cp = 1ll * cp * c % P;
    }
    auto g = a.mulT(b);
    poly res(n);
    for(int i = 0;i < n;i++){
        res[i] = 1ll * g[i] * invfac[i] % P;
    }
    return res;
}

int bostanMori(poly& p, poly& q, i64 n) // calc [x^n] P(x)/Q(x)
{
    int k = q.size() - 1;
    while (n > 0) {
        poly nq = q;
        for (int i = 1; i <= k; i += 2) {
            nq[i] = -nq[i];
            if(nq[i] < 0)nq[i] += P;
        }
        auto np = p * nq;
        nq = q * nq;
        for (int i = 0; i < k; i++) {
            p[i] = np[i * 2 + n % 2];
        }
        for (int i = 0; i <= k; i++) {
            q[i] = nq[i * 2];
        }
        n /= 2;
    }
    return 1ll * p[0] * inv(q[0]) % P;
}

int linearRecurrence(poly a, poly c, i64 n) //a_0 ... a_{k - 1} , c_1 ... c_k
{
    assert(a.size() + 1 == c.size());
    assert(c[0] == 0 || c[0] == P - 1);
    c[0] = -1;
    int k = a.size();
    poly p = (a * (-c)).trunc(k), q = -c;
    return bostanMori(p, q, n);
}

poly berlekampMassey(const vector<int>& s) {
    if (s.empty()) {
        return poly{P - 1};
    }

    poly C{1};   // c[0] = 1
    poly B{1};   
    int L = 0;   
    int m = 1;   // dist from last update
    int b = 1;   // last discrepancy

    for (int n = 0; n < (int)s.size(); n++) {
        int d = 0;
        for (int i = 0; i <= L; i++) {
            if (n - i < 0) break;
            d = (d + 1ll * C[i] * s[n - i]) % P;
        }
        if (d == 0) {
            m++;
            continue;
        }

        poly T = C; 

        // coef = d / b
        int coef = 1LL * d * inv(b) % P;

        // C = C - coef * x^m * B
        if ((int)C.size() < (int)B.size() + m) C.resize(B.size() + m, 0);
        for (int i = 0; i < (int)B.size(); i++) {
            C[i + m] = C[i + m] - 1ll * coef * B[i] % P;
            if(C[i + m] < 0)C[i + m] += P;
        }

        if (2 * L <= n) {
            L = n + 1 - L;
            B = T;
            b = d;
            m = 1;
        } else {
            m++;
        }
    }
    return -C;
}

namespace compositon{
    using biPoly = vector<poly>; // sum_{j>=0} A[j](x) * y^j

    // (A*B) mod x^nx, y^my
    // packing: x^i y^j -> t^{i + j*block}
    biPoly biMul(const biPoly& A, const biPoly& B, int nx, int my) {
        int Ay = min<int>(A.size(), my);
        int By = min<int>(B.size(), my);
        if (Ay == 0 || By == 0) return {};
        int Cy = min<int>(Ay + By - 1, my);

        int block = 2 * nx - 1;           
        poly PA(Ay * block), PB(By * block);

        for (int j = 0; j < Ay; j++) {
            int lim = min<int>(nx, A[j].size());
            for (int i = 0; i < lim; i++) PA[j * block + i] = A[j][i];
        }
        for (int j = 0; j < By; j++) {
            int lim = min<int>(nx, B[j].size());
            for (int i = 0; i < lim; i++) PB[j * block + i] = B[j][i];
        }

        poly PC = PA * PB;

        biPoly C(Cy, poly(nx));
        for (int j = 0; j < Cy; j++) {
            int base = j * block;
            for (int i = 0; i < nx; i++) {
                int idx = base + i;
                if (idx < (int)PC.size()) C[j][i] = PC[idx];
            }
        }
        return C;
    }

    // Q(-x,y)
    biPoly neg_x(const biPoly& Q, int nx) {
        biPoly R = Q;
        for (auto& fx : R) {
            if ((int)fx.size() < nx) fx.resize(nx);
            for (int i = 1; i < nx; i += 2){
                fx[i] = -fx[i];
                if(fx[i] < 0)fx[i] += P;
            }
        }
        return R;
    }

    // V(x^2,y) = A(x,y)
    // 输入 A mod x^nx，输出 V mod x^{ceil(nx/2)}
    biPoly even_x(const biPoly& A, int nx) {
        int nx2 = (nx + 1) / 2;
        biPoly V(A.size(), poly(nx2));
        for (int j = 0; j < (int)A.size(); j++) {
            int lim = min<int>(nx, A[j].size());
            for (int i = 0; i < nx2; i++) {
                int k = 2 * i;
                if (k < lim) V[j][i] = A[j][k];
            }
        }
        return V;
    }

    // W(x^2,y) mod x^nx
    biPoly subst_x2(const biPoly& W, int nx) {
        biPoly R(W.size(), poly(nx));
        for (int j = 0; j < (int)W.size(); j++) {
            for (int i = 0; i < (int)W[j].size(); i++) {
                int k = 2 * i;
                if (k < nx) R[j][k] = W[j][i];
            }
        }
        return R;
    }

    // 取 y 的 [l, r) 段并左移：返回 size=r-l
    biPoly slice_y(const biPoly& A, int l, int r, int nx) {
        int len = max(0, r - l);
        biPoly R(len, poly(nx));
        for (int i = 0; i < len; i++) {
            int j = l + i;
            if (0 <= j && j < (int)A.size()) {
                R[i] = A[j].trunc(nx);
            }
        }
        return R;
    }

    // 忠实版 Comp：返回 F_{d,m}( P(y) / Q(x,y) ) mod x^nx
    // P 是 y 多项式（长度 m）；Q 是 biPoly（y>=0）
    // 返回 biPoly，size = m-d，且每项是 x 多项式（长度 nx）
    biPoly Comp(const poly& P, const biPoly& Q,int nx, int d, int m) {
        // base: x 只剩常数项
        if (nx == 1) {
            // Q0(y) = Q(0,y)
            poly Q0(m);
            int limy = min<int>(m, Q.size());
            for (int j = 0; j < limy; j++) {
                if (!Q[j].empty()) Q0[j] = Q[j][0];
            }
            // C(y) = P(y) / Q0(y) mod y^m
            poly C = (P * Q0.inv(m)).trunc(m);

            // 输出 y 的 [d,m)
            biPoly res(m - d, poly(1));
            for (int i = d; i < m; i++) res[i - d][0] = C[i];
            return res;
        }

        int degyQ = min(m - 1, (int)Q.size() - 1);
        int e = max(0, d - degyQ);

        biPoly Qneg = neg_x(Q, nx);

        // A = Q * Q(-x)  (mod x^nx, y^m)
        biPoly A = biMul(Q, Qneg, nx, m);

        // V(x,y) = A_even(x,y) with x := x^2 collapsed, so xlen halves
        biPoly V = even_x(A, nx);
        int nx2 = (nx + 1) / 2;

        // W = Comp(nx2, e, m, P, V)   (y窗口从 e 开始)
        biPoly W = Comp(P, V, nx2, e, m);

        // lift: W(x^2,y)
        biPoly W2 = subst_x2(W, nx);

        // B = W(x^2,y) * Q(-x,y) 只需要 y < (m-e)
        biPoly B = biMul(W2, Qneg, nx, m - e);

        // 返回 y 的 [d-e, m-e)
        return slice_y(B, d - e, m - e, nx);
    }

    poly comp(poly f, poly g, int n) {
        f = f.trunc(n);
        g = g.trunc(n);
        if ((int)g.size() < n) g.resize(n);

        int m = min<int>(n, f.size());
        // P(y) = y^{m-1} f(1/y)  <=> P[i] = f[m-1-i]
        poly P(m);
        for (int i = 0; i < m; i++) P[i] = f[m - 1 - i];

        // Q(x,y) = 1 - y g(x)
        biPoly Q(2, poly(n));
        Q[0][0] = 1;
        Q[1] = (-g).trunc(n);
        if ((int)Q[1].size() < n) Q[1].resize(n);

        // 取 y^{m-1}：即 d=m-1, 输出 size=1
        biPoly R = Comp(P, Q, n, m - 1, m);
        return R[0].trunc(n);
    }

    biPoly biMul_fullX(const biPoly& A, const biPoly& B, int nx_in, int my) {
        int Ay = std::min<int>((int)A.size(), my);
        int By = std::min<int>((int)B.size(), my);
        if (Ay == 0 || By == 0) return {};

        int Cy = std::min<int>(Ay + By - 1, my);
        int nx_out = 2 * nx_in - 1;

        // block must be >= nx_out to avoid x-carry crossing y-blocks
        int block = nx_out;

        poly PA(Ay * block), PB(By * block);

        for (int j = 0; j < Ay; j++) {
            int lim = std::min<int>(nx_in, (int)A[j].size());
            for (int i = 0; i < lim; i++) PA[j * block + i] = A[j][i];
        }
        for (int j = 0; j < By; j++) {
            int lim = std::min<int>(nx_in, (int)B[j].size());
            for (int i = 0; i < lim; i++) PB[j * block + i] = B[j][i];
        }

        poly PC = PA * PB;

        biPoly C(Cy, poly(nx_out));
        for (int j = 0; j < Cy; j++) {
            int base = j * block;
            for (int i = 0; i < nx_out; i++) {
                int idx = base + i;
                if (idx < (int)PC.size()) C[j][i] = PC[idx];
            }
        }
        return C;
    }

    // pick x-parity and downsample: keep coeffs x^{2t+parity} -> new[t]
    biPoly pick_parity_x(const biPoly& A, int parity, int nx_new) {
        biPoly R(A.size(), poly(nx_new));
        for (int j = 0; j < (int)A.size(); j++) {
            int lim = (int)A[j].size();
            for (int t = 0; t < nx_new; t++) {
                int idx = 2 * t + parity;
                if (idx < lim) R[j][t] = A[j][idx];
            }
        }
        return R;
    }

    // ans[k] = [x^n] f(x)^k * g(x), k=0..n
    // require f.size()==g.size()==n+1 and f[0]==0
    poly powerProjection(poly f, poly g) {
        int N = (int)f.size();
        int n = N - 1;
        // 当前 xlen = n+1, 当前 ylen = my
        int nx = n + 1;
        int my = std::min(N, 2); // 初始 Q = 1 - y f, 只需要 y^0..y^1

        biPoly P, Q;
        P.assign(1, g.trunc(nx)); // P(y) = g(x)
        Q.assign(2, poly(nx));    // Q(y) = 1 - y f(x)
        Q[0][0] = 1;
        Q[1] = (-f).trunc(nx);

        // 主循环：x-Bostan–Mori，目标提取 [x^n]
        while (n > 0) {
            // ylen 更新：乘法会让 ylen 至多变成 2*my-1，但我们只关心到 N 项
            int my2 = std::min(N, 2 * my - 1);

            // R = Q(-x,y)
            biPoly R = neg_x(Q, nx);

            // S = P * R , T = Q * R  (x full conv, y truncated)
            biPoly S = biMul_fullX(P, R, nx, my2);
            biPoly T = biMul_fullX(Q, R, nx, my2);

            int parity = (n & 1);
            int n2 = n >> 1;
            int nx2 = n2 + 1;

            // P <- pick parity row from S; Q <- even row from T
            P = pick_parity_x(S, parity, nx2);
            Q = pick_parity_x(T, 0, nx2);

            // 截 ylen 到 my2（pick_parity_x 保留了全部 y 行）
            if ((int)P.size() > my2) P.resize(my2);
            if ((int)Q.size() > my2) Q.resize(my2);

            // shrink n, nx, my
            n = n2;
            nx = nx2;
            my = my2;
        }

        // n == 0 => x 只剩常数项：结果是 y-series = P0(y)/Q0(y)
        // 取各 y^j 的 x^0 系数拼成 y 多项式
        poly Py(my), Qy(my);
        for (int j = 0; j < my; j++) {
            if (j < (int)P.size() && !P[j].empty()) Py[j] = P[j][0];
            if (j < (int)Q.size() && !Q[j].empty()) Qy[j] = Q[j][0];
        }

        poly invQy = Qy.inv(my);
        poly ans = (Py * invQy).trunc(N); // 需要 0..N-1 共 N 项（k=0..n）
        return ans;
    }

    // g = f^{<-1>} mod x^m, require f[0]=0 and f[1]!=0
    poly compInv(poly f, int m) {
        if (m <= 0) return poly();
        if (m == 1) return poly{0};

        f = f.trunc(m);
        // assert(f.size() >= 2 && f[0] == 0 && f[1] != 0);

        int c = f[1];
        // powerProjection 需要 size = (m+1): n = m
        poly fext = f;
        fext.resize(m + 1);          // f[m]=0 不影响 mod x^m 的 inverse
        poly one(m + 1);
        one[0] = 1;

        // hk[k] = [x^m] f(x)^k
        poly hk = powerProjection(fext, one); // length m+1

        // A(x) = (x/g)^m mod x^m  (length m)
        poly A(m);
        for (int k = 1; k <= m; k++) {
            int deg = m - k;
            // coeff = m/k * hk[k]
            int coef = (long long)m * inv(k) % P * hk[k] % P;
            A[deg] = coef;
        }

        // normalize by c^m so that B[0]=1
        int cpm = power(c, m);
        poly B = A / cpm;
        // (optional) assert(B[0] == 1);

        // H = x/g = c * exp( log(B)/m )
        poly L = (B.log(m) / m); // /m uses scalar inverse via your operator/(int)
        poly H = L.exp(m) * c;

        // g = x * inv(H)
        poly invH = H.inv(m);
        poly ginv = invH.shift(1).trunc(m);
        return ginv;
    }
}


void solve()
{
    
}

int main()
{
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    int tt = 1;
    //cin >> tt;
    while(tt--){
        solve();
    }
    return 0;
}
