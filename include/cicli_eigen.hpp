#pragma once

#include <Eigen/Dense>
#include <iostream>
#include <stdexcept>
#include <optional>
#include <limits>
#include "algoritmi_grafi.hpp"
#include "circuit_eigen.hpp"

// ============================================================
//  METODO 1 - DFS + coalbero
//  Albero di supporto via DFS, coalbero C = G - T, e per ogni arco
//  del coalbero si chiude il ciclo con il cammino nell'albero.
//  Restituisce cicli CHIUSI: [n0,...,nk,n0].
// ============================================================
std::vector<std::vector<int>> find_cycles(const circuit_graph<int>& cg) {

    unidirected_graph<int> support_tree = recursive_dfs(cg.get_graph(), 1);
    unidirected_graph<int> co_tree = cg.get_graph() - support_tree;

    std::vector<std::vector<int>> all_cycles;

    for (const unidirected_edge<int>& e : co_tree.all_edges()) {
        DijkstraResult<int> dij_result = dijkstra(support_tree, e.from());
        std::vector<int> path = get_path_vec(dij_result, e.from(), e.to());
        path.push_back(e.from());   // chiude il ciclo
        all_cycles.push_back(path);
    }

    return all_cycles;
}


// ============================================================
//  METODO 2 - Algoritmo di De Pina (cicli minimi)
// ============================================================


// nodo "liftato": copia del nodo con un bit di segno, usato nel grafo di lifting
template<typename T>
class LiftingNode {

private:
    T value_;
    bool sign_;

public:
    LiftingNode() : value_(T{}), sign_(false) {}
    LiftingNode(const T& value, const bool& sign) : value_(value), sign_(sign) {}

    T value() const { return value_; }
    bool sign() const { return sign_; }

    bool operator<(const LiftingNode& other) const {
        if (value_ != other.value_) return value_ < other.value_;
        return sign_ < other.sign_;
    }
    bool operator<=(const LiftingNode& other) const {
        return value_ <= other.value_;
    }
    bool operator==(const LiftingNode& other) const {
        return value_ == other.value_ && sign_ == other.sign_;
    }
};

// costruisce il grafo di lifting rispetto al vettore di supporto S_i
template<typename T>
unidirected_graph<LiftingNode<T>> lifting(const unidirected_graph<T> &G, const std::vector<bool> &S_i)
{
    unidirected_graph<LiftingNode<T>> Gprimo;

    for (const unidirected_edge<T> edge : G.all_edges()) {
        LiftingNode<T> u_plus  = {edge.from(), false};
        LiftingNode<T> u_minus = {edge.from(), true};
        LiftingNode<T> v_plus  = {edge.to(),   false};
        LiftingNode<T> v_minus = {edge.to(),   true};

        if (S_i[G.edge_number(edge)]) {
            Gprimo.add_edge(u_plus, v_minus);
            Gprimo.add_edge(u_minus, v_plus);
        } else {
            Gprimo.add_edge(u_minus, v_minus);
            Gprimo.add_edge(u_plus, v_plus);
        }
    }
    return Gprimo;
}

// peso di un vettore booleano (numero di archi del ciclo)
int val_bool_vec(const std::vector<bool> &Inc_i)
{
    int sum = 0;
    for (bool el : Inc_i) {
        if (el) sum += 1;
    }
    return sum;
}

// trova il ciclo minimo ortogonale a S_i tramite i cammini minimi nel lifting
template<typename T>
std::vector<bool> ciclo_minimo(const int m, const unidirected_graph<T> &G, const std::vector<bool> &S_i)
{
    unidirected_graph<LiftingNode<T>> Gprimo = lifting(G, S_i);
    std::vector<bool> ciclo_minimo(m, false);
    int val_min = std::numeric_limits<int>::max();

    for (const T node : G.all_nodes()) {
        LiftingNode<T> u_plus  = {node, false};
        LiftingNode<T> u_minus = {node, true};

        DijkstraResult<LiftingNode<T>> res  = dijkstra(Gprimo, u_minus);
        std::vector<LiftingNode<T>> path = get_path_vec(res, u_minus, u_plus);

        if (path.empty()) continue;

        std::vector<bool> ciclo_i(m, false);
        for (int i = 0; i < (int)path.size()-1; i++)
        {
            T u = path[i].value();
            T v = path[i+1].value();

            int id = G.edge_number(unidirected_edge<T>(u, v));
            if (id < 0 || id >= m)
            {
                std::cerr << "edge_number fuori range: " << id << " per arco (" << u << "," << v << ")\n";
                continue;
            }
            ciclo_i[id] = !ciclo_i[id]; // modulo 2
        }

        int val_i = val_bool_vec(ciclo_i);
        if (val_i > 0 && val_i < val_min) {
            val_min = val_i;
            ciclo_minimo = ciclo_i;
        }
    }

    return ciclo_minimo;
}

// nucleo di De Pina: restituisce i cicli come vettori di incidenza sugli archi
template<typename T>
Eigen::MatrixXi incidenza_de_pina (const unidirected_graph<T> G, T sorgente)
{
    unidirected_graph<T> DFS = recursive_dfs(G, sorgente);
    unidirected_graph<T> C = G - DFS;

    int m = G.all_edges().size();
    int n = G.all_nodes().size();
    int k = m - n + 1;

    if (k != C.all_edges().size())
    {
        throw std::runtime_error("Errore: il grafo C non ha dimensione k, come dovrebbe");
    }

    Eigen::MatrixXi S = Eigen::MatrixXi::Zero(k, m);
    Eigen::MatrixXi Cicli = Eigen::MatrixXi::Zero(k, m);


    // qua  O(k^2) perché nel ciclo di lunghezza k cicla (edge_number) su tutti i nodi (al più k?)
    // verificare se si può migliorare
    int iter = 0;
    for (const unidirected_edge<T> edge : C.all_edges())
    {
        int id = G.edge_number(edge);
        if (id < 0 || id >= m)
        {
            std::cerr << "edge_number fuori range\n";
            continue;
        }

        S(iter, id) = 1;
        iter++;
    }

    for (int i = 0; i < k; i++)
    {
        // controllare se si può modificare qua ed evitare conversione in vec<bool>
        std::vector<bool> s_i(m);
        for (int j = 0; j < m; j++) s_i[j] = S(i, j) != 0;

        std::vector<bool> c_i = ciclo_minimo(m, G, s_i);
        for (int j = 0; j < m; j++) Cicli(i, j) = c_i[j] ? 1 : 0;
        for (int j = i+1; j < k; j++)
        {
            // Riduco a questa riga il prodotto scalare definito nel file
            // ovvero: modulo 2 del prodotto scalare dei due vettori booleani (interi considerati booleani in questo caso)
            bool res = (Cicli.row(i).array() * S.row(j).array()).sum() % 2 != 0;
            if (res) {
                // compattato anche la funzione della differenza simmetrica, ovvero lo xor dei due vettori
                S.row(j) = (S.row(j).cwiseNotEqual(Cicli.row(i))).cast<int>();
            }
            // CHIEDERE AL PROF SE é MEGLIO MANTENERE I CONTROLLI CHE C'ERANO PRIMA
        }
    }

    return Cicli;
}

// converte un vettore di incidenza sugli archi in sequenza di nodi (ciclo aperto)
template<typename T>
std::vector<T> incidenza_to_nodi(const Eigen::RowVectorXi& C, const unidirected_graph<T>& G)
{
    std::map<T, std::vector<T>> adj;
    int m = C.size();
    for (int i = 0; i < m; i++)
    {
        if (C(i)) {
            auto e = G.edge_at(i);
            adj[e.from()].push_back(e.to());
            adj[e.to()].push_back(e.from());
        }
    }

    if (adj.empty()) return {};

    std::vector<T> percorso;
    T start = adj.begin()->first;
    T current = start;
    T prev = start;

    do {
        percorso.push_back(current);
        T next = prev;
        for (const T& vicino : adj[current]) {
            if (vicino != prev) { next = vicino; break; }
        }
        prev = current;
        current = next;
    } while (current != start);

    return percorso;
}

// De Pina completo. Restituisce cicli CHIUSI [n0,...,nk,n0],
template<typename T>
std::vector<std::vector<T>> de_pina(const unidirected_graph<T>& G)
{
    T sorgente = *G.all_nodes().begin();
    Eigen::MatrixXi incidenze = incidenza_de_pina(G, sorgente);

    std::vector<std::vector<T>> result;
    for (int i = 0; i < incidenze.rows(); i++)
    {
        std::vector<T> nodi = incidenza_to_nodi(incidenze.row(i), G);
        if (!nodi.empty())
            nodi.push_back(nodi.front());
        result.push_back(nodi);
    }
    return result;
}