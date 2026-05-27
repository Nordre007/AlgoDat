#include <bits/stdc++.h>

using namespace std;

struct Edge {
    int node;
    int capacity; 
    int flow;
    int remaninCapacity;
    int reverseIndex;
    bool Active;
} typedef Edge;

struct Tuple {
    int u;
    int v;
    int c;
} typedef Tuple;

int N, M, C, P;


bool bfs(int source, int sink, vector<vector<Edge>>& graph, 
         vector<int>& parent, vector<int>& parentEdge) {
    fill(parent.begin(), parent.end(), -1);
    parent[source] = source;
    queue<int> toVisit;
    toVisit.push(source);

    while(!toVisit.empty()) {
        int current = toVisit.front(); toVisit.pop();
        for(int i = 0; i < graph[current].size(); i++) {
            Edge& edge = graph[current][i];
            if(edge.Active && parent[edge.node] == -1 && edge.remaninCapacity > 0) {
                parent[edge.node] = current;
                parentEdge[edge.node] = i; 
                toVisit.push(edge.node);
                if(edge.node == sink) return true;
            }
        }
    }

    return false;
}

int pushFlow(int source, int sink, vector<vector<Edge>>& graph,
             vector<int>& parent, vector<int>& parentEdge) {
    int bottleneck = INT_MAX;
    for(int cur = sink; cur != source; cur = parent[cur]) {
        Edge& fwd = graph[parent[cur]][parentEdge[cur]]; 
        bottleneck = min(bottleneck, fwd.remaninCapacity);
    }
    for(int cur = sink; cur != source; cur = parent[cur]) {
        Edge& fwd = graph[parent[cur]][parentEdge[cur]];
        Edge& bwd = graph[cur][fwd.reverseIndex]; 
        fwd.remaninCapacity -= bottleneck;
        bwd.remaninCapacity += bottleneck;
    }
    return bottleneck;
}

int maxFlow(vector<vector<Edge>>& graph, int source, int sink) {
    int totalFlow = 0;
    vector<int> parent(N), parentEdge(N); 
    while(bfs(source, sink, graph, parent, parentEdge))
        totalFlow += pushFlow(source, sink, graph, parent, parentEdge);
    return totalFlow;
}

void resetGraph(vector<vector<Edge>>& graph) {
    for(int i = 0; i < N; i++) {
        for(auto& edge : graph[i]) {
            if(edge.Active) {
                edge.remaninCapacity = edge.capacity;
                edge.flow = 0;
            }
        }
    }
}

int main() {


    cin >> N >> M >> C >> P;

    vector<vector<Edge>> graph(N);
    vector<Tuple> edges(M);
    vector<pair<int,int>> edgeLocation(M);


    int u, v, c;
    for(int i = 0; i < M; i++) {

        cin >> u >> v >> c;

        edges[i] = {u, v, c};
        edgeLocation[i] = {u, graph[u].size()};

        graph[u].push_back({v, c, 0, c, (int) graph[v].size() , true});
        graph[v].push_back({u, c, 0, c, (int) graph[u].size() - 1, true});
    }

    vector<int> IndexOfedgesToRemove(P);
    for(int i = 0; i < P; i++) cin >> IndexOfedgesToRemove[i];

    int source = 0, sink = N - 1;

    int numberOfEdgesToRemove {0};
    int maximumFlow {0};

    for(int i = 0; i < P; i++) {
        int id = IndexOfedgesToRemove[i]; 

        Edge& fwd = graph[edgeLocation[id].first][edgeLocation[id].second];
        Edge& bwd = graph[fwd.node][fwd.reverseIndex];

        fwd.Active = false;
        bwd.Active = false;

        resetGraph(graph);

        int newFlow = maxFlow(graph, source, sink);

        if (newFlow < C) {
            fwd.Active = true;  
            bwd.Active = true;
            break;
        }

        numberOfEdgesToRemove++;
        maximumFlow = newFlow;
    }

    cout << numberOfEdgesToRemove << " " << maximumFlow << "\n";

}