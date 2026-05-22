#include <iostream>
#include <set>
#include <vector>
#include "../include/algoritmi_grafi.hpp"

bool percorso_contiene(const std::vector<int>& percorso, int nodo)
{
    for (int n : percorso)
        if (n == nodo) return true;
    return false;
}

bool percorso_connette(const std::vector<int>& percorso, int u, int v)
{
    return percorso_contiene(percorso, u) && percorso_contiene(percorso, v);
}

int main()
{
    // --- TEST find_path ---

    unidirected_graph<int> T;
    T.add_edge(1, 2);
    T.add_edge(2, 3);
    T.add_edge(3, 4);

    std::set<int> visitati;
    std::vector<int> path;
    bool trovato = find_path(T, visitati, path, 1, 4);
    if (!(trovato && path.front() == 1 && path.back() == 4 && path.size() == 4))
        return EXIT_FAILURE;

    visitati.clear();
    path.clear();
    trovato = find_path(T, visitati, path, 2, 2);
    if (!(trovato && path.size() == 1 && path.front() == 2))
        return EXIT_FAILURE;

    unidirected_graph<int> T_disco;
    T_disco.add_edge(1, 2);
    T_disco.add_edge(4, 5);
    visitati.clear();
    path.clear();
    trovato = find_path(T_disco, visitati, path, 1, 5);
    if (!(!trovato && path.empty()))
        return EXIT_FAILURE;

    unidirected_graph<int> T_y;
    T_y.add_edge(1, 2);
    T_y.add_edge(2, 3);
    T_y.add_edge(2, 4);
    visitati.clear();
    path.clear();
    trovato = find_path(T_y, visitati, path, 1, 4);
    if (!(trovato && percorso_connette(path, 1, 4) && !percorso_contiene(path, 3)))
        return EXIT_FAILURE;



    // --- TEST cicli_dfs ---

    unidirected_graph<int> G;
    G.add_edge(1, 2);
    G.add_edge(1, 4);
    G.add_edge(2, 3);
    G.add_edge(2, 4);
    G.add_edge(3, 4);

    auto cicli = cicli_dfs(G, 1);

    if (cicli.size() != 2)
        return EXIT_FAILURE;

    for (const auto& c : cicli)
        if (c.size() < 2)
            return EXIT_FAILURE;

    auto nodi_G = G.all_nodes();
    for (const auto& c : cicli)
        for (int n : c)
            if (!nodi_G.count(n))
                return EXIT_FAILURE;

    unidirected_graph<int> G_triangolo;
    G_triangolo.add_edge(1, 2);
    G_triangolo.add_edge(2, 3);
    G_triangolo.add_edge(1, 3);
    auto c = cicli_dfs(G_triangolo, 1);
    if (c.size() != 1)
        return EXIT_FAILURE;

    unidirected_graph<int> G_albero;
    G_albero.add_edge(1, 2);
    G_albero.add_edge(2, 3);
    c = cicli_dfs(G_albero, 1);
    if (!c.empty())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}