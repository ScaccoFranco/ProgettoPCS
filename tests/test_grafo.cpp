#include <iostream>
#include <cassert>
#include "../include/unidirected_graph.hpp"
#include "../include/algoritmi_grafi.hpp"

void test_inserimento_nodi_archi() {
    unidirected_graph<int> g;
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    g.add_edge(1, 2); // Inserimento duplicato

    // Verificando il duplicato correttamente ignorato
    assert(g.all_nodes().size() == 3);
    assert(g.all_edges().size() == 2);
}

void test_sottrazione_grafi() {
    unidirected_graph<int> g;
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    g.add_edge(3, 1); // Chiusura di un ciclo (3 nodi, 3 archi)

    // Estrazione albero di supporto
    unidirected_graph<int> albero = recursive_dfs(g, 1);
    
    // Calcolo coalbero
    unidirected_graph<int> coalbero = g - albero;

    // Un albero su 3 nodi deve avere esattamente 2 archi
    assert(albero.all_edges().size() == 2);
    
    // Il coalbero deve contenere gli archi rimanenti (|E| - |V| + 1 = 3 - 3 + 1 = 1)
    assert(coalbero.all_edges().size() == 1);
}

void test_dijkstra() {
    unidirected_graph<int> g;
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    
    DijkstraResult<int> result = dijkstra(g, 1);
    std::vector<int> path = get_path(result, 1, 3);

    assert(path.size() == 3);
    assert(path[0] == 1);
    assert(path[1] == 2);
    assert(path[2] == 3);
}

int main() {
    test_inserimento_nodi_archi();
    test_sottrazione_grafi();
    test_dijkstra();
    
    std::cout << "test_grafo: TUTTI I TEST SUPERATI\n";
    return 0;
}