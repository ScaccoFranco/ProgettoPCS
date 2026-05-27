#pragma once

#include <ostream>

// ============================================================
//  Arco non orientato (templated sul tipo del nodo)
//  I due nodi vengono SEMPRE ordinati: from_ minore, to_ maggiore.
//  Cosi' (1,2) e (2,1) rappresentano lo stesso arco.
// ============================================================
template<typename T>
class unidirected_edge
{

private:
    T from_;
    T to_;

public:
    // ordina i nodi all'inserimento
    unidirected_edge(T t1, T t2) {
        if (t1 < t2) {
            from_ = t1;
            to_ = t2;
        } else {
            from_ = t2;
            to_ = t1;
        }
    }

    T from() const {   // nodo minore
        return from_;
    }

    T to() const {     // nodo maggiore
        return to_;
    }

    // ordinamento lessicografico: serve per usare l'arco come chiave di una std::map/std::set
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

// stampa nel formato (from,to). E' un template, quindi non serve inline.
template<typename T>
std::ostream& operator<<(std::ostream& os, const unidirected_edge<T>& other)
{
    os << '(' << other.from() << ',' << other.to() << ')';
    return os;
}
