#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "unidirected_graph.hpp"
#include "elements.hpp"

// ============================================================
//  Costruzione delle matrici del sistema: B, R, v.
//  Le funzioni non-template sono 'inline' per evitare definizioni
//  multiple se gli header vengono inclusi in piu' unita' di traduzione.
// ============================================================

// Segno di un RESISTORE in una maglia: verso naturale dell'arco (min -> max).
// +1 se la maglia lo percorre min->max, -1 se max->min, 0 se non appartiene.
double plus_minus(const component& resistor, const std::vector<int>& cycle) {

    int start = std::min(resistor.positive_node, resistor.negative_node);
    int end = std::max(resistor.positive_node, resistor.negative_node);

    // ciclo chiuso [n0,...,nk,n0]: si fermano le coppie a size()-1
    for (int i = 0; i < (int)cycle.size() - 1; i++) {
        if (cycle[i] == start && cycle[i+1] == end) {
            return 1.0;
        }
        else if (cycle[i] == end && cycle[i+1] == start) {
            return -1.0;
        }
    }
    return 0;
}

// Matrice di incidenza B (resistori x maglie)
template<typename T>
std::vector<std::vector<double>> build_B(const std::vector<std::vector<int>>& cycles, const std::vector<component>& resistors, const circuit_graph<T>& cg) {

    std::vector<std::vector<double>> B(cg.get_allresistor().size(), std::vector<double>(cycles.size(), 0.0));

    for (int i = 0; i < (int)resistors.size(); i++) {
        for (int j = 0; j < (int)cycles.size(); j++) {
            B[i][j] = plus_minus(resistors[i],cycles[j]);
        }
    }

    return B;
}

// Matrice delle resistenze R (diagonale)
std::vector<std::vector<double>> build_R(const std::vector<component>& resistors) {

    std::vector<std::vector<double>> R(resistors.size(), std::vector<double>(resistors.size(), 0.0));

    for (int i = 0; i < (int)resistors.size(); i++) {
        R[i][i] = resistors[i].c_value;
    }

    return R;
}

// Segno di un GENERATORE in una maglia: in base alla polarita' reale.
// +1 se percorso da - a +, -1 se da + a -, 0 se non appartiene.
int gen_sign(const component& gen, const std::vector<int>& cycle) {
    for (int i = 0; i < (int)cycle.size() - 1; i++) {
        if (cycle[i] == gen.negative_node && cycle[i+1] == gen.positive_node)
            return 1;
        if (cycle[i] == gen.positive_node && cycle[i+1] == gen.negative_node)
            return -1;
    }
    return 0;
}

// Vettore dei termini noti v (un valore per maglia)
std::vector<double> build_v(const std::vector<std::vector<int>>& cycles, const std::vector<component>& generators) {

    std::vector<double> v(cycles.size(), 0.0);

    for (int i = 0; i < (int)cycles.size(); i++) {
        for (int j = 0; j < (int)generators.size(); j++){
            v[i] = v[i] + generators[j].c_value * gen_sign(generators[j], cycles[i]);
        }
    }

    return v;
}
