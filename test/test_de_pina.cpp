#include <iostream>
#include <set>
#include <vector>
#include "../include/cicli.hpp"

int peso_ciclo(const std::vector<bool>& C)
{
    int w = 0;
    for (bool b : C) if (b) w++;
    return w;
}

bool ciclo_contiene_arco(const unidirected_graph<int>& G,
                          const std::vector<int>& ciclo,
                          int u, int v)
{
    int m = ciclo.size();
    for (int i = 0; i < m; i++)
    {
        int a = ciclo[i];
        int b = ciclo[(i + 1) % m];
        if ((a == u && b == v) || (a == v && b == u))
            return true;
    }
    return false;
}

bool ciclo_e_valido(const unidirected_graph<int>& G, const std::vector<int>& ciclo)
{
    // ogni nodo del ciclo deve esistere nel grafo
    auto nodi = G.all_nodes();
    for (int n : ciclo)
        if (!nodi.count(n)) return false;

    // ogni arco consecutivo deve esistere nel grafo
    int m = ciclo.size();
    for (int i = 0; i < m; i++)
    {
        int u = ciclo[i];
        int v = ciclo[(i + 1) % m];
        if (G.edge_number(unidirected_edge<int>(u, v)) < 0)
            return false;
    }
    return true;
}

int main()
{
    // --- TEST: grafo con due cicli minimi di lunghezza 3 ---
    // nodi: 1,2,3,4   archi: (1,2)(1,4)(2,3)(2,4)(3,4)
    // k = 5 - 4 + 1 = 2
    // cicli minimi attesi: entrambi di lunghezza 3
    unidirected_graph<int> G;
    G.add_edge(1, 2);
    G.add_edge(1, 4);
    G.add_edge(2, 3);
    G.add_edge(2, 4);
    G.add_edge(3, 4);

    auto cicli = de_pina(G);

    if (cicli.size() != 2)
        return EXIT_FAILURE;

    for (const auto& c : cicli)
    {
        // ogni ciclo deve avere nodi validi e archi esistenti
        if (!ciclo_e_valido(G, c))
            return EXIT_FAILURE;

        // cicli minimi su questo grafo hanno 3 archi
        if (c.size() != 3)
            return EXIT_FAILURE;
    }

    // i due cicli devono essere distinti
    if (cicli[0] == cicli[1])
        return EXIT_FAILURE;

    // --- TEST: triangolo — un solo ciclo di lunghezza 3 ---
    // k = 3 - 3 + 1 = 1
    unidirected_graph<int> G_triangolo;
    G_triangolo.add_edge(1, 2);
    G_triangolo.add_edge(2, 3);
    G_triangolo.add_edge(1, 3);

    auto c_tri = de_pina(G_triangolo);

    if (c_tri.size() != 1)
        return EXIT_FAILURE;
    if (!ciclo_e_valido(G_triangolo, c_tri[0]))
        return EXIT_FAILURE;
    if (c_tri[0].size() != 3)
        return EXIT_FAILURE;
    // deve contenere tutti e tre gli archi del triangolo
    if (!ciclo_contiene_arco(G_triangolo, c_tri[0], 1, 2))
        return EXIT_FAILURE;
    if (!ciclo_contiene_arco(G_triangolo, c_tri[0], 2, 3))
        return EXIT_FAILURE;
    if (!ciclo_contiene_arco(G_triangolo, c_tri[0], 1, 3))
        return EXIT_FAILURE;

    // --- TEST: albero — nessun ciclo ---
    // k = 2 - 3 + 1 = 0
    unidirected_graph<int> G_albero;
    G_albero.add_edge(1, 2);
    G_albero.add_edge(2, 3);

    auto c_alb = de_pina(G_albero);

    if (!c_alb.empty())
        return EXIT_FAILURE;

    // --- TEST: quadrato con diagonale ---
    // nodi: 1,2,3,4   archi: (1,2)(2,3)(3,4)(1,4)(1,3)
    // k = 5 - 4 + 1 = 2
    // De Pina deve trovare i due triangoli (lunghezza 3), non il quadrato (lunghezza 4)
    unidirected_graph<int> G_quad;
    G_quad.add_edge(1, 2);
    G_quad.add_edge(2, 3);
    G_quad.add_edge(3, 4);
    G_quad.add_edge(1, 4);
    G_quad.add_edge(1, 3);

    auto c_quad = de_pina(G_quad);

    if (c_quad.size() != 2)
        return EXIT_FAILURE;
    for (const auto& c : c_quad)
    {
        if (!ciclo_e_valido(G_quad, c))
            return EXIT_FAILURE;
        // verifica minimalità: deve trovare triangoli, non il quadrato
        if (c.size() != 3)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}