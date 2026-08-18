// mu Mobius Function
// phi Euler Function
// me Mertens Function
poly Cyclotomic(int n)
{
    if(n == 1)return poly{P - 1, 1};
    int m = phi[n] / 2 + 1;
    poly a(phi[n] + 1);
    a[0] = 1;
    auto mul = [&](int k)->void // *(1 - x^k)
    {
        for(int i = m - 1;i >= k;i--){
            a[i] = a[i] - a[i - k];
            if(a[i] < 0)a[i] += P;
        }
    };
    auto div = [&](int k)->void // /(1 - x^k) = *(\sum x^ik)
    {
        for(int i = k;i < m;i++){
            a[i] = a[i] + a[i - k];
            if(a[i] >= P)a[i] -= P;
        }
    };
    for(int i = 1;i * i <= n;i++){
        if(n % i == 0){
            if(mu[i] == 1)mul(n / i);
            else if(mu[i] == -1)div(n / i);
            if(i * i != n){
                if(mu[n / i] == 1)mul(i);
                else if(mu[n / i] == -1)div(i);
            }
        }
    }
    for(int i = a.size() - 1;i >= m;i--){
        a[i] = a[phi[n] - i];
    }
    return a;
}

poly CyclotomicProd(int n)
{
    int m = 0;
    for(int i = 1;i <= n;i++){
        m += phi[i];
    }
    poly lpc(m + 1);
    for(int i = 1;i <= n;i++){
        for(int k = 1;i * k <= m;k++){
            lpc[i * k] = (lpc[i * k] - 1ll * me[n / i] * comb.inv(k) % P + P) % P;
        }
    }
    poly pc = -lpc.exp2(m + 1);
    return pc;
}
