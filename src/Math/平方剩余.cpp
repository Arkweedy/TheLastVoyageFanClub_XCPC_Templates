namespace cipolla {
// 二次剩余等价于 n^(p-1>>1)=1
// 先找a^2-n非二次剩余 定义w^2=a^2-n 则一解为(a+w)^(p+1>>1)
ll p, w;
struct Q  // 复数类
{
    ll x, y;
    Q operator*(const Q& o) const {
        return {(x * o.x + y * o.y % p * w) % p, (x * o.y + y * o.x) % p};
    }
};
ll qmi(ll x, int y) {
    ll r = 1;
    while (y) {
        if (y & 1) r = r * x % p;
        x = x * x % p;
        y >>= 1;
    }
    return r;
}
Q qmi(Q x, int y) {
    Q r = {1, 0};
    while (y) {
        if (y & 1) r = r * x;
        x = x * x;
        y >>= 1;
    }
    return r;
}
ll modsqrt(ll x, ll P)  // x [0,p)
{
    if (x == 0 || P == 2) return x;
    p = P;
    if (qmi(x, p - 1 >> 1) != 1) return -1;
    ll y;
    mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
    do y = rnd() % p, w = ((ll)y * y + p - x) % p;
    while (qmi(w, p - 1 >> 1) <= 1);  // not for p=2
    y = qmi({y, 1}, p + 1 >> 1).x;
    if (y * 2 > p) y = p - y;  // 两解取小
    return y;
}
}  // namespace cipolla
using cipolla::modsqrt;