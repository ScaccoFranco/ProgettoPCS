#pragma once

#include <iostream>
#include <stdexcept>
#include <optional>
#include "algoritmi_grafi.hpp" 

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
std::vector<std::vector<T>> cicli_dfs (const unidirected_graph<T> G, T sorgente) 
{
    std::vector<std::vector<T>> cicli;
    unidirected_graph<T> DFS = recursive_dfs(G, sorgente);
    unidirected_graph<T> C = G - DFS;

    for (const unidirected_edge<T>& arco : C.all_edges())
    {
        std::set<T> visitati;
        std::vector<T> percorso;

        if (find_path(DFS, visitati, percorso, arco.from(), arco.to()))
            cicli.push_back(percorso);
    }

    return cicli;
}




// DE PINA!!!!

std::optional<bool> prodotto_scalare(const std::vector<bool> &c, const std::vector<bool> &s)
{
    if (c.size() != s.size())
        return std::nullopt;

    // ottimizzo usando solo un booleano con xor invece che un intero che somma tutti (uso meno byte)
    bool res = false;
    for (int i = 0; i < s.size(); i++) {
        res ^= c[i] && s[i];
    }

    return res;
}

std::optional<std::vector<bool>> differenza_simmetrica(const std::vector<bool> &c, const std::vector<bool> &s)
{
    if (c.size() != s.size())
        return std::nullopt;

    std::vector<bool> res(s.size(), false);
    for (int i = 0; i < s.size(); i++) {
        res[i] = c[i] ^ s[i];
    }

    return res;
}

template<typename T>
class LiftingNode {

private:
    T value_;
    bool sign_;

public: 
    LiftingNode() : value_(T{}), sign_(false) {}

    LiftingNode(const T& value, const bool& sign) : value_(value), sign_(sign) {}

    T value() {
        return value_;
    }

    bool sign() {
        return sign_;
    }

    bool operator<(const LiftingNode& other) const {
        if (value_ != other.value_) return value_ < other.value_;
            return sign_ < other.sign_;  // false < true
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
        if (el)
            sum += 1;
    }

    return sum;
}

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
        for (int i = 0; i < path.size()-1; i++)
        {
            T u = path[i].value();
            T v = path[i+1].value();
            int id = G.edge_number(unidirected_edge<T>(u, v));

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

template<typename T>
std::vector<std::vector<bool>> incidenza_de_pina (const unidirected_graph<T> G, T sorgente) 
{
    // parte 1: preparazione
    unidirected_graph<T> DFS = recursive_dfs(G, sorgente);
    unidirected_graph<T> C = G - DFS;

    int m = G.all_edges().size();
    int n = G.all_nodes().size();
    int k = m - n + 1;

    if (k != C.all_edges().size())
    {
        throw std::runtime_error("Errore: il grafo C non ha dimensione k, come dovrebbe");
    }

    std::vector<std::vector<bool>> S(k, std::vector<bool>(m, false));
    std::vector<std::vector<bool>> Cicli(k, std::vector<bool>(m, false));
    
    int iter = 0;
    for (const unidirected_edge<T> edge : C.all_edges())
    {
        S[iter][G.edge_number(edge)] = true;
        iter++;
    }

    // parte 2: ciclo principale
    for (int i = 0; i < k; i++)
    {
        Cicli[i] = ciclo_minimo(m, G, S[i]);
        for (int j = i+1; j < k; j++)
        {
            if (auto res = prodotto_scalare(Cicli[i], S[j]))
            {
                if (*res) {
                    if (auto diff = differenza_simmetrica(S[j], S[i]))
                    {
                        S[j] = *diff;
                    } 
                    else 
                    {
                        std::cerr << "Vettori con lunghezze diverse nella differenza simmetrica!";
                    }
                }
            }
            else 
            {
                std::cerr << "Prodotto scalare tra vettori di lunghezza diversa";
            }
        }
    }

    return Cicli;
}

template<typename T>
std::vector<T> incidenza_to_nodi(const std::vector<bool>& C, const unidirected_graph<T>& G)
{
    // costruisce mappa di adiacenza solo con gli archi del ciclo
    std::map<T, std::vector<T>> adj;
    int m = C.size();
    for (int i = 0; i < m; i++)
    {
        if (C[i]) {
            auto e = G.edge_at(i);
            adj[e.from()].push_back(e.to());
            adj[e.to()].push_back(e.from());
        }
    }

    if (adj.empty()) return {};

    // segui il percorso partendo dal primo nodo
    std::vector<T> percorso;
    T start = adj.begin()->first;
    T current = start;
    T prev = start; // sentinella per non tornare indietro

    do {
        percorso.push_back(current);
        T next = prev;
        for (const T& vicino : adj[current]) {
            if (vicino != prev) { 
                next = vicino; 
                break; 
            }
        }
        prev = current;
        current = next;
    } while (current != start);

    return percorso;
}


template<typename T>
std::vector<std::vector<T>> de_pina(const unidirected_graph<T>& G)
{
    T sorgente = *G.all_nodes().begin(); 
    auto incidenze = incidenza_de_pina(G, sorgente);

    std::vector<std::vector<T>> result;
    for (const auto& C : incidenze)
        result.push_back(incidenza_to_nodi(C, G));

    return result;
}



// per testare (dfs)

std::vector<std::vector<int>> find_cycles(const circuit_graph& cg) {

    unidirected_graph support_tree = recursive_dfs(cg.get_graph(), 1);
    unidirected_graph co_tree = cg.get_graph() - support_tree;

    std::vector<std::vector<int>> all_cycles;

    for (const unidirected_edge& e : co_tree.all_edges()) {
        dijkstraResult dij_result = dijkstra(support_tree, e.from());
        std::vector<int> path = get_path(dij_result,e.from(),e.to());
        path.push_back(e.from());
        all_cycles.push_back(path);
    }

    return all_cycles;
}