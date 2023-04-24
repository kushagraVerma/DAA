#include <bits/stdc++.h>
#define int long long int
using namespace std;

vector<int> partitionGraph(int n, vector<vector<int>> &graph) {
    vector<int> partition(n);
    vector<int> visited(n);
    queue<int> q;
    q.push(0);
    partition[0] = 1;
    visited[0] = 1;
    while (!q.empty())
    {
        int k = q.front();
        visited[k] = 1;
        q.pop();
        for (int i = 0; i < graph[k].size(); i++)
        {
            if (!visited[i] && graph[k][i])
            {
                q.push(i);
                partition[i] = 3 - partition[k];
                visited[i] = 1;
            }
        }
    }
    return partition;
}

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
    int n,m;
    cin >> n >> m;
    vector<vector<int>> adjList(n, vector<int>(n));
    for (int i = 0; i < m; i++)
    {
        int a,b;
        cin >> a >> b;
        a--;
        b--;
        adjList[a][b] = 1;
        adjList[b][a] = 1;
    }
    vector<int> partition = partitionGraph(n, adjList);
    vector<int> partition1, partition2;
    for (int i = 0; i < partition.size(); i++)
    {
        if (partition[i] == 1) partition1.push_back(i + 1);
        else if (partition[i] == 2) partition2.push_back(i + 1);
    }

    vector<vector<int>> directedGraph(n+2, vector<int>(n+2));
    for (int i = 0; i < partition1.size(); i++)
    {
        directedGraph[0][partition1[i]] = 1;
    }
    for (int i = 0; i < partition2.size(); i++)
    {
        directedGraph[partition2[i]][n+1] = 1;
    }
    for (int i = 0; i < partition1.size(); i++)
    {
        for (int j = 0; j < partition2.size(); j++)
        {
            int vertex1 = partition1[i] - 1;
            int vertex2 = partition2[j] - 1;
            if (adjList[vertex1][vertex2]) directedGraph[vertex1 + 1][vertex2 + 1] = 1;
        }
    }
    vector<vector<int>> flowGraph(n + 2, vector<int>(n + 2));
    
    int source = 0, sink = n + 1;
    while (1)
    {
        vector<int> path = findPath(n + 2, source, sink, directedGraph);
        if (path.size() < 2) break;
        for (int i = 0; i < path.size() - 1; i++)
        {
            directedGraph[path[i]][path[i + 1]] -= 1;
            directedGraph[path[i + 1]][path[i]] += 1;
            flowGraph[path[i]][path[i + 1]] += 1;
        }
    }
    for (int i = 0; i < flowGraph.size(); i++)
    {
        flowGraph[0][i] = 0;
        flowGraph[i][n + 1] = 0;
    }
    
    for (int i = 0; i < partition2.size(); i++)
    {
        for (int j = 0; j < partition1.size(); j++)
        {
            if (directedGraph[partition2[i]][partition1[j]])
            {
                cout << partition2[i] << "-" << partition1[j] << endl;
            }
        }
        
    }
    
}