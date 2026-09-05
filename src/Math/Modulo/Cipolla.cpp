int powerF(int a, int b, int d, int p) {
    int ra = 1, rb = 0;
    while (p) {
        if (p & 1) {
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

int Cipolla(int a) {
    if (a == 0) return 0;
    int ea = power(a, (P - 1) / 2);
    if (ea == P - 1) return -1;
    int r, d, ed;
    mt19937 rng(random_device{}());
    do {
        r = rng() % P;
        d = (1ll * r * r - a + P) % P;
        ed = power(d, (P - 1) / 2);
    } while (ed == 1);
    int x = powerF(r, 1, d, (P + 1) / 2);
    return x;
}
