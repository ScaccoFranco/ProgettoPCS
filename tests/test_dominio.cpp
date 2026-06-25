#include <iostream>
#include <vector>
#include <cassert>
#include "../include/elements.hpp"
#include "../include/circuit.hpp"

void test_segni_resistenze() {
    component res('R', 1, 10.0, 1, 2); // Nodo min 1, max 2
    
    std::vector<int> ciclo_concorde = {1, 2, 3, 1}; // Attraversa 1 -> 2
    std::vector<int> ciclo_discorde = {3, 2, 1, 3}; // Attraversa 2 -> 1
    std::vector<int> ciclo_assente = {4, 5, 6, 4};  // Non tocca la resistenza

    assert(plus_minus(res, ciclo_concorde) == 1.0);
    assert(plus_minus(res, ciclo_discorde) == -1.0);
    assert(plus_minus(res, ciclo_assente) == 0.0);
}

void test_segni_generatori() {
    component gen('V', 1, 10.0, 1, 2); // Positivo: 1, Negativo: 2 (flusso da 2 a 1 è positivo)
    
    std::vector<int> ciclo_positivo = {3, 2, 1, 3}; // Entra nel -, esce nel + (contributo +)
    std::vector<int> ciclo_negativo = {3, 1, 2, 3}; // Entra nel +, esce nel - (contributo -)

    assert(gen_sign(gen, ciclo_positivo) == 1);
    assert(gen_sign(gen, ciclo_negativo) == -1);
}

int main() {
    test_segni_resistenze();
    test_segni_generatori();
    std::cout << "test_dominio: TUTTI I TEST SUPERATI\n";
    return 0;
}