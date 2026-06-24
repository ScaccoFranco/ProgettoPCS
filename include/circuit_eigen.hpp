#pragma once

#include <Eigen/Dense>
#include <vector>
#include <string>
#include "eigen_support.hpp"
#include "unidirected_graph.hpp"
#include "elements.hpp"

// Metodo delle Correnti di Maglia. 
// Si traducono i percorsi del grafo in equazioni lineari: "plus_minus" e "gen_sign" 
// risolvono le difficoltà dei segni valutando come le maglie attraversano i componenti. 
// Le funzioni "build_B", "build_R" e "build_v" assemblano rispettivamente la matrice 
// di incidenza rami-maglie, la matrice diagonale delle resistenze e il vettore noto 
// dei generatori. Infine, "solve_system" unisce i pezzi creando la matrice quadrata 
// A = B^T * R * B e lo chiama la funzione per risolverlo.

double plus_minus(const component& resistor, const std::vector<int>& cycle) {

    int start = std::min(resistor.positive_node, resistor.negative_node);
    int end = std::max(resistor.positive_node, resistor.negative_node);

    for (int i = 0; i + 1 < (int)cycle.size(); i++) {
        if (cycle[i] == start && cycle[i+1] == end) {
            return 1.0;
        }
        else if (cycle[i] == end && cycle[i+1] == start) {
            return -1.0;
        }
    }
    return 0;
}

template<typename T>
Eigen::MatrixXd build_B(const std::vector<std::vector<int>>& cycles, const std::vector<component>& resistors, const circuit_graph<T>& cg) {

    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(cg.get_allresistor().size(), cycles.size());

    for (int i = 0; i < (int)resistors.size(); i++)
        for (int j = 0; j < (int)cycles.size(); j++)
            B(i, j) = plus_minus(resistors[i], cycles[j]);

    return B;
}

Eigen::MatrixXd build_R(const std::vector<component>& resistors) {

    int n = resistors.size();
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(n, n);

    for (int i = 0; i < n; i++)
        R(i, i) = resistors[i].c_value;

    return R;
}

int gen_sign(const component& gen, const std::vector<int>& cycle) {
    for (int i = 0; i + 1 < cycle.size(); i++) {
        if (cycle[i] == gen.negative_node && cycle[i+1] == gen.positive_node)
            return 1;
        if (cycle[i] == gen.positive_node && cycle[i+1] == gen.negative_node)
            return -1;
    }
    return 0;
}

Eigen::VectorXd build_v(const std::vector<std::vector<int>>& cycles, const std::vector<component>& generators) {

    Eigen::VectorXd v = Eigen::VectorXd::Zero(cycles.size());

    for (int i = 0; i < (int)cycles.size(); i++)
        for (int j = 0; j < (int)generators.size(); j++)
            v(i) += generators[j].c_value * gen_sign(generators[j], cycles[i]);

    return v;
}

Eigen::VectorXd solve_system(const Eigen::MatrixXd& B, const Eigen::MatrixXd& R, Eigen::VectorXd& v)
{
    Eigen::MatrixXd A = B.transpose() * R * B;
    return gradiente_coniugato(A, v);
}
