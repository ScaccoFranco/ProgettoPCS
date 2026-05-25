#include "circuit.hpp"
#include "lifofifo.hpp"
#include <fstream>
#include <sstream>
#include <string>


circuit_graph build_graph(std::ifstream& input_file)
{
    circuit_graph g;

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

std::vector<std::vector<double>> build_B(const std::vector<std::vector<int>>& cycles, const std::vector<component>& resistors, const circuit_graph& cg) {

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

// l'output da modificare, l'ho fatto fare a claude solo per testare se aveva senso quello che avevo scritto prima

int main(int argc, const char *argv[]) 
{
    if (argc < 2) {
		std::cerr << "Errore: Mancata definizione del file \n";
		return 1;
	}
	
	std::ifstream file(argv[1]);
	
	if (file.is_open()) {

        circuit_graph cg = build_graph(file);
        for (int n : cg.get_graph().all_nodes()) {
             std::cout << n << " ";
        }
        for (const auto& pair : cg.get_edge_map()) {
            const unidirected_edge& e = pair.first;
            const component& c = pair.second;
            std::cout << c.c_type << c.c_number << " | valore: " << c.c_value  << " | nodi: (" << e.from() << ", " << e.to() << ")\n";
        }
         auto cycles = find_cycles(cg);
    
        std::cout << "\nCicli trovati: " << cycles.size() << "\n";
    
        for (int i = 0; i < cycles.size(); i++) {
            std::cout << "Ciclo " << i+1 << ": ";
            for (int node : cycles[i]) {
            std::cout << node << " ";
        }
        std::cout << "\n";
        }

        auto B = build_B(cycles, cg.get_allresistor(), cg);
        std::cout << "\nMatrice B:\n";
        for (int i = 0; i < B.size(); i++) {
            std::cout << "R" << cg.get_allresistor()[i].c_number << ": ";
            for (int j = 0; j < B[i].size(); j++) {
            std::cout << B[i][j] << " ";
        }
        std::cout << "\n";
        }
        auto R = build_R(cg.get_allresistor());
        auto v = build_v(cycles, cg.get_allgenerator());
        std::cout << "\nVettore v:\n";
        for (int i = 0; i < v.size(); i++) {
            std::cout << "Ciclo " << i+1 << ": " << v[i] << "\n";
        }
	}
	else {
		
		std::cerr << "Errore: impossibile aprire: " << argv[1] << "\n";
		return 1;
	}

}