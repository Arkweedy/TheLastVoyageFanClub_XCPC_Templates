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
Z min_25(ll n)
{
    const int m = (int)floorl(sqrtl(n));

    // -------- Step 0. 先在“整数处”初始化两套多项式型前缀和 ----------
    // 约定：
    //   g0[t] = sum_{i<=t} g(i) - g(1)
    //   g1[j] = sum_{i<=floor(N/j)} g(i) - g(1)
    //   h0/h1 同理
    // 注意：减去 g(1), h(1) 是为了后面“差分判素”的一致性（1 的贡献去掉）。
    //
    // 【本题】g(i)=i, h(i)=i^2 （因为我们想要在“质数处”得到 ∑p 与 ∑p^2）。
    vector<Z> g0(m + 1), g1(m + 1), h0(m + 1), h1(m + 1);
    for (int i = 1; i <= m; ++i) {
        ll x = n / i;
        g0[i] = Z(i) * (i + 1) / 2 - 1;                     // ∑_{t<=i} t - 1
        g1[i] = Z(x) * (x + 1) / 2 - 1;                     // ∑_{t<=floor(N/i)} t - 1
        h0[i] = Z(i) * (i + 1) * (i * 2 + 1) / 6 - 1;       // ∑_{t<=i} t^2 - 1
        h1[i] = Z(x) * (x + 1) * (x * 2 + 1) / 6 - 1;       // ∑_{t<=floor(N/i)} t^2 - 1
    }

    // -------- Step 1. 将整数和改写为“只含质数”的和 ----------
    // 经典分法：
    //   A 段：用 h1/g1 改 h1/g1（大值域自洽，p*j<=m）
    //   B 段：用 h0/g0 改 h1/g1（跨链，p*j>m）
    //   C 段：用 h0/g0 改 h0/g0（小值域自洽）
    // 递推等式（以 g 为例，h 同理）：
    //   G(n, a+1) = G(n, a) - g(p) * ( G(floor(n/p), a) - G(p-1, a) ),
    //   仅在 p^2 <= n 时有效（否则“最小质因子为 p”的数不存在）。
    for (int p : prime) {
        ll p2 = 1ll * p * p;
        if (p2 > n) break;            // 只需处理到 p^2 <= N
        Z gp = Z(p), hp = Z(p) * p;   // g(p)=p, h(p)=p^2

        // --- A + B：更新大值域链 g1/h1 ---
        for (int j = 1; j <= m && 1ll * p2 * j <= n; ++j) {
            // 加上 “+ g(p) * G(p-1)” 这一项（对应 -(-g(p)*G(p-1)) 的那部分）
            g1[j] += gp * g0[p - 1];
            h1[j] += hp * h0[p - 1];

            // 再减去 g(p)*G(floor((N/j)/p)) —— 依据落点决定用 g1（大值域）还是 g0（小值域）
            if (1ll * p * j <= m) {               // A 段：仍在大值域
                g1[j] -= gp * g1[p * j];          // floor((N/j)/p) == floor(N/(p*j)) 映到 g1[p*j]
                h1[j] -= hp * h1[p * j];
            } else {                               // B 段：跨回小值域
                g1[j] -= gp * g0[(n / p) / j];    // floor((N/j)/p) == floor((N/p)/j) 映到 g0[(N/p)/j]
                h1[j] -= hp * h0[(n / p) / j];
            }
        }

        // --- C：小值域自洽，更新 g0/h0 的 [p^2..m] 区间 ---
        for (int j = m; j >= (int)p2; --j) {
            g0[j] -= gp * (g0[j / p] - g0[p - 1]);
            h0[j] -= hp * (h0[j / p] - h0[p - 1]);
        }
    }

    // 到此，g0/g1, h0/h1 都已经变成“只含质数”的和：
    //   g0[i] = sum_{p<=i} p,     g1[j] = sum_{p<=floor(N/j)} p
    //   h0[i] = sum_{p<=i} p^2,   h1[j] = sum_{p<=floor(N/j)} p^2
    // 因而“只在质数处的 f(p)”就可由它们线性组合得到：
    vector<Z> fprime0(m + 1), fprime1(m + 1);
    for (int i = 1; i <= m; ++i) {
        fprime0[i] = h0[i] - g0[i];             // ∑_{p<=i} (p^2 - p)
        fprime1[i] = h1[i] - g1[i];             // ∑_{p<=floor(N/i)} (p^2 - p)
    }

    // -------- Step 2. 最小质因子分层递归，把“合数”补回来 ----------
    // 记 F(x,k) = ∑_{ n : 1<=n , pmin(n) >= p_k , x*n <= N } f(n)
    // 我们传进来的 x 是“当前积”的分母（即在原式里考察 floor(N/(x·...)) 的那个 x）。
    //
    // 起始段（只含质数）：
    //   ∑_{p_k <= p <= floor(N/x)} f(p)
    //   = (fprime1[x] or fprime0[N/x])  - (k>0 ? fprime0[p_{k-1}] : 0)
    //
    // 递推段（枚举最小质因子 p_i 及其幂 p_i^c）：
    //   对每个 i>=k, 只要 p_i^2 * x <= N：
    //     对 c>=1，满足 p_i^{c+1} * x <= N：
    //       res += f(p_i^c) * F(x * p_i^c, i+1)   // 乘上“剩余”的贡献
    //            + f(p_i^{c+1});                  // 纯质数幂项的校正
    //
    // 【本题】f(p^c) = p^{2c} - p^c = (pc)*(pc-1)，直接代入即可。
    auto F = [&](auto&& self, ll x, int k) -> Z {
        Z res = 0;

        // ---- 起始段：只含质数 p in [p_k, floor(N/x)] 的那一段 ----
        if (k > 0) res -= fprime0[ prime[k - 1] ];      // 去掉 p < p_k 的质数
        if (x <= m) res += fprime1[x];                  // 加上 p <= floor(N/x)
        else        res += fprime0[n / x];

        // ---- 递推段：按最小质因子 p_i ≥ p_k 分层，枚举其幂 ----
        for (int i = k, p = prime[i]; 1ll * p * p * x <= n; ++i, p = prime[i]) {
            ll pc = p;                                  // pc = p^c
            for (int c = 1; pc * p * x <= n; ++c, pc *= p) {
                // f(p^c) = pc^2 - pc = pc*(pc-1)
                res += Z(pc) * (pc - 1) * self(self, x * pc, i + 1)   // 按 pc 分裂出的“剩余”
                     +  Z(pc * p) * (pc * p - 1);                      // 加上 f(p^{c+1}) 的纯质数幂校正
            }
        }
        return res;
    };

    // F(1,0) 覆盖了所有 n≥2 的贡献；若 f(1)≠0 需要手动补上， 积性函数均满足f(1) = 1
    return F(F, 1, 0) + 1;   // 这里 f(1)=1（约定），故 +1
}