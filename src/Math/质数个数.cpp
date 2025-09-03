//author : min_25

int isqrt(ll n) {
    return sqrtl(n);
}

ll prime_pi(const ll N) {
    if (N <= 1)
        return 0;

    const int v = isqrt(N);
    vector<int> smalls(v + 1);

    for (int i = 2; i <= v; ++i)
        smalls[i] = (i + 1) / 2;

    int s = (v + 1) / 2;
    vector<int> roughs(s);

    for (int i = 0; i < s; ++i)
        roughs[i] = 2 * i + 1;

    vector<ll> larges(s);

    for (int i = 0; i < s; ++i)
        larges[i] = (N / (2 * i + 1) + 1) / 2;

    vector<bool> skip(v + 1);
    auto divide = [](ll N, ll d) -> int { return double(N) / d; };

    int pc = 0;

    for (int p = 3; p <= v; ++p)
        if (smalls[p] > smalls[p - 1]) {
            int q = p * p;
            pc++;

            if (ll(q) * q > N)
                break;

            skip[p] = true;

            for (int i = q; i <= v; i += 2 * p)
                skip[i] = true;

            int ns = 0;

            for (int k = 0; k < s; ++k) {
                int i = roughs[k];

                if (skip[i])
                    continue;

                ll d = ll(i) * p;
                larges[ns] = larges[k] - (d <= v ? larges[smalls[d] - pc] : smalls[divide(N, d)]) + pc;
                roughs[ns++] = i;
            }

            s = ns;

            for (int i = v, j = v / p; j >= p; --j) {
                int c = smalls[j] - pc;

                for (int e = j * p; i >= e; --i)
                    smalls[i] -= c;
            }
        }

    larges[0] += ll(s + 2 * (pc - 1)) * (s - 1) / 2;

    for (int k = 1; k < s; ++k)
        larges[0] -= larges[k];

    for (int l = 1; l < s; ++l) {
        int q = roughs[l];
        ll M = N / q;
        int e = smalls[M / q] - pc;

        if (e < l + 1)
            break;

        ll t = 0;

        for (int k = l + 1; k <= e; ++k)
            t += smalls[divide(M, roughs[k])];

        larges[0] += t - ll(e - l) * (pc + l - 1);
    }

    return larges[0];
}