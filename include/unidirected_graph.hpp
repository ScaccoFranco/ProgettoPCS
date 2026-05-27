#pragma once

#include <ostream>
#include <set>
#include <map>
#include <vector>
#include <stdexcept>
#include "unidirected_edge.hpp"

// ============================================================
//  Grafo non orientato (templated sul tipo del nodo)
//  Mantiene la lista di adiacenza e un vettore di archi numerati.
// ============================================================
template<typename T>
class unidirected_graph
{

private:
    // lista di adiacenza: per ogni nodo l'insieme dei vicini
    std::map<T, std::set<T>> adj_;

    // vettore degli archi: serve a numerarli (indice = posizione nel vettore)
    std::vector<unidirected_edge<T>> edges_;


public:
    unidirected_graph() = default;

    unidirected_graph(const unidirected_graph& other) : adj_(other.adj_), edges_(other.edges_) {}

    // vicini di un nodo (insieme vuoto se il nodo non esiste)
    std::set<T> neighbors(T node) const {
        if (adj_.count(node)) return adj_.at(node);
        return {};
    }

    // aggiunge un arco solo se non gia' presente
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

    // indice numerico di un arco (-1 se assente). Usato da De Pina.
    int edge_number(unidirected_edge<T> edge) const {
        for (int i = 0; i < edges_.size(); i++) {
            if (edges_[i] == edge) return i;
        }
        return -1;
    }

    // arco a un dato indice
    unidirected_edge<T> edge_at(int index) const {
        if (index < 0 || index >= edges_.size()) throw std::out_of_range("Indice fuori limite");
        return edges_[index];
    }

    // differenza tra grafi: tiene gli archi di *this non presenti in other.
    // Usato per il coalbero C = G - T.
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
