#pragma once
/*

Si implementi una classe unidirected edge che rappresenta un arco in un grafo. La classe deve
avere un costruttore che permette di specificare i due nodi connessi dall’arco, inoltre devono esserci
due metodi from() e to() che restituiscono i due nodi. Si implementi anche :
• operator< per unidirected edge, affinch´e gli archi possano essere ordinabili,
• operator== per unidirected edge,
• operator<< per unidirected edge.
Si faccia in modo che unidirected edge garantisca sempre che from `e minore di to.
Si passi quindi all’implementazione di una classe unidirected graph. Essa deve prevedere:
• Un costruttore di default
• Un costruttore di copia
• Un metodo neighours() che, dato un nodo, restituisce i suoi vicini,
1
• Un metodo add edge() che permetta di aggiungere un arco al grafo,
• Un metodo all edges() che restituisce tutti gli archi,
• Un metodo all nodes() che restituisce tutti gli nodi,
• Un metodo edge number() che, dato un arco, ne restituisce la sua numerazione all’interno
del grafo,
• Un metodo edge at() che, dato un numero d’arco, restituisce il corrispondente oggetto arco
all’interno del grafo,
• L’operatore operator-(), che permette di calcolare la differenza tra due grafi: dati G e G′,
la differenza G − G′ `e data dagli archi presenti in G e non presenti in G′.
Si noti che l’operazione add node() non `e stata menzionata. Si propongano e si scrivano opportuni
test per la struttura dati implementata.

*/

#include <ostream>
#include <set>
#include <map>
#include <vector>
#include <stdexcept>


template<typename T>
class unidirected_graph
{

private:
    // Matrice o mappa di adiacenza
    // in pratica è una mappa di insiemi, ogni insieme sono i nodi di adiacenza
    // serve a trovare i vicini
    std::map<T, std::set<T>> adj_;

    // vettore per memorizzare e numerare gli archi, sennò non ci posso accedere
    std::vector<unidirected_edge<T>> edges_;


public:
    unidirected_graph() = default; // inizializza tutto vuoto (set, vector ecc)

    unidirected_graph(const unidirected_graph& other) : adj_(other.adj_), edges_(other.edges_) {}

    std::set<T> neighbors(T node) const {
        if (adj_.count(node)) return adj_.at(node);
        return {};
    }

    void add_edge(T u, T v) {
        unidirected_edge<T> new_edge(u, v);

        // controllo se esiste o meno
        if (adj_[u].find(v) == adj_[u].end()) {
            adj_[u].insert(v);
            adj_[v].insert(u);
            edges_.push_back(new_edge); // aggiungo alla fine del vettore.
        }
    }

    std::set<unidirected_edge<T>> all_edges() const {
        return std::set<unidirected_edge<T>>(edges_.begin(), edges_.end()); // converto vector in set
    }

    std::set<T> all_nodes() const {
        std::set<T> nodes;

        for (auto it = edges_.begin(); it != edges_.end(); it++) {
            nodes.insert(it->from()); 
            nodes.insert(it->to()); // se uguale non aggiungono
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
    
    // differenza tra grafi
    unidirected_graph operator-(const unidirected_graph& other) const {
        unidirected_graph G;
        for (const auto& e : this->edges_) { // for alternativo in forma simil python
            // Se l'arco non è presente nell'altro grafo, lo aggiungiamo a G
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


