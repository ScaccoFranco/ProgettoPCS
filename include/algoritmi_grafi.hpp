#pragma once

#include <queue>
#include <stack>
#include <ostream>
#include <set>
#include <map>
#include <vector>
#include <stdexcept> 
#include <limits>
#include "unidirected_graph.hpp"

template<typename T>
class fifo
{
private:
    std::queue<T> q;

public:
    void put(T x) {
        q.push(x); 
    }
    T get() { 
        T x = q.front(); 
        q.pop(); return x; 
    }
    bool empty() { 
        return q.empty(); 
    }
};

template<typename T>
class lifo
{
private:
    std::stack<T> s;
    
public:
    void put(T x) { 
        s.push(x); 
    }
    T get() { 
        T x = s.top(); 
        s.pop(); return x; 
    }
    bool empty() { 
        return s.empty(); 
    }
};

template<typename T, typename Contenitore>
unidirected_graph<T> graph_visit(const unidirected_graph<T>& G, T sorgente, Contenitore contenitore)
{
    unidirected_graph<T> albero;  // grafo risultante (l'albero di visita)
    std::set<T> visitati; // nodi già visitati, uso set e non array di booleani, più semplice (ma piu spazio ma vabbe)

    contenitore.put(sorgente);
    visitati.insert(sorgente);

    while (!contenitore.empty())
    {

        T nodo = contenitore.get();    // estrae dalla testa (fifo) o dalla cima (lifo), vista in ampiezza o profondità

        for (const T& vicino : G.neighbors(nodo))
        {
            if (visitati.find(vicino) == visitati.end())  // se non ancora visitato
            {
                visitati.insert(vicino);
                contenitore.put(vicino);
                albero.add_edge(nodo, vicino); // arco dell'albero di visita
            }
        }
    }

    return albero;
}


// uso stessa lgica del dfs iterativo ma uso la ricorsione per i vicini dei vicini ecc

// dentro la funzione
template<typename T>
void ricorsione(const unidirected_graph<T>& G, T nodo, std::set<T>& visitati, unidirected_graph<T>& albero)
{
    // ogni nodo che viene passato alla funzione diventa visitato e si cerca tra tutti i vicini che vengono ricorsivamente passati alla funzione.

    visitati.insert(nodo);
    for (const T& vicino : G.neighbors(nodo))
    {
        if (visitati.find(vicino) == visitati.end())  // se non visitato
        {
            albero.add_edge(nodo, vicino); // arco dell'albero
            ricorsione(G, vicino, visitati, albero); // chiamata ricorsiva
        }
    }
}

// da chiamare
template<typename T>
unidirected_graph<T> recursive_dfs (const unidirected_graph<T>& G, T sorgente)
{
    unidirected_graph<T> albero;
    std::set<T> visitati;
    ricorsione(G, sorgente, visitati, albero);
    return albero;
}


// djkastra seve a trovare il cammino più corto, la mia classe non ha i pesi sugli archi, quindi li faccio tutti = 1
// faccio quindi in modo che stampi output nodo - distanza, non albero
template<typename T>
std::map<T, int> dijkstra(const unidirected_graph<T>& G, T sorgente)
{
    // usando tutti i pesi uguali equivale a BSF e guardare quanti nodi di distanza hanno i nodi dalla sorgente....
    std::map<T, int> dist;
    for (const T& nodo : G.all_nodes())
        dist[nodo] = std::numeric_limits<int>::max();
    dist[sorgente] = 0;

    // set di coppie (distanza, nodo) ordinato automaticamente per distanza
    std::set<std::pair<int, T>> da_visitare;
    da_visitare.insert({0, sorgente});

    while (!da_visitare.empty())
    {
        // il primo elemento è sempre quello con distanza minore
        auto [d, u] = *da_visitare.begin();
        da_visitare.erase(da_visitare.begin());

        for (const T& vicino : G.neighbors(u))
        {
            int nuova_dist = d + 1;

            if (nuova_dist < dist[vicino])
            {
                // rimuovi il vecchio valore e inserisci quello aggiornato
                da_visitare.erase({dist[vicino], vicino});
                dist[vicino] = nuova_dist;
                da_visitare.insert({nuova_dist, vicino});
            }
        }
    }

    return dist;
}




/// ALGORITMI SU CICLI

// CICLO BASATO SU DFS
/*
Si calcola l’albero DFS T = dfs(G) ed il coalbero C = G \ T . Ciascun lato del coalbero, se
reinserito nell’albero DFS, richiude un ciclo del grafo originale. Si procede dunque come segue:
1. Per ogni lato (u, v) ∈ C si calcola il percorso tra u e v in T . Il percorso deve necessariamente
esistere.
2. Durante la visita di T si memorizzano tutti i nodi attraversati a partire da u, una volta
raggiunto v si aggiunge il percorso alla lista dei cicli trovati.
3. Si itera fino ad esaurire gli archi di C.
L’algoritmo di ricerca del percorso tra u e v può essere qualcosa simile all’Algoritmo 1
Algorithm 1: Finding a path between u and v
Input: DFS tree T ; Nodes u, v
Output: Path between u and v in T
findpath (T, u, v)
visited[u] = true;
path.push(u);
if u == v then
return true
for n ∈ neighbours(u) do
if not visited[n] then
if findpath(T, n, v) then
return true
path.pop();
return false
*/
template<typename T>
bool find_path(const unidirected_graph<T> G, std::set<T>& visitati, std::vector<T>& percorso, T sorgente, T arrivo) 
{
    visitati.insert(sorgente);
    percorso.push_back(sorgente);

    if (sorgente == arrivo) 
        return true;

    for (const T& vicino : G.neighbors(sorgente))
    {
        if (visitati.find(vicino) == visitati.end())
        {
            if (find_path(G, visitati, percorso,  vicino, arrivo))
                return true;
        }
    }
    
    percorso.pop_back();
    return false;
}


// verificare se va bene vector o se serve usare grafi
template<typename T>
std::set<std::vector<T>> cicli_dfs (const unidirected_graph<T> G, T sorgente) 
{
    std::set<std::vector<T>> cicli;
    unidirected_graph<T> DFS = recursive_dfs(G, sorgente);
    unidirected_graph<T> C = G - DFS;

    for (const unidirected_edge<T>& arco : C.all_edges())
    {
        std::set<T> visitati;
        std::vector<T> percorso;

        if (find_path(DFS, visitati, percorso, arco.from(), arco.to()))
            cicli.insert(percorso);
    }

    return cicli;
}

