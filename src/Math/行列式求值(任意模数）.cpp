int cal(int a[][N], register int n, const int p) {
    register int i, j, k, r = 1, fh = 0, l;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) a[i][j] = (a[i][j] % p + p) % p;
    for (i = 1; i <= n; i++) {
        k = i;
        for (j = i; j <= n; j++)
            if (a[j][i]) {
                k = j;
                break;
            }
        if (a[k][i] == 0) return 0;
        for (++j; j <= n; j++)
            if (a[j][i] && a[j][i] < a[k][i]) k = j;
        if (i != k) {
            swap(a[k], a[i]);
            fh ^= 1;
        }
        for (j = i + 1; j <= n; j++) {
            if (a[j][i] > a[i][i]) {
                swap(a[j], a[i]);
                fh ^= 1;
            }
            while (a[j][i]) {
                l = a[i][i] / a[j][i];
                for (k = i; k <= n; k++)
                    a[i][k] = (a[i][k] + (ll)(p - l) * a[j][k]) % p;
                swap(a[j], a[i]);
                fh ^= 1;
            }
        }
        r = (ll)r * a[i][i] % p;
    }
    if (fh) return (p - r) % p;
    return r;
}