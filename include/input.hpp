#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include "circuit.hpp"

circuit_graph<int> build_graph(std::ifstream& input_file)
{
    circuit_graph<int> g;

    std::string line;
    
    while (std::getline(input_file, line)) {
        
        std::istringstream ss(line);
        char type;
        int number;
        double value;
        int n1, n2;
        if (!(ss >> type >> number >> value >> n1 >> n2)) continue;

        component c(type, number, value, n1, n2);

        g.add_component(c,n1,n2);
    }

    return g;
}
