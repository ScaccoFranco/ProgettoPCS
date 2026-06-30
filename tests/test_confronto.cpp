#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cassert>
#include "../include/input.hpp"
#include "../include/cicli.hpp"
#include "../include/circuit.hpp"

// Le liste di cicli non sono uguali e non ha senso confrontarle nodo per nodo.
// Cio' che invece deve coincidere e' il risultato fisico, cioe' le correnti
// sui rami: non dipende da quale base di cicli si sceglie.
Eigen::VectorXd correnti_con(std::vector<std::vector<int>>& cicli, const circuit_graph<int>& cg)
{
    auto B = build_B(cicli, cg.get_allresistor(), cg);
    auto R = build_R(cg.get_allresistor());
    auto v = build_v(cicli, cg.get_allgenerator());
    auto correnti_di_maglia = solve_system(B, R, v);
    return B * correnti_di_maglia;   // correnti vere sui rami
}

void test_stesse_correnti() {
    std::ifstream file("../input/circuit_s.txt");
    circuit_graph<int> cg = build_graph(file);
    file.close();

    std::vector<std::vector<int>> cicli_albero = find_cycles(cg.get_graph());
    std::vector<std::vector<int>> cicli_depina = de_pina(cg.get_graph(), false);

    Eigen::VectorXd correnti_albero = correnti_con(cicli_albero, cg);
    Eigen::VectorXd correnti_depina = correnti_con(cicli_depina, cg);

    double differenza_massima = (correnti_albero - correnti_depina).cwiseAbs().maxCoeff();
    assert(differenza_massima < 1e-9); // uso come tol 1e-9
}

int main() {
    test_stesse_correnti();
    std::cout << "test_cicli: TUTTI I TEST SUPERATI\n";
    return 0;
}