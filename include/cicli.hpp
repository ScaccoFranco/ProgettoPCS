#pragma once

#include <Eigen/Dense>
#include <iostream>
#include <stdexcept>
#include <optional>
#include <limits>
#include "algoritmi_grafi.hpp"

// Si trovano le maglie del circuito.
// Si implementano le due alternative richieste dal progetto: la ricerca base 
// (costruendo l'albero di supporto DFS e chiudendo i cicli con gli archi del coalbero) 
// e l'algoritmo di De Pina. Quest'ultimo sfrutta la tecnica del "lifting" 
// del grafo e l'algoritmo di Dijkstra per estrarre una base di cicli minimi indipendenti, 
// garantendo che il sistema lineare risultante sia il più compatto e stabile possibile.



// ALGORITMO 1:
template<typename T>
bool findpath(const unidirected_graph<T>& T_albero, T u, T v, std::set<T>& visitati, std::vector<T>& path)
{
    visitati.insert(u);
    path.push_back(u);

    if (u == v)
        return true;

    for (const T& n : T_albero.neighbors(u))
    {
        if (visitati.find(n) == visitati.end())
            if (findpath(T_albero, n, v, visitati, path))
                return true;
    }

    path.pop_back();   // backtracking = u non porta a v
    return false;
}

// Wrapper che inizializza visitati e path
template<typename T>
std::vector<T> findpath(const unidirected_graph<T>& T_albero, T u, T v)
{
    std::set<T> visitati;
    std::vector<T> path;
    findpath(T_albero, u, v, visitati, path);
    return path;
}

template<typename T>
std::vector<std::vector<T>> find_cycles(const unidirected_graph<T>& G) {

    T sorgente = *G.all_nodes().begin();
    unidirected_graph<T> support_tree = recursive_dfs(G, sorgente);
    unidirected_graph<T> co_tree = G - support_tree;

    std::vector<std::vector<T>> all_cycles;

    for (const unidirected_edge<T>& e : co_tree.all_edges()) {
        std::vector<T> path = findpath(support_tree, e.from(), e.to());
        path.push_back(e.from());
        all_cycles.push_back(path);
    }

    return all_cycles;
}

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

int val_bool_vec(const std::vector<bool> &Inc_i)
{
    int sum = 0;
    for (bool el : Inc_i) {
        if (el) sum += 1;
    }
    return sum;
}

template<typename T>
std::vector<bool> ciclo_minimo(const int m, const unidirected_graph<T> &G, const std::vector<bool> &S_i, bool usa_bfs = false)
{
    unidirected_graph<LiftingNode<T>> Gprimo = lifting(G, S_i);
    std::vector<bool> ciclo_minimo(m, false);
    int val_min = std::numeric_limits<int>::max();

    for (const T node : G.all_nodes()) {
        LiftingNode<T> u_plus  = {node, false};
        LiftingNode<T> u_minus = {node, true};

        DijkstraResult<LiftingNode<T>> res  = cammino_minimo(Gprimo, u_minus, usa_bfs); // se vero usa bfs, di standard usa dijkstra!!
        std::vector<LiftingNode<T>> path = get_path(res, u_minus, u_plus);

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
            ciclo_i[id] = !ciclo_i[id];
        }

        int val_i = val_bool_vec(ciclo_i);
        if (val_i > 0 && val_i < val_min) {
            val_min = val_i;
            ciclo_minimo = ciclo_i;
        }
    }

    return ciclo_minimo;
}

// prodotto interno mod 2 tra due vettori di incidenza
int dot_mod2(const Eigen::RowVectorXi& a, const Eigen::RowVectorXi& b) {
    int somma = 0;
    for (int t = 0; t < a.size(); t++)
        somma += a(t) * b(t);
    return somma % 2;
}

// somma mod 2 (XOR) componente per componente
Eigen::RowVectorXi xor_mod2(const Eigen::RowVectorXi& a, const Eigen::RowVectorXi& b) {
    Eigen::RowVectorXi risultato(a.size());
    for (int t = 0; t < a.size(); t++)
        risultato(t) = (a(t) + b(t)) % 2;
    return risultato;
}


// funzione per ottenere i vettori eigen di incidenza di de pina
template<typename T>
Eigen::MatrixXi incidenza_de_pina (const unidirected_graph<T> G, T sorgente, bool usa_bfs = false)
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

        std::vector<bool> s_i(m);
        for (int j = 0; j < m; j++) s_i[j] = S(i, j) != 0;

        std::vector<bool> c_i = ciclo_minimo(m, G, s_i, usa_bfs);
        for (int j = 0; j < m; j++) Cicli(i, j) = c_i[j] ? 1 : 0;
        
        for (int j = i + 1; j < k; j++)
            if (dot_mod2(Cicli.row(i), S.row(j)) != 0)
                S.row(j) = xor_mod2(S.row(j), S.row(i));
    }

    return Cicli;
}

// funzione per passare dai vettori eigen di incidenza ai nodi del ciclo
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


// qua ho quindi come output un vettoredi cicli, ogni ciclo è un vettore di nodi
template<typename T>
std::vector<std::vector<T>> de_pina(const unidirected_graph<T>& G, bool usa_bfs = false)
{
    T sorgente = *G.all_nodes().begin();
    Eigen::MatrixXi incidenze = incidenza_de_pina(G, sorgente, usa_bfs);

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
