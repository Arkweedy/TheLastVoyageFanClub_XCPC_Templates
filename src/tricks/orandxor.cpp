ll prexor(ll x) {
    if (x % 4 == 0)
        return x;
    else if (x % 4 == 1)
        return 1;
    else if (x % 4 == 2)
        return x ^ 1;
    else
        return 0;
}

ll askxor(ll l, ll r) { return prexor(r) ^ prexor(l - 1); }

ll askand(ll l, ll r) {
    ll res = 0;
    while (l) {
        ll t = l & (-l);
        if (l + t >= r) res += t;
        l ^= t;
    }
    return res;
}

ll askor(ll l, ll r) {
    while (l < r) l |= (l + 1);
    return l;
}