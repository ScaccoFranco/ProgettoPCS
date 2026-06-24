#pragma once

#include <iostream>
#include "circuit_eigen.hpp"

// Si raccolgono le funzioni di output per un main pulito. 
// Oltre a stampare i cicli trovati e le correnti di maglia, 
// si esegue qui l'ultimo vero calcolo in "stampa_risultati": riapplica la matrice B 
// alle correnti di maglia per trovare le correnti effettive su ogni ramo, 
// e usando la legge di Ohm (V=R*I) per ricavare e stampare le tensioni finali sui resistori.

template<typename T>
void stampa_cicli(std::vector<std::vector<T>> &cycles)
{
    std::cout << "\nCicli trovati: " << cycles.size() << "\n";
    for (int i = 0; i < (int)cycles.size(); i++) {
        std::cout << "Ciclo " << i+1 << ": ";
        for (int node : cycles[i]) std::cout << node << " ";
        std::cout << "\n";
    }
}

void stampa_correnti(Eigen::VectorXd &correnti)
{
    std::cout << "\nCorrenti di maglia:\n";
    for (int i = 0; i < (int)correnti.size(); i++)
        std::cout << "I" << i+1 << " = " << correnti(i) << "\n";
}

template<typename T>
void stampa_risultati(const Eigen::VectorXd &correnti, const circuit_graph<int> &cg, const Eigen::MatrixXd &B, std::vector<std::vector<T>> &cycles)
{
    const auto& resistori = cg.get_allresistor();
    std::cout << "\nTensioni sui resistori:\n";
    for (int k = 0; k < (int)resistori.size(); k++) {
        double corrente_k = 0.0;
        for (int j = 0; j < (int)cycles.size(); j++)
            corrente_k += B(k, j) * correnti(j);
        double tensione_k = resistori[k].c_value * corrente_k;
        std::cout << "R" << resistori[k].c_number
                  << ": V = " << tensione_k << " volts"
                  << ", I = " << corrente_k << " amps.\n";
    }
    std::cout << "\n";
}
