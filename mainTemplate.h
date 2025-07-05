#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>

std::map<std::string, std::string> readExistingResults(const std::string &filename) {
    std::map<std::string, std::string> lines;
    std::ifstream in(filename);
    std::string line;

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string key;
        std::getline(ss, key, ',');

        lines[key] = line;
    }

    in.close();
    return lines;
}

template<class T>
void mainRun(std::string outputfile, int nIterations = 5) {
    std::map<std::string, std::string> results = readExistingResults(outputfile);

    auto runAndCheck = [&](const std::string &filename, int nColors) {
        std::cout << "Instance " << filename << std::endl;

        double values1 = 0, values2 = 0, values3 = 0, values4 = 0, nEdges;

        for (int i = 0; i < nIterations; i++) {
            T gcp1(filename, nColors);
            T gcp2(filename, nColors-1);
            T gcp3(filename, nColors-2);
            T gcp4(filename, nColors-3);
            std::cout << "k\n";
            values1 += gcp1.solve();
            std::cout << "k-1\n";
            values2 += gcp2.solve();
            std::cout << "k-2\n";
            values3 += gcp3.solve();
            std::cout << "k-3\n";
            values4 += gcp4.solve();

            nEdges = gcp1.getNE();
        }

        values1 /= nIterations;
        values2 /= nIterations;
        values3 /= nIterations;
        values4 /= nIterations;

        std::stringstream line;
        line << filename << "," << std::fixed << std::setprecision(4) << 100 * values1 / nEdges << "," << 100 * values2 / nEdges << "," << 100 * values3 / nEdges << "," << 100 * values4 / nEdges;

        results[filename] = line.str();
        std::cout << results[filename] << std::endl;
    };

    runAndCheck("anna.col", 11);
    runAndCheck("david.col", 11);
    runAndCheck("fpsol2.i.1.col", 65);
    runAndCheck("fpsol2.i.2.col", 30);
    runAndCheck("fpsol2.i.3.col", 30);
    runAndCheck("games120.col", 9);
    runAndCheck("homer.col", 13);
    runAndCheck("huck.col", 11);
    runAndCheck("inithx.i.1.col", 54);
    runAndCheck("inithx.i.2.col", 31);
    runAndCheck("inithx.i.3.col", 31);
    runAndCheck("jean.col", 10);
    runAndCheck("le450_15a.col", 15);
    runAndCheck("le450_25a.col", 25);
    runAndCheck("le450_25b.col", 25);
    runAndCheck("le450_5c.col", 5);
    runAndCheck("le450_5d.col", 5);
    runAndCheck("miles1000.col", 42);
    runAndCheck("miles1500.col", 73);
    runAndCheck("miles250.col", 8);
    runAndCheck("miles500.col", 20);
    runAndCheck("miles750.col", 31);
    runAndCheck("mulsol.i.1.col", 49);
    runAndCheck("mulsol.i.2.col", 31);
    runAndCheck("mulsol.i.3.col", 31);
    runAndCheck("mulsol.i.4.col", 31);
    runAndCheck("mulsol.i.5.col", 31);
    runAndCheck("myciel3.col", 4);
    runAndCheck("myciel4.col", 5);
    runAndCheck("myciel5.col", 6);
    runAndCheck("myciel6.col", 7);
    runAndCheck("queen5_5.col", 5);
    runAndCheck("queen6_6.col", 7);
    runAndCheck("queen7_7.col", 7);
    runAndCheck("queen8_12.col", 12);
    runAndCheck("queen8_8.col", 9);
    runAndCheck("queen9_9.col", 10);
    runAndCheck("school1.col", 14);
    runAndCheck("school1_nsh.col", 14);
    runAndCheck("zeroin.i.1.col", 49);
    runAndCheck("zeroin.i.2.col", 30);
    runAndCheck("zeroin.i.3.col", 30);
    
    std::ofstream out(outputfile);
    for (auto value: results) {
        out << value.second << '\n';
    }
    out.close();
}
