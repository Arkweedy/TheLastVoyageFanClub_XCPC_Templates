constexpr int ALPHA_SIZE = 26;
struct GSAM
{
    struct Node{
        array<int,ALPHA_SIZE>next;
        int link;
        int len;
        Node():next{},link{},len{}{}
    };
    vector<Node>t;
    static constexpr int root = 1;

    GSAM()
    {
        init();
    }

    void init()
    {
        t.assign(2,{});
        t[0].next.fill(1);
        t[0].len = -1;
    }

    int newNode()
    {
        t.emplace_back();
        return t.size() - 1;
    }

    int num(char c)
    {
        return c - 'a';
    }

    void insert(const vector<int>& v)//insert trie
    {
        int p = root;
        for(auto x : v){
            if(t[p].next[x] == 0){
                t[p].next[x] = newNode();
            }
            p = t[p].next[x];
        }
        return;
    }

    void insert(const string& s)//insert trie
    {
        int p = root;
        for(auto c : s){
            int x = num(c);
            if(t[p].next[x] == 0){
                t[p].next[x] = newNode();
            }
            p = t[p].next[x];
        }
        return;
    }

    int insertSAM(int last, int x)//return last
    {
        int cur = t[last].next[x];
        t[cur].len = t[last].len + 1;
        int p = t[last].link;
        while(p != 0 && t[p].next[x] == 0){
            t[p].next[x] = cur;
            p = t[p].link;
        }
       
        if(p == 0){
            t[cur].link = 1;
        }
        else{
            int q = t[p].next[x];
            if(t[p].len + 1 == t[q].len){
                t[cur].link = q;
            }
            else{
                int clone = newNode();
              
                //t[clone].next = t[q].next;
                for(int i = 0;i < ALPHA_SIZE;i++){
                    t[clone].next[i] = t[t[q].next[i]].len != 0 ? t[q].next[i] : 0;
                }
                t[clone].link = t[q].link;
                t[clone].len = t[p].len + 1;
                while(p != 0 && t[p].next[x] == q){
                    t[p].next[x] = clone;
                    p = t[p].link;
                }
                t[q].link = t[cur].link = clone;
            }
        }
        return cur;
    }

    void build()
    {
        queue<pair<int,int>>q;
        for(int i = 0;i < ALPHA_SIZE;i++){
            if(t[root].next[i] != 0){
                q.push(make_pair(root,i));
            }
        }

        while(!q.empty()){
            auto [la, x] = q.front();
            q.pop();
            int last = insertSAM(la,x);
            for(int i = 0;i < ALPHA_SIZE;i++){
                if(t[last].next[i] != 0){
                    q.push(make_pair(last,i));
                }
            }
        }
    }

    // inline int& next(const int& p,const char& c)noexcept
    // {
    //     return t[p].next[num(c)];
    // }
};