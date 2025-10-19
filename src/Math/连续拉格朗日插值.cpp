constexpr int mod=1000000007;
int qmi(int a,int b)
{
    int ans=1;
    while(b)
    {
        if(b&1) ans=1ll*ans*a%mod;
        a=1ll*a*a%mod;b>>=1;
    }
    return ans;
}
int inv(int p) {return qmi(p,mod-2);}
//n次多项式   n+1个点值    (0~n)
struct Lagrange{
    int n;
    vector<int > y,fac,invfac;
    Lagrange(int n){
        this->n=n;
        y.resize(n+1);
        fac.resize(n+1);
        invfac.resize(n+1);
        init(n);
    }
    void init(int n){
        fac[0]=1;
        for(int i=1;i<=n;i++) fac[i]=1ll*fac[i-1]*i%mod;
        invfac[n]=inv(fac[n]);
        for(int i=n-1;i>=0;i--) invfac[i]=1ll*invfac[i+1]*(i+1)%mod;
    }
    int solve(int k)
    {
        if(k<=n) return y[k];

        vector<int> pre(n+1),suf(n+1);
        int ans=0;
        for(int i=0;i<=n;i++) pre[i]=suf[i]=(k-i);

        for(int i=1;i<=n;i++) pre[i]=1ll*pre[i-1]*pre[i]%mod;
        for(int i=n-1;i>=0;i--) suf[i]=1ll*suf[i+1]*suf[i]%mod;
       
        for(int i=0;i<=n;i++)
        {
            int con=y[i];
            if(i) con=1ll*con*pre[i-1]%mod*invfac[i]%mod;
            if(i<n) con=1ll*con*suf[i+1]%mod*invfac[n-i]%mod;
            if((n-i)&1) con=(mod-con)%mod;
            ans=(ans+con)%mod;
        }
        return ans;
    }

};
