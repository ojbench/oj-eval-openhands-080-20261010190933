#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,m; if(!(cin>>n>>m)) return 0;
    vector<vector<int>> adj(n+1);
    vector<int> deg(n+1,0);
    for(int i=0;i<m;++i){
        int x,y;cin>>x>>y;
        adj[x].push_back(y);
        adj[y].push_back(x);
        deg[x]++; deg[y]++;
    }
    vector<char> removed(n+1,false);
    queue<int> q;
    for(int i=1;i<=n;++i) if(deg[i]<2){ q.push(i); removed[i]=true; }
    while(!q.empty()){
        int v=q.front(); q.pop();
        for(int u: adj[v]) if(!removed[u]){
            deg[u]--;
            if(deg[u]<2){ removed[u]=true; q.push(u); }
        }
    }
    vector<char> visited(n+1,false);
    vector<char> color(n+1,0);
    long long good=0; // vertices that can be invited
    for(int i=1;i<=n;++i){
        if(removed[i]||visited[i]) continue;
        // BFS component
        bool is_bipartite=true;
        vector<int> comp;
        queue<int> bfs; bfs.push(i); visited[i]=true; color[i]=0; comp.push_back(i);
        while(!bfs.empty()){
            int v=bfs.front(); bfs.pop();
            for(int u: adj[v]) if(!removed[u]){
                if(!visited[u]){ visited[u]=true; color[u]=color[v]^1; bfs.push(u); comp.push_back(u); }
                else if(color[u]==color[v]){ is_bipartite=false; }
            }
        }
        if(!is_bipartite){
            good += comp.size();
        }
    }
    long long answer = (long long)n - good;
    cout<<answer<<"\n";
    return 0;
}
