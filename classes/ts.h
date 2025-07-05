#pragma once

#include "gcp.h"

class TS : public GCP {
private:
    int m_nTabuIterations, m_maxItWithoutImprovement;
    std::vector<int> m_color;
    std::pair<int,int> m_bestSolution; // <value, it>
    std::vector<std::vector<int>> m_tabuList;

private:
    void m_greedyInitialization();
    void m_tabuSearch();
    int m_isTabu(int it, int u, int color);

public: 
    TS(std::string filename, int nColors, int nTabuIterations = 7);

    int solve() override;
};