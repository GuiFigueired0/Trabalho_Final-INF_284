#include "gcp.h"

#include <map>
#include <set>

GCP::GCP(std::string filename, int nColors): 
m_nColors(nColors), m_nEdges(0) {
    std::map<int, std::set<int>> graph;
    std::ifstream in("data/" + filename);
    std::string line;

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        int u, v;
        char c;
        ss >> c;
        if (c == 'e') {
            ss >> u >> v;
            u--; v--;
            if (u<0 || v<0 || u==v) {
                //std::cout << "Entry Error: edge " << u << " - " << v << " is not possible!\n";
            } else {
                graph[u].emplace(v);
                graph[v].emplace(u);
            }
        }
    }

    in.close();

    m_graph.clear();
    for (int u=0; u<graph.size(); u++) {
        m_graph.emplace_back(graph[u].begin(), graph[u].end());
        m_nEdges += m_graph[u].size();
    }
    m_nEdges /= 2;
}
int GCP::m_evaluateSolution(const std::vector<int> &coloring) {
    int conflicts = 0;
    for (int u = 0; u < getNV(); ++u) {
        conflicts += m_localConflicts(u, coloring);
    }
    return conflicts / 2;
}

int GCP::m_localConflicts(int u, const std::vector<int> &coloring) {
    int sum = 0;
    for (int v : m_graph[u]) {
        if (coloring[u] == coloring[v]) {
            sum++;
        }
    }
    return sum;
}

int GCP::getNE() {
    return m_nEdges;
}

int GCP::getNV() {
    return m_graph.size();
}