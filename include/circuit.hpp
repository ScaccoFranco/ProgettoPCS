#pragma once

#include <vector>
#include <string>
#include "unidirected_graph.hpp"

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


double plus_minus(const component& resistor, const std::vector<int>& cycle) {
    
    int start = std::min(resistor.positive_node, resistor.negative_node);
    int end = std::max(resistor.positive_node, resistor.negative_node);

    for (int i = 0; i < cycle.size(); i++) {
        if (cycle[i] == start && cycle[i+1] == end) {
            return 1.0;
        }
        else if (cycle[i] == end && cycle[i+1] == start) {
            return -1.0;
        }
    }
    return 0;
}

template<typename T>
std::vector<std::vector<double>> build_B(const std::vector<std::vector<int>>& cycles, const std::vector<component>& resistors, const circuit_graph<T>& cg) {

    std::vector<std::vector<double>> B(cg.get_allresistor().size(), std::vector<double>(cycles.size(), 0.0));

    for (int i = 0; i < resistors.size(); i++) {
        for (int j = 0; j < cycles.size(); j++) {
            B[i][j] = plus_minus(resistors[i],cycles[j]);
        }
    }
    
    return B;
}

std::vector<std::vector<double>> build_R(const std::vector<component>& resistors) {

    std::vector<std::vector<double>> R(resistors.size(), std::vector<double>(resistors.size(), 0.0));

    for (int i = 0; i < resistors.size(); i++) {
        R[i][i] = resistors[i].c_value;
    }

    return R;
}

std::vector<double> build_v(const std::vector<std::vector<int>>& cycles, const std::vector<component>& generators) {
    
    std::vector<double> v(cycles.size(), 0.0);

    for (int i = 0; i < cycles.size(); i++) {       
    for (int j = 0; j < generators.size(); j++){                
       
        int sign = plus_minus(generators[j], cycles[i]);
        v[i] = v[i] + generators[j].c_value*(-1*(sign));

    }
}

    return v;

}