#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class GCP {
protected:
    int m_nColors, m_nEdges;
    std::vector<std::vector<int>> m_graph;

protected:
    int m_localConflicts(int u, const std::vector<int> &coloring);
    int m_evaluateSolution(const std::vector<int> &coloring);

public: 
    GCP(std::string filename, int nColors);

    int getNE();
    int getNV();

    virtual int solve() = 0;
};