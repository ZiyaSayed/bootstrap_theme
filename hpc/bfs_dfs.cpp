#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// Structure for representing an undirected graph
struct Graph
{
    int numVertices;
    vector<vector<int>> adjList;
};

// Function to add an edge between two vertices in the graph
void addEdge(Graph &graph, int src, int dest)
{
    graph.adjList[src].push_back(dest);
    graph.adjList[dest].push_back(src);
}

// Function to create a new graph with a given number of vertices
Graph createGraph(int numVertices)
{
    Graph graph;
    graph.numVertices = numVertices;
    graph.adjList.resize(numVertices);
    return graph;
}

void parallelBFS(Graph &graph, int startVertex)
{
    vector<bool> visited(graph.numVertices, false);
    queue<int> bfsQueue;

    visited[startVertex] = true;
    bfsQueue.push(startVertex);

    while (!bfsQueue.empty())
    {
#pragma omp parallel
        {
#pragma omp for
            for (int i = 0; i < bfsQueue.size(); ++i)
            {
                int currentVertex = bfsQueue.front();
                bfsQueue.pop();

                // Process the current vertex
                cout << "Visited vertex: " << currentVertex << endl;

                // Explore neighboring vertices
                for (int neighbor : graph.adjList[currentVertex])
                {
                    if (!visited[neighbor])
                    {
#pragma omp critical
                        {
                            visited[neighbor] = true;
                            bfsQueue.push(neighbor);
                        }
                    }
                }
            }
        }
    }
}

void parallelDFS(Graph &graph, int startVertex)
{
    vector<bool> visited(graph.numVertices, false);
    stack<int> dfsStack;

    dfsStack.push(startVertex);

    while (!dfsStack.empty())
    {
        int currentVertex = dfsStack.top();
        dfsStack.pop();

        if (!visited[currentVertex])
        {
// Process the current vertex
#pragma omp critical
            {
                visited[currentVertex] = true;
                cout << "Visited vertex: " << currentVertex << endl;
            }

// Explore neighboring vertices
#pragma omp parallel
            {
#pragma omp for
                for (int neighbor : graph.adjList[currentVertex])
                {
                    if (!visited[neighbor])
                    {
#pragma omp critical
                        {
                            dfsStack.push(neighbor);
                        }
                    }
                }
            }
        }
    }
}
int main()
{
    int numVertices, numEdges;
    cout << "Enter the number of vertices: ";
    cin >> numVertices;
    cout << "Enter the number of edges: ";
    cin >> numEdges;

    Graph graph = createGraph(numVertices);

    cout << "Enter the edges in the format (source destination):" << endl;
    for (int i = 0; i < numEdges; ++i)
    {
        int src, dest;
        cin >> src >> dest;
        addEdge(graph, src, dest);
    }

    int startVertex;
    cout << "Enter the starting vertex for BFS and DFS: ";
    cin >> startVertex;

    cout << "Parallel BFS:" << endl;
    parallelBFS(graph, startVertex);

    cout << "\nParallel DFS:" << endl;
    parallelDFS(graph, startVertex);

    return 0;
}

// Graph graph = createGraph(7);
//     addEdge(graph, 0, 1);
//     addEdge(graph, 0, 2);
//     addEdge(graph, 1, 3);
//     addEdge(graph, 1, 4);
//     addEdge(graph, 2, 5);
//     addEdge(graph, 2, 6);
