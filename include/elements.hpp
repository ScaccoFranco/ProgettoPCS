#pragma once

#include <vector>
#include "unidirected_graph.hpp"

// La struct "component" racchiude i dati del singolo elemento (tipo, valore, nodi). 

struct component
{

    char c_type;
    int c_number;
    double c_value;
    int positive_node;
    int negative_node;

    component() : c_type('\0'), c_number(0), c_value(0.0), positive_node(0), negative_node(0) {}

    component(const char& type, const int& number, const double& value, const int& node, const int& nodebis)
        : c_type(type), c_number(number), c_value(value), positive_node(node), negative_node(nodebis) {}

};

// La classe "circuit_graph" forma il grafo di base e associa ogni arco 
// al suo componente fisico reale. Inoltre, si separa da subito i resistori dai generatori 
// salvandoli in vettori dedicati, un trucco che semplifica la costruzione 
// delle matrici R, B e del vettore v più avanti nel codice.

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
