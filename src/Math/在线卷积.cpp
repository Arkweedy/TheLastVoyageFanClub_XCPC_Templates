poly F(n + 1), G(n + 1);
F[1] = 1;

auto convol = [&](int a, int b, int c, int d, int r)->void
{
    poly f(b - a + 1), g(d - c + 1);
    for(int i = a;i <= b;i++){
        f[i - a] = F[i]; 
    }
    for(int i = c;i <= d;i++){
        g[i - c] = G[i]; 
    }
    auto h = f * g;
    int l = min(a + d, b + c);
    //h[0] -> F[a + b]
    for(int i = l;i <= r;i++){
        F[i] += h[i - a - c];
    }
    return;
};

[&](this auto&&cdq, int l, int r)->void
{
    if(l == r){
        if(l != 1)F[l] /= (l - 1);
        for(int i = 1;i * l <= n;i++){
            G[i * l] += l * F[l];
        }
        return;
    }
    int m = l + r >> 1;
    cdq(l, m);
    if(l == 1){
        convol(l,m,l,m,r);
    }
    else{
        convol(l,m,1,r-l,r);
        convol(1,r-l,l,m,r);
    }
    cdq(m + 1, r);
    return;
}(1, n);