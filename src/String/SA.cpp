struct SA {
    int n;
    std::vector<int> sa, rk, lc;
    vector<vector<int>> st;
    /*
    sa[k] = 后缀排名为 k 的后缀起始位置（k = 0..n-1）
    rk[pos] = 后缀 s[pos..] 的排名（0..n-1）
    lc[i] = LCP(sa[i], sa[i+1])

    求任意两个后缀的 LCP：LCP(s[a, n], s[b, n])
    LCP(LCP(s[l], s[l + 1]) ... LCP(s[r - 1], s[r]))
    min(lc[l] ... lc[r-1]) = rmq(l, r-1)

    */
    SA(std::string s) {
        n = s.size();
        sa.resize(n);
        lc.resize(n - 1);
        rk.resize(n);
        std::iota(sa.begin(), sa.end(), 0);
        std::sort(sa.begin(), sa.end(),
                  [&](int a, int b) { return s[a] < s[b]; });
        rk[sa[0]] = 0;
        for (int i = 1; i < n; i++) {
            rk[sa[i]] = rk[sa[i - 1]] + (s[sa[i]] != s[sa[i - 1]]);
        }
        int k = 1;
        std::vector<int> tmp, cnt(n);
        tmp.reserve(n);
        while (rk[sa[n - 1]] < n - 1) {
            tmp.clear();
            for (int i = 0; i < k; i++) {
                tmp.push_back(n - k + i);
            }
            for (auto i : sa) {
                if (i >= k) {
                    tmp.push_back(i - k);
                }
            }
            std::fill(cnt.begin(), cnt.end(), 0);
            for (int i = 0; i < n; i++) {
                cnt[rk[i]]++;
            }
            for (int i = 1; i < n; i++) {
                cnt[i] += cnt[i - 1];
            }
            for (int i = n - 1; i >= 0; i--) {
                sa[--cnt[rk[tmp[i]]]] = tmp[i];
            }
            std::swap(rk, tmp);
            rk[sa[0]] = 0;
            for (int i = 1; i < n; i++) {
                rk[sa[i]] =
                    rk[sa[i - 1]] + (tmp[sa[i - 1]] < tmp[sa[i]] ||
                                     sa[i - 1] + k == n ||
                                     tmp[sa[i - 1] + k] < tmp[sa[i] + k]);
            }
            k *= 2;
        }
        for (int i = 0, j = 0; i < n; i++) {
            if (rk[i] == 0) {
                j = 0;
            } else {
                for (j -= j > 0; i + j < n && sa[rk[i] - 1] + j < n &&
                                 s[i + j] == s[sa[rk[i] - 1] + j];) {
                    j++;
                }
                lc[rk[i] - 1] = j;
            }
        }
    }

    void init_lcp()
    {
        constexpr int K = 21;//注意根据题目范围调整K大小，不同范围也可以取log计算。
        st.resize(K, vector<int>(n - 1));
        st[0] = lc;
        for (int j = 0; j < K - 1; j++) {
            for (int i = 0; i + (2 << j) <= n - 1; i++) {
                st[j + 1][i] = min(st[j][i], st[j][i + (1 << j)]);
            }
        }
    }
    
    int rmq(int l, int r) {
        int k = __lg(r - l);
        return min(st[k][l], st[k][r - (1 << k)]);
    };
    int lcp(int i, int j) {
        if (i == j || i == n || j == n) {
            return min(n - i, n - j);
        }
        int a = rk[i];
        int b = rk[j];
        if (a > b) {
            swap(a, b);
        }
        return min({n - i, n - j, rmq(a, b)});
    };
};
