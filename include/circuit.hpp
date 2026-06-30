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


// restituisce il valore B(i,j), segno se i versi di percorrimento corrisponsono al verso che definiamo come positivo
double plus_minus(const component& resistor, const std::vector<int>& cycle) {
    // questo serve perchè a differenza dei generatori vogliamo arbitrarizzare il verso del resistore, in quanto non fa differenza
    int start = std::min(resistor.positive_node, resistor.negative_node);
    int end = std::max(resistor.positive_node, resistor.negative_node);

    for (int i = 0; i + 1 < (int)cycle.size(); i++) {
        if (cycle[i] == start && cycle[i+1] == end) {
            return 1.0; // resistore i appartiene alla maglia j con stesso verso di percorrenza
        }
        else if (cycle[i] == end && cycle[i+1] == start) {
            return -1.0; // resistore i appartiene alla maglia j con verso di percorrenza opposto
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

// matrice diagonale con i valori delle resistenze
Eigen::MatrixXd build_R(const std::vector<component>& resistors) {

    int n = resistors.size();
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(n, n);

    for (int i = 0; i < n; i++)
        R(i, i) = resistors[i].c_value;

    return R;
}

int gen_sign(const component& gen, const std::vector<int>& cycle) {
    // qua non calcoliamo min e max come in plus_minus perché i generatori hanno il loro verso
    for (int i = 0; i + 1 < cycle.size(); i++) {
        if (cycle[i] == gen.negative_node && cycle[i+1] == gen.positive_node)
            return 1; // verso concorde
        if (cycle[i] == gen.positive_node && cycle[i+1] == gen.negative_node)
            return -1; // verso opposto
    }
    return 0; // non attraversato dalla maglia
}

// costruzione del termine noto, ogni elemento di v equivale alla sommacon segno 
// della ddp dei generatori attraversati dalla maglia. 
Eigen::VectorXd build_v(const std::vector<std::vector<int>>& cycles, const std::vector<component>& generators) {

    Eigen::VectorXd v = Eigen::VectorXd::Zero(cycles.size());

    for (int i = 0; i < (int)cycles.size(); i++)
        for (int j = 0; j < (int)generators.size(); j++)
            v(i) += generators[j].c_value * gen_sign(generators[j], cycles[i]);

    return v;
}


// si applica il gradiente coniugato e si ha la soluzione
Eigen::VectorXd solve_system(const Eigen::MatrixXd& B, const Eigen::MatrixXd& R, Eigen::VectorXd& v)
{
    Eigen::MatrixXd A = B.transpose() * R * B;
    return gradiente_coniugato(A, v);
}
