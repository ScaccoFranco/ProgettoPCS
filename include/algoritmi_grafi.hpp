#pragma once

#include <queue>
#include <stack>
#include <ostream>
#include <set>
#include <map>
#include <vector>
#include <stdexcept>
#include <limits>
#include <algorithm>
#include "unidirected_graph.hpp"

// Contiene gli algoritmi di esplorazione dei grafi.
// Implementa la DFS ricorsiva per estrarre l'albero di supporto 
// (usato nella ricerca base delle maglie) e l'algoritmo di Dijkstra per i cammini minimi. 
// Dijkstra, in particolare, è il motore dell'algoritmo di De Pina.

template<typename T>
class fifo
{
private:
    std::queue<T> q;
public:
    void put(T x) { q.push(x); }
    T get() { T x = q.front(); q.pop(); return x; }
    bool empty() { return q.empty(); }
};

template<typename T>
class lifo
{
private:
    std::stack<T> s;
public:
    void put(T x) { s.push(x); }
    T get() { T x = s.top(); s.pop(); return x; }
    bool empty() { return s.empty(); }
};

template<typename T, typename Contenitore>
unidirected_graph<T> graph_visit(const unidirected_graph<T>& G, T sorgente, Contenitore contenitore)
{
    unidirected_graph<T> albero;
    std::set<T> visitati;

    contenitore.put(sorgente);
    visitati.insert(sorgente);

    while (!contenitore.empty())
    {
        T nodo = contenitore.get();
        for (const T& vicino : G.neighbors(nodo))
        {
            if (visitati.find(vicino) == visitati.end())
            {
                visitati.insert(vicino);
                contenitore.put(vicino);
                albero.add_edge(nodo, vicino);
            }
        }
    }
    return albero;
}

template<typename T>
void ricorsione(const unidirected_graph<T>& G, T nodo, std::set<T>& visitati, unidirected_graph<T>& albero)
{
    visitati.insert(nodo);
    for (const T& vicino : G.neighbors(nodo))
    {
        if (visitati.find(vicino) == visitati.end())
        {
            albero.add_edge(nodo, vicino);
            ricorsione(G, vicino, visitati, albero);
        }
    }
}

template<typename T>
unidirected_graph<T> recursive_dfs (const unidirected_graph<T>& G, T sorgente)
{
    unidirected_graph<T> albero;
    std::set<T> visitati;
    ricorsione(G, sorgente, visitati, albero);
    return albero;
}

template<typename T>
struct DijkstraResult {
    std::map<T, int> dist;
    std::map<T, T> prev;
};

template<typename T>
DijkstraResult<T> dijkstra(const unidirected_graph<T>& G, T sorgente)
{
    DijkstraResult<T> result;

    for (const T& nodo : G.all_nodes())
        result.dist[nodo] = std::numeric_limits<int>::max();
    result.dist[sorgente] = 0;

    std::set<std::pair<int, T>> da_visitare;
    da_visitare.insert({0, sorgente});

    while (!da_visitare.empty())
    {
        auto [d, u] = *da_visitare.begin();
        da_visitare.erase(da_visitare.begin());

        for (const T& vicino : G.neighbors(u))
        {
            int nuova_dist = d + 1;
            if (nuova_dist < result.dist[vicino])
            {
                da_visitare.erase({result.dist[vicino], vicino});
                result.dist[vicino] = nuova_dist;
                result.prev[vicino] = u;
                da_visitare.insert({nuova_dist, vicino});
            }
        }
    }

    return result;
}


// BFS: cammini minimi su grafo a peso unitario, alternativa a dijkstra().
// Restituisce lo stesso DijkstraResult<T> per riusare get_path().
// Riusa il contenitore fifo<T> definito sopra (stessa logica di graph_visit).
template<typename T>
DijkstraResult<T> bfs(const unidirected_graph<T>& G, T sorgente)
{
    DijkstraResult<T> result;
 
    for (const T& nodo : G.all_nodes())
        result.dist[nodo] = std::numeric_limits<int>::max();
    result.dist[sorgente] = 0;
 
    fifo<T> coda;
    coda.put(sorgente);
 
    while (!coda.empty())
    {
        T u = coda.get();
        for (const T& vicino : G.neighbors(u))
        {
            if (result.dist[vicino] == std::numeric_limits<int>::max())
            {
                result.dist[vicino] = result.dist[u] + 1;
                result.prev[vicino] = u;
                coda.put(vicino);
            }
        }
    }
 
    return result;
}
 
// Wrapper che instrada su bfs() o dijkstra() in base al flag.
template<typename T>
DijkstraResult<T> cammino_minimo(const unidirected_graph<T>& G, T sorgente, bool usa_bfs)
{
    return usa_bfs ? bfs(G, sorgente) : dijkstra(G, sorgente);
}



// usata in de pina:
// funzione che seve per ricavare un percorso a partire da un DijkstraResult, non calcola direttamente
// differenz a con la funzione successiva: questa non calcola niente, ricava solo il path seguendo i collegamenti già fatti
template<typename T>
std::vector<T> get_path(const DijkstraResult<T>& r, T sorgente, T destinazione)
{
    if (r.dist.find(destinazione) == r.dist.end() || r.dist.at(destinazione) == std::numeric_limits<int>::max())
        return {};

    std::vector<T> path;
    T current = destinazione;
    while (current != sorgente)
    {
        path.push_back(current);
        current = r.prev.at(current);
    }
    path.push_back(sorgente);
    std::reverse(path.begin(), path.end());
    return path;
}



// algortimi per find_cycles, ovvero il metdodo basato sul dfs
// cicli non necessariamente minimi.
