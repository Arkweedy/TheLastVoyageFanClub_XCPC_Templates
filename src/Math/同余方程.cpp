std::pair<ll, ll> exgcd(ll a, ll b, ll c) {
    assert(a || b);
    ll d = std::gcd(a, b);
    if (c % d) return {-1, -1}; 
    if (!b) return {c / a, 0};
    ll x = 1, x1 = 0, p = a, q = b, k;
    ll mod = std::abs(b / d);
    while (b) {
        k = a / b;
        x -= k * x1;
        a -= k * b;
        std::swap(x, x1);
        std::swap(a, b);
    }
    x = x * (c / d) % mod;
    if (x < 0) x += mod;
    return {x, mod};
}


//a*x===b(mod c)
pair<ll,ll> cal(ll a,ll b,ll c) //0<=r<p  /  (-1,-1)
{
    auto [x,mod]=exgcd(a,c,b);
    return {x,mod};
}