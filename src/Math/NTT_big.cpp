constexpr int N = 1e5 + 10;
constexpr ll mod = 1231453023109121;
ll mul(ll a, ll b) { return (__int128)a * b % mod; }

ll qmi(ll a, ll k) {
    ll res = 1;
    while (k) {
        if (k & 1) res = mul(res, a);
        a = mul(a, a);
        k >>= 1;
    }
    return res;
}

const ll g = 3;
vector<ll> Omega(int L) {
    ll wn = qmi(g, mod / L);
    vector<ll> w(L);
    w[L >> 1] = 1;
    for (ll i = L / 2 + 1; i <= L - 1; i++) w[i] = mul(w[i - 1], wn);
    for (ll i = L / 2 - 1; i; i--) w[i] = w[i << 1];
    return w;
}
auto W = Omega(1 << 21);
void DIF(ll* a, ll n) {
    for (ll k = n >> 1; k; k >>= 1)
        for (ll i = 0, y; i < n; i += k << 1)
            for (ll j = 0; j < k; j++) {
                y = a[i + j + k];
                a[i + j + k] = mul(a[i + j] - y + mod, W[k + j]);
                a[i + j] = (y + a[i + j]) % mod;
            }
}
void IDIT(ll* a, ll n) {
    for (ll k = 1; k < n; k <<= 1)
        for (ll i = 0, x, y; i < n; i += k << 1)
            for (ll j = 0; j < k; j++) {
                x = a[i + j], y = mul(a[i + j + k], W[k + j]) % mod;
                a[i + j + k] = x - y < 0 ? x - y + mod : x - y;
                a[i + j] = (y + a[i + j]) % mod;
            }
    ll INV = mod - (mod - 1) / n;
    for (ll i = 0; i < n; i++) a[i] = mul(a[i], INV);
    reverse(a + 1, a + n);
}

using Poly = std::vector<ll>;
int norm(int n) { return 1 << (32 - __builtin_clz(n - 1)); }

Poly& dot(Poly& a, Poly& b) {
    for (int i = 0; i < a.size(); i++) a[i] = mul(a[i], b[i]);
    return a;
}
void DFT(Poly& a) { DIF(a.data(), a.size()); }
void IDFT(Poly& a) { IDIT(a.data(), a.size()); }

Poly operator*(Poly a, Poly b) {
    int n = a.size() + b.size() - 1, L = norm(n);
    if (a.size() <= 8 || b.size() <= 8) {
        Poly c(n);
        for (int i = 0; i < a.size(); i++)
            for (int j = 0; j < b.size(); j++)
                c[i + j] = (c[i + j] + mul(a[i], b[j])) % mod;
        return c;
    }
    a.resize(L);
    b.resize(L);
    DFT(a), DFT(b);
    dot(a, b);
    IDFT(a);
    return a.resize(n), a;
}