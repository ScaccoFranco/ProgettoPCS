#pragma once

#include <ostream>
#include <set>
#include <map>
#include <vector>
#include <stdexcept>
#include "unidirected_edge.hpp"

// Si rappresenta un grafo non orientato generico. Utilizza una lista
// per tracciare i vicini di ogni nodo e un vettore per memorizzare gli archi.
// Si ncludono funzioni per esplorare la struttura (nodi, archi, vicini) e implementa 
// l'operatore di sottrazione tra grafi (operator-), usato nel progetto per 
// isolare i lati del coalbero eliminando l'albero DFS.

template<typename T>
class unidirected_graph
{

private:

    std::map<T, std::set<T>> adj_;

    std::vector<unidirected_edge<T>> edges_;

public:
    unidirected_graph() = default;

    unidirected_graph(const unidirected_graph& other) : adj_(other.adj_), edges_(other.edges_) {}

    std::set<T> neighbors(T node) const {
        if (adj_.count(node)) return adj_.at(node);
        return {};
    }

    void add_edge(T u, T v) {
        unidirected_edge<T> new_edge(u, v);

        if (adj_[u].find(v) == adj_[u].end()) {
            adj_[u].insert(v);
            adj_[v].insert(u);
            edges_.push_back(new_edge);
        }
    }

    std::set<unidirected_edge<T>> all_edges() const {
        return std::set<unidirected_edge<T>>(edges_.begin(), edges_.end());
    }

    std::set<T> all_nodes() const {
        std::set<T> nodes;
        for (auto it = edges_.begin(); it != edges_.end(); it++) {
            nodes.insert(it->from());
            nodes.insert(it->to());
        }
        return nodes;
    }

    int edge_number(unidirected_edge<T> edge) const {
        for (int i = 0; i < edges_.size(); i++) {
            if (edges_[i] == edge) return i;
        }
        return -1;
    }

    unidirected_edge<T> edge_at(int index) const {
        if (index < 0 || index >= edges_.size()) throw std::out_of_range("Indice fuori limite");
        return edges_[index];
    }

    unidirected_graph<T> operator-(const unidirected_graph& other) const {
        unidirected_graph G;
        for (const auto& e : this->edges_) {
            bool trovato = false;
            for (const auto& other_e : other.all_edges()) {
                if (e == other_e) {
                    trovato = true;
                    break;
                }
            }
            if (!trovato) G.add_edge(e.from(), e.to());
        }
        return G;
    }

};
