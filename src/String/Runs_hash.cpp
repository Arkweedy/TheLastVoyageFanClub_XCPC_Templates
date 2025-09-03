    string s;
    cin>>s;
    int n = s.length();

    constexpr int P1 = 1000000007u;
    constexpr int P2 = 1000000009u;
    constexpr int b1 = 131;
    constexpr int b2 = 13331;

    // 0-based prefixes on s[0..n-1]; sentinel s[n] exists but is NOT hashed.
    vector<int> p1(n), h1(n), p2(n), h2(n);
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

    // get hash of 0-based substring [l..r]; allow l>r/negative => 0
    auto get = [&](int l, int r)->pair<int,int>
    {
        int hs1 = h1[r], hs2 = h2[r];
        if(l)hs1 -=  1ll * h1[l - 1] * p1[r - l + 1] % P1, hs2 -= 1ll * h2[l - 1] * p2[r - l + 1] % P2;
        if(hs1 < 0)hs1 += P1;
        if(hs2 < 0)hs2 += P2;
        return make_pair(hs1,hs2);
    };

    auto lcp = [&](int x, int y)->int //return len(lcp(x, y))
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
    };

    auto lcs = [&](int x, int y)->int //return len(lcs(x, y))
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
    };

    auto get_lyndon_suffix = [&](bool order)->vector<int>
    {
        vector<int>lyn(n);
        stack<int>stk;
        for(int i = n - 1;i >= 0;i--){
            lyn[i] = i;
            while(!stk.empty()){
                int j = stk.top();
                int len = lcp(i, j);
                if(order == 0){
                    if(s[i + len] > s[j + len]){//no
                        lyn[i] = j - 1;
                        break;
                    }
                    else{
                        lyn[i] = lyn[j];
                        stk.pop();
                    }
                }
                else{
                    if(s[i + len] < s[j + len]){//no
                        lyn[i] = j - 1;
                        break;
                    }
                    else{
                        lyn[i] = lyn[j];
                        stk.pop();
                    }
                }
            }
            stk.push(i);
        }
        return lyn;
    };

    auto lyn0 = get_lyndon_suffix(0);
    auto lyn1 = get_lyndon_suffix(1);

    vector<array<int,3>>runs;
    for(int i = 0;i < n;i++){
        int j,l,r,p;
        j = lyn0[i];//period is j - i + 1, compare i, j + 1 ([i,j] is cyc)
        l = i - 1 - lcs(i - 1, j) + 1, r = j + 1 + lcp(i, j + 1) - 1,p = j - i + 1;
        if(r - l + 1 >= 2 * (j - i + 1)){
            runs.push_back({l, r, p});
            //runs.push_back({l,r,j - i + 1});
        }

        j = lyn1[i];
        l = i - 1 - lcs(i - 1, j) + 1, r = j + 1 + lcp(i, j + 1) - 1,p = j - i + 1;
        if(r - l + 1 >= 2 * (j - i + 1)){
            runs.push_back({l, r, p});
            //runs.push_back({l,r,j - i + 1});
        }
    }

    sort(runs.begin(),runs.end());
    runs.erase(unique(runs.begin(),runs.end()),runs.end());

    vector<array<int,3>>nruns;
    for(int i = 0;i < runs.size();i++){
        if(i == 0 || runs[i][0] != runs[i - 1][0] || runs[i][1] != runs[i - 1][1])nruns.push_back(runs[i]);
    }
    runs = move(nruns);