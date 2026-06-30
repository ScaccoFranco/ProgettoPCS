#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include "elements.hpp"


// Legge gli argomenti da riga di comando.
// Restituisce true se il parsing ha successo, false in caso di errore
// (in tal caso ha gia' stampato il messaggio su std::cerr)..
bool leggi_argomenti(int argc, const char* argv[], bool& usa_bfs, bool& misura_tempo, bool& usa_find_cycles, const char*& nome_file)
{
    // valori di default
    usa_bfs = false;
    misura_tempo = false;
    usa_find_cycles = false;
    nome_file = nullptr;

    if (argc < 2) {
        std::cerr << "Errore: Mancata definizione del file \n";
        std::cerr << "Uso: " << argv[0] << " <file> [--bfs] [--time] [--no-minimi]\n";
        return false;
    }

    //   --bfs        usa la BFS al posto di Dijkstra dentro De Pina
    //   --time       misura e stampa il tempo impiegato dal calcolo dei cicli
    //   --no-minimi  usa find_cycles invece di de_pina, quindi non cerca cicli minimi

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--bfs") usa_bfs = true;
        else if (arg == "--time") misura_tempo = true;
        else if (arg == "--no-minimi") usa_find_cycles = true;
        else if (nome_file == nullptr) nome_file = argv[i];
    }

    if (nome_file == nullptr) {
        std::cerr << "Errore: Mancata definizione del file \n";
        return false;
    }

    return true;
}


// Parser che legge la netlist riga per riga e popola il grafo del circuito. 
// Il controllo in fase di estrazione (ss >> ...) funge da filtro di sicurezza: 
// se una riga è vuota, scritta male o contiene caratteri non ammessi, 
// viene semplicemente ignorata evitando che il programma abbia problemi.

circuit_graph<int> build_graph(std::ifstream& input_file)
{
    circuit_graph<int> g;

    std::string line;

    while (std::getline(input_file, line)) {

        std::istringstream ss(line);
        char type;
        int number;
        double value;
        int n1, n2;
        if (!(ss >> type >> number >> value >> n1 >> n2)) continue;

        component c(type, number, value, n1, n2);

        g.add_component(c, n1, n2);
    }

    return g;
}
