#pragma once

#include <iostream>
#include "circuit.hpp"


void stampa_risultati(const std::vector<double> &correnti, const circuit_graph &cg, const std::vector<std::vector<double>> &B)
{
    auto resistori = cg.get_allresistors();
    int n = resistori.size();
    std::vector<double> I = (n, 0.0);
    for (int i = 0; i < n; i++) 
    {
        double V_k = resistors[k].c_value * I_k;  // legge di Ohm: V = R * I
        std::cout << "R" << resistors[k].c_number
                << ": V = " << V_k << " volts"
                << ", I = " << I_k << " amps.\n";
    }
}