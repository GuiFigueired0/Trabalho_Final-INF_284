#include "aco.h"

#include <map>
#include <set>
#include <algorithm>
#include <cmath>

ACO::ACO(
    std::string filename,
    int nColors, 
    int nAnts, 
    int nIterations, 
    double alpha, 
    double beta, 
    double evaporationRate):
GCP(filename, nColors),
m_nAnts(nAnts), 
m_nIterations(nIterations), 
m_alpha(alpha), 
m_beta(beta), 
m_evaporationRate(evaporationRate) {
    m_pheromoneMatriz = std::vector<std::vector<double>>(getNV(), std::vector<double>(m_nColors, 1));
    m_randGen.seed(std::chrono::system_clock::now().time_since_epoch().count());
    m_bestSolution.first = m_nEdges + 1;
}

int ACO::solve() {
    for (int it = 0; it < m_nIterations; it++) {
        std::vector<std::pair<int, std::vector<int>>> antSolutions(m_nAnts);
        
        for (int k = 0; k < m_nAnts; k++) {
            antSolutions[k].second = m_RLFConstruction();
            antSolutions[k].first = m_evaluateSolution(antSolutions[k].second);
        }

        m_updatePheromone(antSolutions);

        if (m_bestSolution.first == 0) 
            break;
    }

    return m_bestSolution.first;
}

std::vector<int> ACO::m_RLFConstruction() {
    std::vector<int> currentColoring(getNV(), -1);
    std::set<int> uncoloredVertices;
    for (int i = 0; i < getNV(); i++) {
        uncoloredVertices.insert(i);
    }

    while (!uncoloredVertices.empty()) {
        int vertexToColor = -1, vexterDegree = -1;

        for (int u : uncoloredVertices) {
            int currentDegree = 0;
            for (int neighbor : m_graph[u])
                if (uncoloredVertices.count(neighbor))
                    currentDegree++;

            if (currentDegree > vexterDegree || (currentDegree == vexterDegree && (m_randGen() % 2 == 0))) {
                vexterDegree = currentDegree;
                vertexToColor = u;
            }
        }

        std::vector<double> probabilities(m_nColors, 0.0);
        double sum = 0.0;

        for (int c = 0; c < m_nColors; c++) {
            currentColoring[vertexToColor] = c;
            int currConflicts = m_localConflicts(vertexToColor, currentColoring);
            
            double heuristic = 1.0 / (1.0 + currConflicts);

            double pheromone = m_pheromoneMatriz[vertexToColor][c];
            double probability = std::pow(pheromone, m_alpha) * std::pow(heuristic, m_beta);
            probabilities[c] = probability;
            sum += probability;
        }

        int chosenColor = -1;
        if (sum != 0.0) {
            std::uniform_real_distribution<> dist(0.0, 1.0);
            double r = dist(m_randGen);
            double cumulativeProbability = 0.0;

            for (int c = 0; c < m_nColors; ++c) {
                cumulativeProbability += (probabilities[c] / sum);
                if (r <= cumulativeProbability) {
                    chosenColor = c;
                    break;
                }
            }
        }
        if (chosenColor == -1) 
            chosenColor = m_randGen() % m_nColors;

        currentColoring[vertexToColor] = chosenColor;
        uncoloredVertices.erase(vertexToColor);
    }
    return currentColoring;
}

void ACO::m_localSearch(std::vector<int> &colouring) {
    bool improved;
    do {
        improved = false;
        int currValue = m_evaluateSolution(colouring);

        for (int u=0; u<getNV(); u++) {
            int currConflicts = m_localConflicts(u, colouring), currColor = colouring[u];

            for (int color = 0; color < m_nColors; color++) {
                colouring[u] = color;
                int newValue = currValue - currConflicts + m_localConflicts(u, colouring);

                if (newValue < currValue) {
                    currConflicts = newValue - currValue + currConflicts;
                    currValue = newValue;
                    currColor = color;
                    improved = true;
                } else colouring[u] = currColor;
            }
        }
    } while (improved);
}

void ACO::m_updatePheromone(std::vector<std::pair<int, std::vector<int>>> &antSolutions) {
    for (int i = 0; i < getNV(); i++) {
        for (int c = 0; c < m_nColors; c++) {
            m_pheromoneMatriz[i][c] *= (1.0 - m_evaporationRate);
        }
    }

    std::sort(antSolutions.begin(), antSolutions.end());
    m_localSearch(antSolutions[0].second); 
    antSolutions[0].first = m_evaluateSolution(antSolutions[0].second); 

    if (antSolutions[0].first < m_bestSolution.first)
        m_bestSolution = antSolutions[0];

    for (int i = 0; i < m_nAnts/2; i++) {
        std::pair<int, std::vector<int>>& ant = antSolutions[i];

        double pheromone = 1.0 / (1.0 + ant.first); 
        for (int u = 0; u < getNV(); u++)
            m_pheromoneMatriz[u][ant.second[u]] += pheromone;
    }

    double elitistPheromone = 1.0 / (1.0 + m_bestSolution.first); 
    for (int u = 0; u < getNV(); u++)
        m_pheromoneMatriz[u][m_bestSolution.second[u]] += elitistPheromone;
}
