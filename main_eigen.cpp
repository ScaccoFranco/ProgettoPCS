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
    if (argc < 2) {
        std::cerr << "Errore: Mancata definizione del file \n";
        return 1;
    }
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Errore: impossibile aprire: " << argv[1] << "\n";
        return 1;
    }

    // --- Lettura netlist e costruzione del grafo ---
    circuit_graph<int> cg = build_graph(file);
    std::cout << " \n";

    for (const auto& pair : cg.get_edge_map()) {
        const unidirected_edge<int>& e = pair.first;
        const component& c = pair.second;
        std::cout << c.c_type << c.c_number
                  << " | valore: " << c.c_value
                  << " | nodi: (" << e.from() << ", " << e.to() << ")\n";
    }

    // --- Cicli fondamentali (maglie) ---
    // De Pina (cicli minimi). Per usare la DFS: auto cycles = find_cycles(cg);
    // Entrambi restituiscono cicli CHIUSI [n0,...,nk,n0].
    auto cycles = de_pina(cg.get_graph());

    stampa_cicli(cycles);

    // --- Sistema lineare B^T R B i = v ---
    auto B = build_B(cycles, cg.get_allresistor(), cg);
    auto R = build_R(cg.get_allresistor());
    auto v = build_v(cycles, cg.get_allgenerator());

    auto correnti = solve_system(B, R, v);
    stampa_correnti(correnti);

    // --- Output richiesto: tensioni (e correnti) sui resistori, v_R = R B i ---
    stampa_risultati(correnti, cg, B, cycles);

}
