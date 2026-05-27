#pragma once

#include <vector>
#include "unidirected_graph.hpp"

// ============================================================
//  component: un componente del circuito (resistore o generatore).
//  Per i generatori positive_node/negative_node danno la polarita';
//  per i resistori sono solo i due punti di collegamento.
// ============================================================
struct component
{

    char c_type;        // 'R' resistore, 'V' generatore
    int c_number;       // numero progressivo (R1, V2, ...)
    double c_value;     // Ohm se resistore, Volt se generatore
    int positive_node;  // primo nodo della netlist (per V: terminale +)
    int negative_node;  // secondo nodo della netlist (per V: terminale -)

    component() : c_type('\0'), c_number(0), c_value(0.0), positive_node(0), negative_node(0) {}

    component(const char& type, const int& number, const double& value, const int& node, const int& nodebis)
        : c_type(type), c_number(number), c_value(value), positive_node(node), negative_node(nodebis) {}

};

// ============================================================
//  circuit_graph: modello del circuito. Per composizione usa un
//  unidirected_graph per la topologia e vi associa i componenti.
// ============================================================
template<typename T>
class circuit_graph
{
    private:

        unidirected_graph<T> graph;
        std::map<unidirected_edge<T>, component> edge_to_component;
        std::vector<component> resistors;
        std::vector<component> generators;

    public:

        circuit_graph() {}

        void add_component (component c, int n1, int n2) {

            graph.add_edge(n1,n2);

            unidirected_edge<T> e(n1,n2);
            edge_to_component[e] = c;

            if (c.c_type == 'R') {
                resistors.push_back(c);
            }
            else if (c.c_type == 'V') {
                generators.push_back(c);
            }
        }

        const unidirected_graph<T>& get_graph() const {
            return graph;
        }

        const std::map<unidirected_edge<T>, component>& get_edge_map() const {
            return edge_to_component;
        }

        const std::vector<component>& get_allresistor() const {
            return resistors;
        }

        const std::vector<component>& get_allgenerator() const {
            return generators;
        }
};
