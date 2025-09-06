// ---------------------------  这一段是模板化注释  ---------------------------
// min_25 主体：返回  sum_{n<=N} f(n)  的值（本题 f(p^a)=p^{2a}-p^a）。
//
// min_25 的套路：
//   (1) 先“只做质数处”的和：把  G(x)=sum_{p<=x} g(p), H(x)=sum_{p<=x} h(p) 这些
//       通过“值域压缩 + 素数筛改写”得到（代码里的 g0/g1, h0/h1）。
//       ——对当前这题：g(p)=p, h(p)=p^2，于是  f(p)=h(p)-g(p)=p^2-p。
//   (2) 再用按“最小质因子分层”的递归，把合数按 p^c·y（p<minpf(y)）拆回去。
//       递归形态：
//         F(x, k) = ∑_{p_i >= p_k, p_i^2 x <= N} [ ∑_{c>=1, p_i^{c+1}x<=N} ( f(p_i^c)*F(x·p_i^c, i+1) + f(p_i^{c+1}) ) ]
//       并配上“只含质数”的起始段（即 ∑_{p_k <= p <= N/x} f(p) ）。
//       ——这一步就是下面 Lambda `F` 那个递归。
// ---------------------------------------------------------------------------

constexpr int P = 1000000007;
using Z = MInt<P>;

constexpr int N = 1e5 + 10;//初始化sqrt以内的质数
vector<int>prime;
bitset<N + 1>isprime;

//记得在main里面调用一下seive
void seive()
{
    isprime.set();
    isprime[1] = 0;
    for(int i = 2;i <= N;i++){
        if(isprime[i]){
            prime.push_back(i);
        }

        for(auto p : prime){
            if(i * p > N)break;
            isprime[i * p].flip();
            if(i % p == 0)break;
        }
    }
}

Z min_25(ll n)//若要加上其他参数，选一个不重复的，如t,mod等等,不要用k,m,c
{
    int m = floorl(sqrtl(n));
    // f(p^k) = p^k(p^k - 1) 
    // g(p) = p,  h(p) = p^2

    // g(p) = p^a ,可以用自然数的幂拼出来f在质数处的点值
    // 其他的点值(质数幂和其他)不用管是什么形式，只关注质数处的点值形式即可


    vector<Z>g0(m + 1),g1(m + 1),h0(m + 1),h1(m + 1);

    //这里是 g(p) = p, h(p) = p^2 
    //也可以有g(p) = 1 等等
    for(int i = 1;i <= m;i++){
        ll x = n / i; // long long !!!
        g0[i] = Z(i) * (i + 1) / 2 - 1;
        g1[i] = Z(x) * (x + 1) / 2 - 1;
        h0[i] = Z(i) * (i + 1) * (i * 2 + 1) / 6 - 1;
        h1[i] = Z(x) * (x + 1) * (x * 2 + 1) / 6 - 1;
    }

    for(auto p : prime){
        ll pp = 1ll * p * p;//用来判定是否需要更新
        Z gp = p; // g(p),根据情况修改
        Z hp = Z(p) * p; // h(p),根据情况修改
        if(pp > n)break;

        //若 gp/hp = 1, 可省略
        //update 1
        for(int j = 1;j <= m && j * pp <= n;j++){
            g1[j] += gp * g0[p - 1];
            h1[j] += hp * h0[p - 1];
            if(1ll * p * j <= m){
                g1[j] -= gp * g1[p * j];
                h1[j] -= hp * h1[p * j];
            }
            else{
                g1[j] -= gp * g0[n / p / j];
                h1[j] -= hp * h0[n / p / j];
            } 
        }

        //update 0
        for(int j = m;j >= pp;j--){
            g0[j] -= gp * (g0[j / p] - g0[p - 1]);
            h0[j] -= hp * (h0[j / p] - h0[p - 1]);
        }
    }

    //迭代完后，这里的g,h只剩质数处的点值
    //fprime 使用前面准备好的g，h， 拼出来即可
    //如这里是f(p) = p^2 - p
    //如果需要特判一些位置的点值，记得处理好边界
    vector<Z>fprime0(m + 1),fprime1(m + 1);
    for(int i = 1;i <= m;i++){
        fprime0[i] = h0[i] - g0[i];
        fprime1[i] = h1[i] - g1[i];
    }

    //calc f_0
    //F(n, k) = \sum_p \sum c f(p^c)F(n / p^c, k + 1) + f(p ^ (c + 1))

    //这里要关注质数幂处的点值
    auto F = [&](auto&&self, ll x, int k)->Z //F(n / x, k)
    {
        Z res = 0;
        if(k > 0)res -= fprime0[prime[k - 1]];
        if(x <= m)res += fprime1[x];
        else res += fprime0[n / x];

        //若f(p^c) 需要O(log)直接求，可O(1)递推，可以在循环时维护
        //像mu这种特殊的，c>1时为0，可以直接去掉第二重循环
        for(int i = k, p = prime[i]; 1ll * p * p * x <= n;++i, p = prime[i]){
            ll pc = p;//记录p^c,以便判断什么时候终止
            for(int c = 1; pc * p * x <= n;c++, pc *= p){
                res += /* f(p^c) */ * self(self, x * pc, i + 1) + /* f(p^(c+1)) */;
            }
        }
        return res;
    };

    //F(F,1,0)代表2~(n/1)的值,要加上f(1) = 1
    return F(F, 1, 0) + 1;
}