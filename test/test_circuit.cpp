#include <fstream>
#include <cstdlib>
#include <cmath>

#include "../include/circuit.hpp"
#include "../include/input.hpp"
#include "../include/cicli.hpp"

// Scrive una stringa su file (sostituisce write_temp che non esiste negli header)
static void write_file(const std::string& content, const std::string& path)
{
    std::ofstream f(path);
    f << content;
}

int main()
{
    // =========================================================
    // TEST 1: parsing corretto della netlist di prova dal progetto
    //   V1 30.0 1 4 | V2 40.0 3 5
    //   R1 4.0 4 2  | R2 10.0 1 2 | R3 30.0 1 3 | R4 10.0 3 2 | R5 4.0 2 5
    // =========================================================
    write_file(
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

    // nodi e segni dei resistori
    if (res[0].positive_node != 4 || res[0].negative_node != 2) return EXIT_FAILURE;
    if (res[1].positive_node != 1 || res[1].negative_node != 2) return EXIT_FAILURE;

    // valori e nodi dei generatori
    const auto& gen = cg.get_allgenerator();
    if (gen[0].c_type != 'V' || gen[0].c_number != 1 || gen[0].c_value != 30.0)  return EXIT_FAILURE;
    if (gen[1].c_type != 'V' || gen[1].c_number != 2 || gen[1].c_value != 40.0)  return EXIT_FAILURE;

    // nodi corretti nel grafo: devono esserci 1, 2, 3, 4, 5
    auto nodi = cg.get_graph().all_nodes();
    if (nodi.size() != 5) return EXIT_FAILURE;
    if (!nodi.count(1))   return EXIT_FAILURE;
    if (!nodi.count(2))   return EXIT_FAILURE;
    if (!nodi.count(3))   return EXIT_FAILURE;
    if (!nodi.count(4))   return EXIT_FAILURE;
    if (!nodi.count(5))   return EXIT_FAILURE;

    // 7 componenti = 7 archi
    if (cg.get_graph().all_edges().size() != 7) return EXIT_FAILURE;

    // =========================================================
    // TEST 2: righe vuote e righe malformate vengono ignorate
    // =========================================================
    write_file(
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

    // =========================================================
    // TEST 3: file vuoto -> grafo vuoto
    // =========================================================
    write_file("", "/tmp/test_netlist_empty.txt");

    std::ifstream f3("/tmp/test_netlist_empty.txt");
    circuit_graph<int> cg3 = build_graph(f3);

    if (!cg3.get_allresistor().empty())  return EXIT_FAILURE;
    if (!cg3.get_allgenerator().empty()) return EXIT_FAILURE;

    // =========================================================
    // TEST 4: de_pina sul grafo principale
    //   k = |E| - |V| + 1 = 7 - 5 + 1 = 3 cicli fondamentali
    // =========================================================
    // de_pina prende unidirected_graph, non circuit_graph
    auto cycles = de_pina(cg.get_graph());

    if (cycles.size() != 3) return EXIT_FAILURE;

    // ogni ciclo deve avere almeno 2 nodi
    for (const auto& c : cycles)
        if (c.size() < 2) return EXIT_FAILURE;

    // =========================================================
    // TEST 5: build_B
    //   B deve essere: #resistori righe x #cicli colonne = 5x3
    //   ogni elemento deve essere -1, 0 o +1
    // =========================================================

    // plus_minus si aspetta che il ciclo abbia il nodo di ritorno in coda,
    // cioè percorso = [n0, n1, ..., nk, n0]. incidenza_to_nodi non lo aggiunge,
    // quindi lo aggiungiamo qui prima di passare i cicli a build_B / build_v.
    std::vector<std::vector<int>> cycles_closed;
    for (auto c : cycles) {
        c.push_back(c.front()); // chiude il ciclo
        cycles_closed.push_back(c);
    }

    auto B = build_B(cycles_closed, cg.get_allresistor(), cg);

    if (B.size() != 5)    return EXIT_FAILURE;
    if (B[0].size() != 3) return EXIT_FAILURE;

    for (const auto& row : B)
        for (double val : row)
            if (val != -1.0 && val != 0.0 && val != 1.0)
                return EXIT_FAILURE;

    // =========================================================
    // TEST 6: build_v
    //   dimensione = numero di cicli = 3
    // =========================================================
    auto v = build_v(cycles_closed, cg.get_allgenerator());

    if (v.size() != 3) return EXIT_FAILURE;

    // =========================================================
    // TEST 7: build_R
    //   matrice diagonale 5x5, diagonale = valori resistori
    // =========================================================
    auto R = build_R(cg.get_allresistor());

    if (R.size() != 5)    return EXIT_FAILURE;
    if (R[0].size() != 5) return EXIT_FAILURE;

    // diagonale corretta
    const double expected_r[] = {4.0, 10.0, 30.0, 10.0, 4.0};
    for (int i = 0; i < 5; i++) {
        if (R[i][i] != expected_r[i]) return EXIT_FAILURE;
        for (int j = 0; j < 5; j++)
            if (i != j && R[i][j] != 0.0) return EXIT_FAILURE;
    }

    // =========================================================
    // pulizia
    // =========================================================
    std::remove("/tmp/test_netlist.txt");
    std::remove("/tmp/test_netlist_sparse.txt");
    std::remove("/tmp/test_netlist_empty.txt");

    return EXIT_SUCCESS;
}