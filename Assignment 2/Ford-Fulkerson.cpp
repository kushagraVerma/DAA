#include <bits/stdc++.h>
#define int long long int
using namespace std;
    
vector<int> findPath(int n, int source, int sink, vector<vector<int>> &residualGraph) {
    vector<int> parent(n, -1);
    vector<int> visited(n);
    queue<int> q;
    q.push(source);
    visited[source] = 1;
    while (!q.empty())
    {
        int k = q.front();
        q.pop();
        if (k == sink) break;
        for (int i = 0; i < residualGraph[k].size(); i++)
        {
            if (!visited[i] && residualGraph[k][i] && parent[i] == -1)
            {
                q.push(i);
                parent[i] = k;
                visited[i] = 1;
            }
        }
    }
    
    vector<int> path;
    while (sink != -1)
    {
        path.push_back(sink);
        sink = parent[sink];
    }
    reverse(path.begin(), path.end());
    return path;
}
    
int32_t main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> originalGraph(n, vector<int>(n));
    vector<vector<int>> residualGraph(n, vector<int>(n));
    
    for (int i = 0; i < m; i++)
    {
        int a,b,val;
        cin >> a >> b >> val;
        a--;
        b--;
        originalGraph[a][b] += val;
        residualGraph[a][b] += val;
    }
    
    int source, sink;
    cin >> source >> sink;
    source--;
    sink--;
    
    int flow = 0;
    while (1)
    {
        vector<int> path = findPath(n, source, sink, residualGraph);
        if (path.size() < 2) break;
        int bottleneck = 1e18;
        for (int i = 0; i < path.size()-1; i++)
        {
            bottleneck = min(bottleneck, residualGraph[path[i]][path[i+1]]);
        }
        flow += bottleneck;
        for (int i = 0; i < path.size()-1; i++)
        {
            residualGraph[path[i]][path[i+1]] -= bottleneck;
            residualGraph[path[i+1]][path[i]] += bottleneck; 
        }
    }
    cout << flow << endl;
}