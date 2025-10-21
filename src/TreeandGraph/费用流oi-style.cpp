#define maxn 5050
#define maxm 50005
#define INF 0x3f3f3f3f
using namespace std;
int s,t,n,m,head[maxn],pre[maxn],dis[maxn],q[maxn];
bool vis[maxn];
int s_e;
struct E{
	int v,c,w,nxt;
}e[maxm*2];
struct Max_fei{//本人喜欢结构体
	inline void a_e(int u,int v,int c,int w){
		e[s_e]=(E){v,c,w,head[u]};
		head[u]=s_e++;
	}
	inline void add(int u,int v,int c,int w){
		a_e(u,v,c,w);
		a_e(v,u,0,-w);
	}
	inline bool spfa(){
		for(int i=1;i<=n;i++){
			dis[i]=INF;
			vis[i]=false;
		}
		dis[s]=0;
		vis[s]=true;
		q[0]=s;
		int hd=0,tl=1;
		while(hd^tl){
			int u=q[hd++];//循环队列
			hd%=maxn;
			for(int i=head[u];i!=-1;i=e[i].nxt){
				int v=e[i].v;
				if(dis[v]>dis[u]+e[i].w&&e[i].c){//判断水管还能运水吗
					dis[v]=dis[u]+e[i].w;//更新
					pre[v]=i;//记录位置
					if(vis[v])continue;//如果在队里，那就不进队
					vis[v]=true;
					q[tl++]=v;
					tl%=maxn;
				}
			}
			vis[u]=false;
		}
		if(dis[t]==INF)return false;
		return true;
	}
	inline int end(int &flow){//flow求最大流
		int p,u,Min=1e9,ans=0;
		for(u=t;u!=s;u=e[p^1].v){//因为开始值为0，可以用xor来找反边
			p=pre[u];//往前找
			Min=min(Min,e[p].c);//找全部经过水管都能流过的最大流
            
		}
		for(u=t;u!=s;u=e[p^1].v){
			p=pre[u];
			e[p].c-=Min;
			e[p^1].c+=Min;
			ans+=e[p].w*Min;//加费用
		}
		flow+=Min;//加最大流
		return ans;
	}
	inline int solve(int &flow){
		int ans=0;
		while(spfa()){
			ans+=end(flow);
		}
		return ans;
	}
};
//for(int i=1;i<=n;i++) head[i]=-1;
//s_e=0;