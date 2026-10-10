#include <bits/stdc++.h>
using namespace std;

// Solution: Count ministers that can never be invited.
// A minister can be invited iff it belongs to some odd cycle in the graph
// formed by the given "non‑witness" edges. Vertices that are not part of any
// odd cycle are impossible to include in a valid odd‑length seating circle.
//
// Approach:
// 1. Build the undirected graph with n vertices and m edges.
// 2. Find all bridges (edges that are not part of any cycle) using a standard
//    DFS low‑link algorithm.
// 3. Build a DSU (union‑find) that unites vertices connected by non‑bridge
//    edges. Each resulting component consists solely of edges that lie on at
//    least one cycle.
// 4. For each component, check if it is bipartite by BFS coloring using only
//    the non‑bridge edges. If a component is NOT bipartite, it contains an odd
//    cycle, and every vertex in that component belongs to some odd cycle.
// 5. The answer is n minus the number of vertices that belong to a non‑bipartite
//    component.
//
// Complexity: O(n + m) time and O(n + m) memory.

struct DSU {
    vector<int> p, sz;
    DSU(int n=0){init(n);}    
    void init(int n){p.resize(n+1); sz.assign(n+1,1); iota(p.begin(), p.end(), 0);}    
    int find(int x){return p[x]==x?x:p[x]=find(p[x]);}
    void unite(int a,int b){a=find(a); b=find(b); if(a==b) return; if(sz[a]<sz[b]) swap(a,b); p[b]=a; sz[a]+=sz[b];}
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,m; if(!(cin>>n>>m)) return 0;
    vector<vector<int>> adj(n+1);
    vector<pair<int,int>> edges; edges.reserve(m);
    for(int i=0;i<m;++i){
        int x,y;cin>>x>>y;
        adj[x].push_back(y);
        adj[y].push_back(x);
        edges.emplace_back(x,y);
    }
    // ---------- find bridges ----------
    vector<int> disc(n+1,0), low(n+1,0);
    vector<char> isBridge(m,false);
    int timer=0;
    function<void(int,int)> dfs = [&](int v,int parentEdge){
        disc[v]=low[v]=++timer;
        for(int to: adj[v]){
            // find edge index
            // we will locate index by scanning edges list (inefficient but acceptable for constraints)
            // To improve, we could store edge ids in adjacency, but for simplicity we map via a map.
        }
    };
    // To efficiently get edge ids, build adjacency with pair (neighbor, edge_id)
    vector<vector<pair<int,int>>> g(n+1);
    for(int i=0;i<m;++i){
        int u=edges[i].first, v=edges[i].second;
        g[u].push_back({v,i});
        g[v].push_back({u,i});
    }
    function<void(int,int)> dfs_bridge = [&](int v,int parentEdge){
        disc[v]=low[v]=++timer;
        for(auto [to, eid] : g[v]){
            if(eid==parentEdge) continue;
            if(!disc[to]){
                dfs_bridge(to, eid);
                low[v]=min(low[v], low[to]);
                if(low[to] > disc[v]){
                    isBridge[eid]=true;
                }
            } else {
                low[v]=min(low[v], disc[to]);
            }
        }
    };
    for(int i=1;i<=n;++i) if(!disc[i]) dfs_bridge(i,-1);
    // ---------- union non‑bridge edges ----------
    DSU dsu(n);
    for(int i=0;i<m;++i){
        if(!isBridge[i]){
            dsu.unite(edges[i].first, edges[i].second);
        }
    }
    // Group vertices by component root
    unordered_map<int, vector<int>> compVertices;
    compVertices.reserve(n*2);
    for(int v=1; v<=n; ++v){
        int root = dsu.find(v);
        compVertices[root].push_back(v);
    }
    // ---------- check bipartiteness of each component (using only non‑bridge edges) ----------
    vector<char> good(n+1, false); // vertices that belong to an odd cycle
    vector<char> color(n+1, -1);
    for(auto &kv : compVertices){
        const vector<int> &verts = kv.second;
        // If component size < 2, cannot contain a cycle
        if(verts.size() < 2) continue;
        bool isBip=true;
        queue<int> q;
        for(int start: verts){
            if(color[start]!=-1) continue;
            color[start]=0; q.push(start);
            while(!q.empty()){
                int v=q.front(); q.pop();
                for(auto [to, eid] : g[v]){
                    if(isBridge[eid]) continue; // only non‑bridge edges
                    if(color[to]==-1){
                        color[to]=color[v]^1; q.push(to);
                    } else if(color[to]==color[v]){
                        isBip=false;
                    }
                }
            }
        }
        if(!isBip){
            for(int v: verts) good[v]=true;
        }
    }
    long long goodCount=0;
    for(int v=1; v<=n; ++v) if(good[v]) ++goodCount;
    long long answer = (long long)n - goodCount;
    cout << answer << "\n";
    return 0;
}
