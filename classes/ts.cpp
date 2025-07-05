#include "ts.h"

#include <queue>
#include <algorithm>

TS::TS(std::string filename, int nColors, int nTabuIterations): 
GCP(filename, nColors), m_nTabuIterations(nTabuIterations) {
    m_color = std::vector<int>(m_graph.size(), -1);
    m_bestSolution = std::make_pair(m_nEdges + 1,-1);
    m_tabuList = std::vector<std::vector<int>>(m_graph.size(), std::vector<int>(nColors, -nTabuIterations));
}

int TS::solve() {
    m_greedyInitialization();
    m_tabuSearch();
    return m_bestSolution.first;
}

void TS::m_greedyInitialization() {
    std::queue<int> q; 
    q.emplace(0);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (m_color[u] != -1) 
            continue;
        std::pair<int, int> bestSwap = std::make_pair(m_graph[u].size() * (-1), -1); 
        for (int color=0; color<m_nColors; color++) {
            m_color[u] = color;
            int currValue = m_graph[u].size() - m_localConflicts(u, m_color);
            if (bestSwap.first < currValue) {
                bestSwap = std::make_pair(currValue, color); 
            }
        }
        m_color[u] = bestSwap.second;
        for (int v: m_graph[u])
            q.emplace(v);
    }
}

void TS::m_tabuSearch() {
    int iteration = 0;
    m_bestSolution = std::make_pair(m_evaluateSolution(m_color), 0);
    while (true) {
        if (++iteration - m_bestSolution.second > 2*getNV()) 
            break;
        
        int currValue = m_evaluateSolution(m_color);
        std::vector<std::pair<int, std::pair<int,int>>> values; // <value, <vertice, color>>

        for (int u=0; u<getNV(); u++) {
            int currConflicts = m_localConflicts(u, m_color), currColor = m_color[u];

            for (int color=0; color<m_nColors; color++) {
                m_color[u] = color;
                int newValue = currValue - currConflicts + m_localConflicts(u, m_color);
                values.emplace_back(newValue, std::make_pair(u, color));
            }
            
            m_color[u] = currColor;
        }

        std::sort(values.begin(), values.end());
        for (auto &option: values) {
            int value = option.first, u = option.second.first, color = option.second.second;
            if (value < m_bestSolution.first) {
                m_bestSolution = std::make_pair(value, iteration);
                m_tabuList[u][color] = iteration;
                m_color[u] = color;
                break;
            }

            if (!m_isTabu(iteration, u, color)) {
                m_tabuList[u][color] = iteration;
                m_color[u] = color;
                break;
            }
        }

        if (m_bestSolution.first == 0) 
            break;
    }
}

int TS::m_isTabu(int it, int u, int color) {
    return it - m_tabuList[u][color] <= m_nTabuIterations;
}