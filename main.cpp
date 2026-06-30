#include <Eigen/Dense>
#include <iostream>
#include <fstream>
#include "./include/elements.hpp"
#include "./include/input.hpp"
#include "./include/output.hpp"
#include "./include/cicli.hpp"
#include "./include/circuit.hpp"
#include "./include/timer.hpp"

int main(int argc, const char *argv[])
{

    bool usa_bfs, misura_tempo, usa_find_cycles;
    const char* nome_file;

    if (!leggi_argomenti(argc, argv, usa_bfs, misura_tempo, usa_find_cycles, nome_file))
        return 1;

    std::ifstream file(nome_file);
    if (!file.is_open()) {
        std::cerr << "Errore: impossibile aprire: " << nome_file << "\n";
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
    // (con scelta dell'algoritmo di cammino minimo e misurazione opzionale)

    Timer cronometro;
    if (misura_tempo)
        cronometro.tic();

    auto cycles = usa_find_cycles ? find_cycles(cg.get_graph()) : de_pina(cg.get_graph(), usa_bfs);

    stampa_cicli(cycles);

    // costruzione delle matrici e del sistema lineare risolvendo
    // attraverso il gradiente coniugato stampando i risultati

    auto B = build_B(cycles, cg.get_allresistor(), cg);
    auto R = build_R(cg.get_allresistor());
    auto v = build_v(cycles, cg.get_allgenerator());

    auto correnti = solve_system(B, R, v);
    stampa_correnti(correnti);

    stampa_risultati(correnti, cg, B, cycles);

    if (misura_tempo)
        stampa_tempo(usa_find_cycles, usa_bfs, cronometro.toc());
}