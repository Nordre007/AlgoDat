#include <bits/stdc++.h>

using namespace std;

struct Edge {
    int capacity; 
    int flow;
    int remaninCapacity;
    bool Active = false;
} typedef Edge;

struct Tuple {
    int u;
    int v;
    int c;
} typedef Tuple;

int N, M, C, P;


bool bfs(int source, int sink, vector<vector<Edge>>& graph, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[source] = source;
    queue<int> toVisit;
    toVisit.push(source);

    while(!toVisit.empty()) {
        int current = toVisit.front(); toVisit.pop();

        for(int i = 0; i < (int) graph[current].size(); i++) {
            Edge& edge = graph[current][i];
            if(edge.Active) {
                if(parent[i] == -1 && edge.remaninCapacity > 0) {
                    parent[i] = current;
                    toVisit.push(i);
                    if (i == sink) return true;
                }
            }
        }
    }
    
    return false;
}

int pushFlow(int source, int sink, vector<vector<Edge>>& graph, vector<int>& parent) {
    
    int bottleneck = INT_MAX;
    for (int cur = sink; cur != source; cur = parent[cur])
        bottleneck = min(bottleneck, graph[parent[cur]][cur].remaninCapacity); 

    
    for (int cur = sink; cur != source; cur = parent[cur]) {
        graph[parent[cur]][cur].remaninCapacity -= bottleneck; // forward
        graph[cur][parent[cur]].remaninCapacity += bottleneck; // backward
    }
    return bottleneck; 
}
 
int maxFlow(vector<vector<Edge>>& graph, int source, int sink) {
    int totalFlow = 0;
    vector<int> parent(N);
    
    while(bfs(source, sink, graph, parent)) {
        totalFlow += pushFlow(source, sink, graph, parent);
    }

    return totalFlow;

}

void resetGraph(vector<vector<Edge>>& graph, vector<Tuple>& edges) {
    for(int i = 0; i < M; i++) {
        auto& edge = edges[i];

        if(graph[edge.u][edge.v].Active) {
            graph[edge.u][edge.v].remaninCapacity = graph[edge.u][edge.v].capacity;
            graph[edge.u][edge.v].flow = 0;

            graph[edge.v][edge.u].remaninCapacity = graph[edge.u][edge.v].capacity;
            graph[edge.v][edge.u].flow = 0;
        }

    }
}

int main() {


    cin >> N >> M >> C >> P;

    vector<vector<Edge>> graph(N, vector<Edge>(N));
    vector<Tuple> edges(M);

    int u, v, c;
    for(int i = 0; i < M; i++) {

        cin >> u >> v >> c;

        edges[i] = {u, v, c};
        graph[u][v] = {c, 0, c, true};
        graph[v][u] = {c, 0, c, true};
    }

    vector<int> IndexOfedgesToRemove(P);

    for(int i = 0; i < P; i++) cin >> IndexOfedgesToRemove[i];

    int source = 0, sink = N - 1;

    int numberOfEdgesToRemove {0};
    int maximumFlow {0};

    for(int i = 0;  i < P; i++) {
        auto& edgeToRemove = edges[IndexOfedgesToRemove[i]];

        graph[edgeToRemove.u][edgeToRemove.v].Active = false;
        graph[edgeToRemove.v][edgeToRemove.u].Active = false;
        
        resetGraph(graph, edges);

        int newFlow = maxFlow(graph, source, sink);

        if (newFlow < C) {
            graph[edgeToRemove.u][edgeToRemove.v].Active = true;
            graph[edgeToRemove.v][edgeToRemove.u].Active = true;
            break;
            
        }

        numberOfEdgesToRemove++;
        maximumFlow = newFlow;
    }

    cout << numberOfEdgesToRemove << " " << maximumFlow << "\n";

}