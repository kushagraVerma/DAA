#include <bits/stdc++.h>
#include "timer.hpp"
#define int long long int
using namespace std;

/**
 * Finds a path from a given source to a sink
 * @param n number of vertices in the residual graph
 * @param source source vertex number
 * @param sink sink vertex number
 * @param residualGraph A residual graph in adjacency matrix formal
 * @return a vector containing the vertex numbers of a path from the given source to sink
*/
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

/**
 * Finds all reachable vertices from a particular source in a residual graph
 * @param n number of vertices in the residual graph
 * @param source source vertex number
 * @param residualGraph a residual graph in adjacency matrix format
 * @return a vector representing the reachability of every vertex from the given source.
*/
vector<int> findReachableVertices(int n, int source, vector<vector<int>> &residualGraph) {
    vector<int> visited(n);
    queue<int> q;
    q.push(source);
    visited[source] = true;

    while (!q.empty())
    {
        int k = q.front();
        q.pop();
        visited[k] = 1;
        for (int i = 0; i < residualGraph[k].size(); i++)
        {
            if (residualGraph[k][i] && !visited[i])
            {
                visited[i] = true;
                q.push(i);
            }
        }
    }
    return visited;
}
    
int32_t main() {
    int n, m; //> Number of vertices and edges in the original graph
    cin >> n >> m;
    vector<vector<int>> originalGraph(n, vector<int>(n)); // The original graph represented in adjacency matrix format
    vector<vector<int>> residualGraph(n, vector<int>(n)); // Initializing residual graph with flows initially set as 0 for all forward edges
    vector<vector<int>> flowGraph(n, vector<int>(n)); // FILL THIS
    
    for (int i = 0; i < m; i++)
    {
        int a,b,val;
        cin >> a >> b >> val;
        a--;
        b--;
        originalGraph[a][b] += val;
        residualGraph[a][b] += val;
    }
    
    int source, sink; // Source and sink vertices
    cin >> source >> sink;
    source--;
    sink--;
    Timer timer = Timer();
    int flow = 0;
    while (1)
    {
        vector<int> path = findPath(n, source, sink, residualGraph);
        if (path.size() < 2) break; // No path found
        int bottleneck = 1e18;
        for (int i = 0; i < path.size()-1; i++)
        {
            bottleneck = min(bottleneck, residualGraph[path[i]][path[i+1]]);
        }
        flow += bottleneck;
        for (int i = 0; i < path.size()-1; i++)
        {
            // Updating the forward and backward edges in the residual graph
            residualGraph[path[i]][path[i+1]] -= bottleneck;
            residualGraph[path[i+1]][path[i]] += bottleneck;

            // Adding the flow for corresponding edges in the flow graph
            flowGraph[path[i]][path[i+1]] += bottleneck;
        }
    }


    cout << "The max flow of the network: " << flow << endl;
    
    // Finding the min s-t cut for the graph
    vector<pair<int,int>> cutEdges; // Vector to store the min s-t cut edges of the graph
    vector<int> visited = findReachableVertices(n, source, residualGraph);
    for (int i = 0; i < visited.size(); i++)
    {
        for (int j = i; j < visited.size(); j++)
        {
            // Finding the vertices that are not reachable in the residual graph, but have a forward edge in the original graph
            // This means that the capacity has been exhausted by the flow
            if (visited[i] && !visited[j] && originalGraph[i][j]) cutEdges.push_back({i, j});
        }
    }
    timer.stopClock();
    cout << "No. of edges in the min s-t cut: " << cutEdges.size() << endl;
    cout << "Edges in the min s-t cut:" << endl;
    for (int i = 0; i < cutEdges.size(); i++)
    {
        cout << cutEdges[i].first + 1 << "-" << cutEdges[i].second + 1 << endl;
    }
    cout << "Total time for execution excluding printing: " << timer.getDuration() << " µs" << endl;
    return 0;
}