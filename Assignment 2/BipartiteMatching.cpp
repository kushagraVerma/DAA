#include <bits/stdc++.h>
#include "timer.hpp"
#define int long long int
using namespace std;

/**
 * Partitions a bipartite graph into two vertex sets 
 * @param n number of vertices
 * @param graph a graph in adjacency matrix format
 * @return a vector that gives the partition set number of every vertex 
*/
vector<int> partitionGraph(int n, vector<vector<int>> &graph) {
    vector<int> partition(n);
    vector<int> visited(n);
    queue<int> q;
    for (int j = 0; j < n; j++)
    {
        if (!visited[j])
        {
            q.push(j);
            partition[j] = 1;
            visited[j] = 1;
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
        }
    }
    return partition;
}

/**
 * Finds a path between a source and a sink in a residual graph
 * @param n number of vertices
 * @param source source vertex number
 * @param sink sink vertex number
 * @param residualGraph a residual graph in adjacency matrix format
 * @return a vector containing vertices along the source-sink path
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

int32_t main() {
    int n,m; // Number of vertices and edges
    cin >> n >> m;
    vector<vector<int>> adjList(n, vector<int>(n)); // Graph stored in adjacency list format
    for (int i = 0; i < m; i++)
    {
        int a,b;
        cin >> a >> b;
        a--;
        b--;
        adjList[a][b] = 1;
        adjList[b][a] = 1;
    }
    Timer timer = Timer();
    vector<int> partition = partitionGraph(n, adjList);
    vector<int> partition1, partition2; // Splitting the vertices into two sets
    for (int i = 0; i < partition.size(); i++)
    {
        if (partition[i] == 1) partition1.push_back(i + 1);
        else if (partition[i] == 2) partition2.push_back(i + 1);
    }

    vector<vector<int>> residualGraph(n+2, vector<int>(n+2)); // Creating a residual directed graph with a temporary source and sink to run Ford-Fulkerson algorithm
    for (int i = 0; i < partition1.size(); i++)
    {
        residualGraph[0][partition1[i]] = 1;
    }
    for (int i = 0; i < partition2.size(); i++)
    {
        residualGraph[partition2[i]][n+1] = 1;
    }
    for (int i = 0; i < partition1.size(); i++)
    {
        for (int j = 0; j < partition2.size(); j++)
        {
            int vertex1 = partition1[i] - 1;
            int vertex2 = partition2[j] - 1;
            if (adjList[vertex1][vertex2]) residualGraph[vertex1 + 1][vertex2 + 1] = 1;
        }
    }
    vector<vector<int>> flowGraph(n + 2, vector<int>(n + 2)); // Graph to denote flow through the edges
    
    int source = 0, sink = n + 1;
    while (1)
    {
        vector<int> path = findPath(n + 2, source, sink, residualGraph);
        if (path.size() < 2) break;
        for (int i = 0; i < path.size() - 1; i++)
        {
            // Updating forward and backward edges in residual graph
            residualGraph[path[i]][path[i + 1]] -= 1;
            residualGraph[path[i + 1]][path[i]] += 1;

            // Updating the flow through the edges in the flow graph
            flowGraph[path[i]][path[i + 1]] += 1;
        }
    }
    for (int i = 0; i < flowGraph.size(); i++)
    {
        // Resetting the flows through the temporary source and sink back to 0, since we do not need them after Ford-Fulkerson
        flowGraph[0][i] = 0;
        flowGraph[i][n + 1] = 0;
    }

    int edgeCount = 0;
    for (int i = 0; i < partition2.size(); i++)
    {
        for (int j = 0; j < partition1.size(); j++)
        {
            if (residualGraph[partition2[i]][partition1[j]]) edgeCount++;
        }
    }
    timer.stopClock();
    cout << "No. of edges in the maximum bipartite matching are: " << edgeCount << endl;
    cout << "The edges part of the maximum bipartite matching are:" << endl;
    for (int i = 0; i < partition2.size(); i++)
    {
        for (int j = 0; j < partition1.size(); j++)
        {
            if (residualGraph[partition2[i]][partition1[j]])
            {
                cout << partition1[j] << "-" << partition2[i] << endl;
            }
        }
    }
    cout << "Total time for execution excluding printing: " << timer.getDuration() << " µs" << endl;
}