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

    // apertura del file e controllo in caso di mancato file
    // o impossibilità ad aprire

    if (argc < 2) {
        std::cerr << "Errore: Mancata definizione del file \n";
        std::cerr << "Uso: " << argv[0] << " <file> [--bfs] [--time] [--no-minimi]\n";
        return 1;
    }

    //   --bfs   usa la BFS al posto di Dijkstra dentro De Pina
    //   --time  misura e stampa il tempo impiegato dal calcolo dei cicli
    //   --no-minimi usa la funzione find_cycles invece di de pina, quindi non cerca cicli minimi

    bool usa_bfs = false;
    bool misura_tempo = false;
    bool usa_find_cycles = false;

    const char* nome_file = nullptr;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--bfs") usa_bfs = true;
        else if (arg == "--time") misura_tempo = true;
        else if (arg == "--no-minimi") usa_find_cycles = true;
        else if (nome_file == nullptr) nome_file = argv[i];
    }
    if (nome_file == nullptr) {
        std::cerr << "Errore: Mancata definizione del file \n";
        return 1;
    }

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

    if (misura_tempo) {
        std::cout << "tempo - usando " << (usa_find_cycles ? "find_cycles" : "De Pina") << " con " << (usa_bfs ? "BFS" : "Dijkstra") << ": " << cronometro.toc() << " ms\n";
    }
}