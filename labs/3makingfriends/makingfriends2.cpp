    #include <bits/stdc++.h>

    using namespace std;

    const int INF = 1e6 + 1;
    const size_t MAXN = 1e5;

    int primMST(vector<vector<pair<int, int>>>& graph, int n) {
        //för att hålla reda på distansen till alla noder som man kan nå, distansen från alla visited
        vector<int> dist(n, INF);
        bitset<MAXN> visited;
        //vector<bool> visited(n + 1, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        int cost = 0;
        pq.push({0, 1});
        dist[1] = 0;
        int visited_count = 0;

        while (!pq.empty() && visited_count < n) {
            auto [w, u] = pq.top();
            pq.pop();

            if (visited[u]) continue;
            visited[u] = true;
            cost += w;
            visited_count++;

            for (auto& neighbor : graph[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;
                if (!visited[v] && weight < dist[v]) {
                    dist[v] = weight;
                    pq.push({dist[v], v});
                }
            }
        }
        return cost;
    }

    int main() {

        ios::sync_with_stdio(false);
        cin.tie(nullptr);

        int n, m;
        cin >> n >> m;

        //adjacency matrix, sparar noder och vikter
        vector<vector<pair<int, int>>> graph(n+1);
        for (int i = 0; i < m; ++i) {
            int u, v, w;
            cin >> u >> v >> w;
            //symmetrisk matris, undirected graph
            graph[u].push_back({v, w});
            graph[v].push_back({u, w});
        }
        int minimum_cost = primMST(graph, n + 1);
        cout  << minimum_cost << "\n";
        return 0;
    }