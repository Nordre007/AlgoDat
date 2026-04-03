#include <bits/stdc++.h>
using namespace std;

int bfs(int src, int target, const vector<vector<int>>& graph) {
    if (src == target) return 0;

    vector<int> dist(graph.size(), -1);
    queue<int> q;

    q.push(src);
    dist[src] = 0;

    while (!q.empty()) {
        int u = q.front(); 
        q.pop();

        for (int v : graph[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;

                if (v == target)
                    return dist[v];

                q.push(v);
            }
        }
    }

    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<string> words(n);
    vector<array<uint8_t, 26>> freq(n);

    for (int i = 0; i < n; ++i) {
        cin >> words[i];
        freq[i].fill(0);

        for (char c : words[i]) {
            freq[i][c - 'a']++;
        }
    }

    vector<vector<int>> graph(n);

    for (int i = 0; i < n; ++i) {
        array<uint8_t, 26> need = {};

        for (int j = 1; j < 5; ++j) {
            need[words[i][j] - 'a']++;
        }

        for (int j = 0; j < n; ++j) {
            if (i == j) continue;

            bool ok = false;

            // try removing one letter from B
            for (int k = 0; k < 5; ++k) {
                array<uint8_t, 26> temp = freq[j];
                temp[words[j][k] - 'a']--;

                if (temp == need) {
                    ok = true;
                    break;
                }
            }

            if (ok) {
                graph[i].push_back(j);
            }
        }
    }

    unordered_map<string, int> id;
    id.reserve(n);
    id.max_load_factor(0.7);

    for (int i = 0; i < n; ++i) {
        id[words[i]] = i;
    }

    while (q--) {
        string a, b;
        cin >> a >> b;

        int start = id[a];
        int end = id[b];

        int res = bfs(start, end, graph);

        if (res == -1)
            cout << "Impossible\n";
        else
            cout << res << "\n";
    }

    return 0;
}
