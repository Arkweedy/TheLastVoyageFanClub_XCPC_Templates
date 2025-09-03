class Hash
{
    static constexpr int P1 = 1000000007;
    static constexpr int P2 = 1000000009;

    static constexpr int b1 = 131;
    static constexpr int b2 = 13331;

    int n;
    int rb1, rb2;
    bool use_random_base;

    // int rp1, rp2;
    // bool use_random_mod;

    vector<int>h1,p1,h2,p2;

public:

    Hash(string& s)//default 
    {
        n = s.length();
        h1.resize(n);
        p1.resize(n);
        h2.resize(n);
        p2.resize(n);

        p1[0] = 1;
        h1[0] = s[0];
        p2[0] = 1;            
        h2[0] = s[0];
        for (int i = 1; i < n; ++i) {
            p1[i] = 1ll * p1[i-1] * b1 % P1;
            p2[i] = 1ll * p2[i-1] * b2 % P2;
            h1[i] = (1ll * h1[i-1] * b1 + s[i]) % P1;
            h2[i] = (1ll * h2[i-1] * b2 + s[i]) % P2;
        }
    }

    Hash(string& s, bool is_random)// random base
    {
        mt19937 g(chrono::steady_clock::now().time_since_epoch().count());
        rb1 = g() % b1 + b1;
        rb2 = g() % b2 + b2;
        use_random_base = 1;

        n = s.length();
        h1.resize(n);
        p1.resize(n);
        h2.resize(n);
        p2.resize(n);

        p1[0] = 1;
        h1[0] = s[0];
        p2[0] = 1;            
        h2[0] = s[0];
        for (int i = 1; i < n; ++i) {
            p1[i] = 1ll * p1[i-1] * rb1 % P1;
            p2[i] = 1ll * p2[i-1] * rb2 % P2;
            h1[i] = (1ll * h1[i-1] * rb1 + s[i]) % P1;
            h2[i] = (1ll * h2[i-1] * rb2 + s[i]) % P2;
        }
    }

    Hash(string& s, int rb1_, int rb2_)// diy 
    {
        rb1 = rb1_;
        rb2 = rb2_;
        use_random_base = 1;

        n = s.length();
        h1.resize(n);
        p1.resize(n);
        h2.resize(n);
        p2.resize(n);

        p1[0] = 1;
        h1[0] = s[0];
        p2[0] = 1;            
        h2[0] = s[0];
        for (int i = 1; i < n; ++i) {
            p1[i] = 1ll * p1[i-1] * rb1 % P1;
            p2[i] = 1ll * p2[i-1] * rb2 % P2;
            h1[i] = (1ll * h1[i-1] * rb1 + s[i]) % P1;
            h2[i] = (1ll * h2[i-1] * rb2 + s[i]) % P2;
        }
    }

    pair<int,int> get(int l, int r)
    {
        int hs1 = h1[r], hs2 = h2[r];
        if(l)hs1 -=  1ll * h1[l - 1] * p1[r - l + 1] % P1, hs2 -= 1ll * h2[l - 1] * p2[r - l + 1] % P2;
        if(hs1 < 0)hs1 += P1;
        if(hs2 < 0)hs2 += P2;
        return make_pair(hs1,hs2);
    }

    int lcp(int x, int y)
    {
        int l = 0, r = min(n - x, n - y) + 1;
        while(r - l > 1){
            int m = l + r >> 1;
            if(get(x, x + m - 1) == get(y, y + m - 1)){
                l = m;
            }
            else{
                r = m;
            }
        }
        return l;
    }

    int lcs(int x, int y)
    {
        int l = 0, r = min(x + 1, y + 1) + 1;
        while(r - l > 1){
            int m = l + r >> 1;
            if(get(x - m + 1, x) == get(y - m + 1, y)){
                l = m;
            }
            else{
                r = m;
            }
        }
        return l;
    }
};