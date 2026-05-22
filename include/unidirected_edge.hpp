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


template<typename T>
class unidirected_edge 
{

private:
    T from_;
    T to_;


public:
    unidirected_edge(T t1, T t2) {
        if (t1 < t2) {
            from_ = t1;
            to_ = t2;
        } else {
            from_ = t2;
            to_ = t1;
        }
    }

    // OUTPUT delle varibaili private
    T from() const {
        return from_;
    }

    T to() const {
        return to_;
    }

    bool operator<(const unidirected_edge& other) const {
        if (from_ != other.from_) return from_ < other.from_;
        return to_ < other.to_;
    }

    bool operator==(const unidirected_edge& other) const {
        if (from_ == other.from() && to_ == other.to()) {
            return true;
        } else {
            return false;
        }
    }
};



template<typename T>
std::ostream& operator<<(std::ostream& os, const unidirected_edge<T>& other) 
{
    os << '(' << other.from() << ',' << other.to() << ')';

    return os;
}
