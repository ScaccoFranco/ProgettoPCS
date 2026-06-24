#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdio> // Per std::remove
#include "../include/input.hpp"

void test_lettura_netlist_sporca() {
    std::string mock_netlist = 
        "R1 10 1 2\n"
        "\n"                   // riga vuota
        "   \n"                // riga con soli spazi
        "ERRORE 99 1 2\n"      // riga malformata
        "V1 10 2 3\n";
        
    std::string temp_filename = "temp_mock_netlist.txt";

    // 1. Creazione e scrittura del file temporaneo
    std::ofstream out_file(temp_filename);
    out_file << mock_netlist;
    out_file.close();

    // 2. Lettura tramite ifstream (come richiesto da build_graph)
    std::ifstream in_file(temp_filename);
    circuit_graph<int> g = build_graph(in_file);
    in_file.close();

    // 3. Asserzioni
    assert(g.get_edge_map().size() == 2);
    assert(g.get_allresistor().size() == 1);
    assert(g.get_allgenerator().size() == 1);

    // 4. Pulizia: rimozione del file temporaneo
    std::remove(temp_filename.c_str());
}

int main() {
    test_lettura_netlist_sporca();
    std::cout << "test_parser: TUTTI I TEST SUPERATI\n";
    return 0;
}