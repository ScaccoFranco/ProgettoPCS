#include <Eigen/Dense>
#include <iostream>
#include <fstream>
#include "./include/elements.hpp"
#include "./include/input.hpp"
#include "./include/output_eigen.hpp"
#include "./include/cicli_eigen.hpp"
#include "./include/circuit_eigen.hpp"

int main(int argc, const char *argv[])
{

    // apertura del file e controllo in caso di mancato file
    // o impossibilità ad aprire

    if (argc < 2) {
        std::cerr << "Errore: Mancata definizione del file \n";
        return 1;
    }
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Errore: impossibile aprire: " << argv[1] << "\n";
        return 1;
    }

    // costruzione del grafico attraverso build_graph() 
    // e popolando un circuit_graph

    circuit_graph<int> cg = build_graph(file);
    std::cout << " \n";

    for (const auto& pair : cg.get_edge_map()) {
        const unidirected_edge<int>& e = pair.first;
        const component& c = pair.second;
        std::cout << c.c_type << c.c_number
                  << " | valore: " << c.c_value
                  << " | nodi: (" << e.from() << ", " << e.to() << ")\n";
    }

    // calcolo e output dei cicli minimi attraverso algoritmo di de_pina 

    auto cycles = de_pina(cg.get_graph());
    stampa_cicli(cycles);

    // costruzione delle matrici e del sistema lineare risolvendo
    // attraverso il gradiente coniugato stampando i risultati

    auto B = build_B(cycles, cg.get_allresistor(), cg);
    auto R = build_R(cg.get_allresistor());
    auto v = build_v(cycles, cg.get_allgenerator());

    auto correnti = solve_system(B, R, v);
    stampa_correnti(correnti);

    stampa_risultati(correnti, cg, B, cycles);

}
