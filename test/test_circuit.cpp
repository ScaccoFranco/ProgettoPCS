
#include "../include/circuit.hpp"
#include "../include/input.hpp"
#include "../include/cicli.hpp"

int main()
{
// --- TEST 1: parsing corretto della netlist di prova dal progetto ---
    // V1 30.0 1 4 | V2 40.0 3 5
    // R1 4.0 4 2  | R2 10.0 1 2 | R3 30.0 1 3 | R4 10.0 3 2 | R5 4.0 2 5
    write_temp(
        "V 1 30.0 1 4\n"
        "V 2 40.0 3 5\n"
        "R 1 4.0 4 2\n"
        "R 2 10.0 1 2\n"
        "R 3 30.0 1 3\n"
        "R 4 10.0 3 2\n"
        "R 5 4.0 2 5\n",
        "/tmp/test_netlist.txt"
    );
    std::ifstream f1("/tmp/test_netlist.txt");
    circuit_graph<int> cg = build_graph(f1);

    // numero corretto di componenti
    if (cg.get_allresistor().size()  != 5) return EXIT_FAILURE;
    if (cg.get_allgenerator().size() != 2) return EXIT_FAILURE;

    // valori e nodi dei resistori
    const auto& res = cg.get_allresistor();
    if (res[0].c_type != 'R' || res[0].c_number != 1 || res[0].c_value != 4.0)   return EXIT_FAILURE;
    if (res[1].c_type != 'R' || res[1].c_number != 2 || res[1].c_value != 10.0)  return EXIT_FAILURE;
    if (res[4].c_type != 'R' || res[4].c_number != 5 || res[4].c_value != 4.0)   return EXIT_FAILURE;

    // valori e nodi dei generatori
    const auto& gen = cg.get_allgenerator();
    if (gen[0].c_type != 'V' || gen[0].c_number != 1 || gen[0].c_value != 30.0)  return EXIT_FAILURE;
    if (gen[1].c_type != 'V' || gen[1].c_number != 2 || gen[1].c_value != 40.0)  return EXIT_FAILURE;

    // nodi corretti nel grafo (1,2,3,4,5)
    auto nodi = cg.get_graph().all_nodes();
    if (nodi.size() != 5)          return EXIT_FAILURE;
    if (!nodi.count(1))            return EXIT_FAILURE;
    if (!nodi.count(5))            return EXIT_FAILURE;

    // archi nel grafo: 7 componenti = 7 archi
    if (cg.get_graph().all_edges().size() != 7) return EXIT_FAILURE;

    // --- TEST 2: righe vuote e righe malformate vengono ignorate ---
    write_temp(
        "\n"
        "   \n"
        "R 1 10.0 1 2\n"
        "riga malformata senza senso\n"
        "V 1 5.0 2 3\n"
        "\n",
        "/tmp/test_netlist_sparse.txt"
    );
    std::ifstream f2("/tmp/test_netlist_sparse.txt");
    circuit_graph<int> cg2 = build_graph(f2);

    if (cg2.get_allresistor().size()  != 1) return EXIT_FAILURE;
    if (cg2.get_allgenerator().size() != 1) return EXIT_FAILURE;

    // --- TEST 3: file vuoto -> grafo vuoto ---
    write_temp("", "/tmp/test_netlist_empty.txt");
    std::ifstream f3("/tmp/test_netlist_empty.txt");
    circuit_graph<int> cg3 = build_graph(f3);

    if (!cg3.get_allresistor().empty())  return EXIT_FAILURE;
    if (!cg3.get_allgenerator().empty()) return EXIT_FAILURE;

    // pulizia file temporanei
    //std::remove("/tmp/test_netlist.txt");
    std::remove("/tmp/test_netlist_sparse.txt");
    std::remove("/tmp/test_netlist_empty.txt");

    // -- TEST build_graph --
    circuit_graph<int> cg = build_graph("/tmp/test_netlist.txt");

    if (cg.get_allresistor().size() != 5)  return EXIT_FAILURE;
    if (cg.get_allgenerator().size() != 2) return EXIT_FAILURE;

    // -- TEST find_cycles --
    auto cycles = de_pina(cg);

    // k = |E| - |V| + 1 = 7 - 5 + 1 = 3 cicli fondamentali
    if (cycles.size() != 3) return EXIT_FAILURE;

    // ogni ciclo deve avere almeno 2 nodi
    for (const auto& c : cycles)
        if (c.size() < 2) return EXIT_FAILURE;

    // -- TEST build_B --
    auto B = build_B(cycles, cg.get_allresistor(), cg);

    // B deve essere 5 righe (resistori) x 3 colonne (cicli)
    if (B.size() != 5)       return EXIT_FAILURE;
    if (B[0].size() != 3)    return EXIT_FAILURE;

    // ogni elemento deve essere -1, 0 o +1
    for (const auto& row : B)
        for (double val : row)
            if (val != -1.0 && val != 0.0 && val != 1.0)
                return EXIT_FAILURE;

    // -- TEST build_v --
    auto v = build_v(cycles, cg.get_allgenerator());

    if (v.size() != 3) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}