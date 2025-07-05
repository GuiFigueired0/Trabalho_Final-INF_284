#pragma once

#include "gcp.h"

#include <random>
#include <chrono>

class ACO : public GCP {
private:
    int m_nAnts;
    int m_nIterations;
    double m_alpha;
    double m_beta;
    double m_evaporationRate;
    double m_initialPheromone;
    std::pair<int, std::vector<int>> m_bestSolution;
    std::vector<std::vector<double>> m_pheromoneMatriz;
    std::mt19937 m_randGen;

private:
    std::vector<int> m_RLFConstruction();
    void m_localSearch(std::vector<int> &colouring);
    void m_updatePheromone(std::vector<std::pair<int, std::vector<int>>> &antSolutions);

public:
    ACO(std::string filename, 
        int nColors, 
        int nAnts = 20, 
        int nIterations = 100, 
        double alpha = 1.0, 
        double beta = 2.0, 
        double evaporationRate = 0.1);

    int solve() override;
};