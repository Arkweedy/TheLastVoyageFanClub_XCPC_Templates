int min_pos(vector<int> a) { // 0-based
	int n = a.size(), i = 0, j = 1, k = 0;
	while (i < n && j < n && k < n) {
		auto u = a[(i + k) % n]; auto v = a[(j + k) % n];
		int t = u > v ? 1 : (u < v ? -1 : 0);
		if (t == 0) k++;
		else {
			if (t > 0) i += k + 1; else j += k + 1;
			if (i == j) j++;
			k = 0; 
		} 
	} return min(i, j); 
}

int Duval(const string& s)//return begin of min expression
{
    int n = s.length();
    int last = -1;
    for(int i = 0;i < n;){
        int j = i + 1, k = i;
        while(j < n && s[k] <= s[j]){
            if(s[j] > s[k])k = i;
            else k++;
            j++;
        }
        
        last = i;
        while(i <= k){//i + j - k <= j
            i += j - k;
        }
        if(j == n)break;//find last but not empty
    }
    return last;
};