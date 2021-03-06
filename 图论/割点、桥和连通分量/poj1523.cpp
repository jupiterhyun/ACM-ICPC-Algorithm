#include<cstdio>
#include<vector>
#include<cstring>
#define min(a,b) (a<b)?a:b;
#define max(a,b) (a>b)?a:b;

using namespace std;

/*-----Cut_Bridge Template Start-----*/

const int MaxN = 1001;
//const int MaxE = MaxN*MaxN;

int vis[MaxN];  //v是否在栈中，0=未访问，1=在栈中，2=搜索完毕 
int dfn[MaxN];  //dfn[u]:遍历到u点的时间;
int low[MaxN];  //low[u]:u或u的子树能够追溯到的最早的栈中节点的次序号
int cut[MaxN];         //cut[i]=1说明第i个节点为割点
int root,BridgeNum;    //root为根下标
//int bridge[MaxN][2];   //桥 
vector<int> edge[MaxN];
 
void AddEdge(int u,int v)
{
    edge[u].push_back(v);
    edge[v].push_back(u);
}
 
void Cut_Bridge(int u, int fa, int dep)
{
    int v,i,son=0;
    vis[u]=1;           //当前点进栈 
    dfn[u]=low[u]=dep;  //开始时dfn[u]=low[u]
    for(i=0;i<edge[u].size();i++) //遍历所有边 
    {
        v=edge[u][i];
        if (vis[v]==1 && v!=fa)         //找到一个环（强连通分量） 
            low[u]=min(low[u],dfn[v]);  //更新当前点的low值 
        if (vis[v]==0)
        {
            Cut_Bridge(v,u,dep+1);      //向下遍历 
            son++;
            low[u]=min(low[u],low[v]);  //确保low[u]最小
            //割点 
            if ((u==root && son>1) || (u!=root && low[v]>=dfn[u])) cut[u]=1;
            /*边(u,v)是桥 
            if (low[v] > dfn[u])       
            {
                bridge[BridgeNum][0]=u;
                bridge[BridgeNum++][1]=v;   
            } */
        }
    }
    vis[u]=2;
}

void  InitCutBridge(int TargetRoot,int PointNum)
{
    int i;
    root=TargetRoot;
    memset(cut,0,sizeof(cut));
    memset(vis,0,sizeof(vis));
    memset(low,0,sizeof(low));
    memset(dfn,0,sizeof(dfn));
    for (i=1;i<=PointNum;i++) edge[i].clear(); //清空向量 
}

/*-----Cut_Bridge Template Over-----*/

void dfs(int u,int dep)
{
	int v,i;
	for(i=0;i<edge[u].size();i++) 
	{
	    v=edge[u][i];
	    if (0==vis[v]) 
	    {
	        vis[v]=dep;
	        dfs(v,dep);
	    }
	}
}

int main()
{
    int u,v,t=0,n,cutpnum,i,j,ans;
    freopen("poj1523.txt","r",stdin);
    freopen("poj1523ans.txt","w",stdout);
    while (scanf("%d",&u)!=EOF)
    {
    	n=0;
        InitCutBridge(1,MaxN);
        t++;
        if (0==u) break;
        if (t>1) printf("\n");
        printf("Network #%d\n",t);
        scanf("%d",&v);
        n=max(n,u);
        n=max(n,v);
        AddEdge(u,v);
        while (scanf("%d",&u)!=EOF)
        {
            if (0==u) break;
            scanf("%d",&v);
            n=max(n,u);
            n=max(n,v);
            AddEdge(u,v);
        }
        Cut_Bridge(1,1,1);
        cutpnum=0;
        for (i=1;i<=n;i++) if (cut[i])
        {
            cutpnum++;
            memset(vis,0,sizeof(vis));
            vis[i]=-1;
            ans=0;
            for (j=1;j<=n;j++) if (0==vis[j]) dfs(j,++ans);
            printf("  SPF node %d leaves %d subnets\n",i,ans);
        }
        if (0==cutpnum) printf("  No SPF nodes\n");
    }
}
