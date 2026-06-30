#pragma once

#include <iostream>
#include "circuit.hpp"

// Si raccolgono le funzioni di output per un main pulito. 
// Oltre a stampare i cicli trovati e le correnti di maglia, 
// si esegue qui l'ultimo vero calcolo in "stampa_risultati": riapplica la matrice B 
// alle correnti di maglia per trovare le correnti effettive su ogni ramo, 
// e usando la legge di Ohm (V=R*I) per ricavare e stampare le tensioni finali sui resistori.


// stampa le maglie individuate, il numero totale di cicli trovati e, per ciascuno, la sequenza di nodi che lo compone
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

// stampa le correnti di maglia, cioè la soluzione del sistema lineare BᵀRB i = v
void stampa_correnti(Eigen::VectorXd &correnti)
{
    std::cout << "\nCorrenti di maglia:\n";
    for (int i = 0; i < (int)correnti.size(); i++)
        std::cout << "I" << i+1 << " = " << correnti(i) << "\n";
}


// produce il risultato vero richiesto dal progetto. 
// Per ogni resistore ricalcola la corrente effettiva di ramo come I_k = Σ_j B(k,j)·I_j
// e poi la tensione con la legge di Ohm V = R·I, stampando entrambe
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

void stampa_tempo(bool usa_find_cycles, bool usa_bfs, double time) {
    std::cout << "tempo - usando " << (usa_find_cycles ? "find_cycles" : "De Pina");
    
    if (!usa_find_cycles) 
        std::cout << " con " << (usa_bfs ? "BFS" : "Dijkstra");

    std::cout << ": " << time << " ms\n";

}