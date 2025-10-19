//F[0] = 0
//f_n = f * g
auto cdq = [&](auto&&self, int l ,int r)->void
{
    if(l == r){
        //
        //f[l] += g[l] / fac[l - 1];
        return;
    }
    int m = l + r >> 1;
    self(self, l, m);
    //
    poly a(m - l + 1),b(r - l);
    //[l,m] * [1, r - l] -> [l + 1, r] -> [m + 1, r]
    for(int i = 0;i <= m - l;i++){
        a[i] = f[i + l];
    }
    for(int i = 0;i < r - l;i++){
        b[i] = g[i + 1];
    }
    auto c = a * b;
    for(int i = 0;i < r - m;i++){
        f[m + i + 1] += c[i + m - l];
    }
    self(self, m + 1, r);
    return;
};

cdq(cdq, 1, n);