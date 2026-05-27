#include <iostream>
#include "./include/circuit.hpp"
#include "./include/input.hpp"
#include "./include/cicli.hpp"
#include "./include/eigen_support.hpp"

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
            const unidirected_edge<int>& e = pair.first;
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
        auto correnti = solve_system(B, R, v);
        std::cout << "\nCorrenti di maglia:\n";
        for (int i = 0; i < correnti.size(); i++) {
            std::cout << "I" << i+1 << " = " << correnti[i] << "\n";
        }
	}
	else {
		
		std::cerr << "Errore: impossibile aprire: " << argv[1] << "\n";
		return 1;
	}

}