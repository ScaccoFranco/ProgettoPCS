#include <iostream>
#include <cassert>
#include <cmath>
#include <Eigen/Dense>
#include "../include/eigen_support.hpp"

void test_gradiente_coniugato() {
    // Matrice A simmetrica e definita positiva 2x2
    Eigen::MatrixXd A(2, 2);
    A << 4, 1,
         1, 3;
         
    Eigen::VectorXd b(2);
    b << 1, 2;

    Eigen::VectorXd x = gradiente_coniugato(A, b);

    // Soluzione analitica attesa del sistema Ax = b: 
    // x_0 = 1/11, x_1 = 7/11
    double expected_x0 = 1.0 / 11.0;
    double expected_x1 = 7.0 / 11.0;

    // Verifica con tolleranza numerica
    assert(std::abs(x(0) - expected_x0) < 1e-9);
    assert(std::abs(x(1) - expected_x1) < 1e-9);
}

void test_condizionamento() {
    Eigen::MatrixXd A(2, 2);
    A << 2, 0,
         0, 2; // Matrice identità scalata, autovalori uguali

    double cond = condA(A);
    
    // Il numero di condizionamento di questa matrice deve essere 1
    assert(std::abs(cond - 1.0) < 1e-9);
}

int main() {
    test_gradiente_coniugato();
    test_condizionamento();
    
    std::cout << "test_matrici: TUTTI I TEST SUPERATI\n";
    return 0;
}