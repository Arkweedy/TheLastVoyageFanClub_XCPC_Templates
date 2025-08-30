//Duval 算法实现的Chen–Fox–Lyndon 分解
//返回所有的CFL分解中Lyndon串的位置
//0 base 闭区间
vector<pair<int,int>>Duval(const string& s)
{
    int n = s.length();
    vector<pair<int,int>>lyn;//
    for(int i = 0;i < n;){
        int j = i + 1, k = i;
        while(j < n && s[k] <= s[j]){
            if(s[j] > s[k])k = i;
            else k++;
            j++;
        }
        while(i <= k){//i + j - k <= j
            lyn.emplace_back(i, i + j - k - 1);
            i += j - k;
        }
    }
    return lyn;
};