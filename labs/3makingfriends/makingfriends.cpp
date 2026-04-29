#include <bits/stdc++.h>

using namespace std;




int Prim(set<int>& vertices, map<pair<int, int>, int>& edges) {

    set<int> visited;
    visited.insert(1);

    int total = 0;
    while(visited.size() < vertices.size()) {

        int smallest = 1000001;
        int tempvertex = -1;
        for(int vertex : visited) {
            for(const auto& [key, value] : edges) {
                if ((vertex == key.first || vertex == key.second)) {
                    if (edges[key] < smallest) {
                        if (key.first == vertex) {
                            if (visited.count(key.second)) {
                                continue;
                            }
                            tempvertex = key.second;
                        } else {
                            if (visited.count(key.first)) {
                                continue;
                            }
                            tempvertex = key.first;
                        }
                        smallest = edges[key]; 
                    }
                }
            }
        }
        
        total += smallest;
        visited.insert(tempvertex);


    }
    return total;        
} 

int main() {

    map<pair<int, int>, int> weightMap;

    vector<int> visited;

    int n, m;

    cin >> n >> m;
    visited.resize(n);

    int u, v, w;

    for(int i = 0; i < m; i++) {
        cin >> u >> v >> w;
        weightMap[{u, v}] = w;
    }

    set<int> vertices;
    for(int i = 1; i <= n; i++) {
        vertices.insert(i);
    }

    cout << Prim(vertices, weightMap) << "\n";

    return 0;
}
