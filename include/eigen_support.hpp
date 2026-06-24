#pragma once

#include <Eigen/Dense>
#include <Eigen/SVD>
#include <vector>
#include <iostream>

// Fa da ponte tra le strutture dati di C++ e la libreria Eigen.
// Fornisce le funzioni di conversione tra std::vector e matrici/vettori Eigen.
// Il fulcro è l'implementazione del Gradiente Coniugato. 
// Poiché la matrice del sistema A = B^T * R * B è simmetrica (e semi-definita positiva)
// il Gradiente Coniugato permette di trovare le correnti in modo molto più efficiente
// rispetto a un'inversione della matrice.

Eigen::MatrixXd to_matrix(const std::vector<std::vector<double>>& mat) {
    Eigen::MatrixXd result(mat.size(), mat[0].size());
    for (int i = 0; i < (int)mat.size(); i++)
        for (int j = 0; j < (int)mat[0].size(); j++)
            result(i, j) = mat[i][j];
    return result;
}

Eigen::VectorXd to_vector(const std::vector<double>& vec) {
    Eigen::VectorXd result(vec.size());
    for (int i = 0; i < (int)vec.size(); i++)
        result(i) = vec[i];
    return result;
}

std::vector<double> from_vector(const Eigen::VectorXd& vec) {
    std::vector<double> result(vec.size());
    for (int i = 0; i < (int)vec.size(); i++)
        result[i] = vec(i);
    return result;
}

double condA(const Eigen::MatrixXd& A)
{
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
    Eigen::VectorXd s = svd.singularValues();
    return s.maxCoeff() / s.minCoeff();
}

Eigen::VectorXd gradiente_coniugato(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, double tol = 1.0e-12, unsigned int max_iter = 10000)
{
    const int n = b.size();

    if (A.rows() != n || A.cols() != n) {
        std::cerr << "A e b incompatibili (dimensioni)";
    }

    Eigen::VectorXd x = Eigen::VectorXd::Zero(n);
    Eigen::VectorXd r = b - A * x;
    Eigen::VectorXd p = r;

    const double r_norm_0 = r.norm();
    if (r_norm_0 == 0.0)
        return x;

    unsigned int it = 0;
    while (it < max_iter && r.norm() > tol * r_norm_0)
    {
        const Eigen::VectorXd Ap = A * p;
        const double pTAp = p.dot(Ap);

        const double alpha_k = p.dot(r) / pTAp;

        x = x + alpha_k * p;
        r = r - alpha_k * Ap;

        const double beta_k = p.dot(A * r) / pTAp;
        p = r - beta_k * p;

        ++it;
    }

    return x;
}

std::vector<double> solve_system(const std::vector<std::vector<double>>& B, const std::vector<std::vector<double>>& R, const std::vector<double>& v)
{
    Eigen::MatrixXd B_eigen = to_matrix(B);
    Eigen::MatrixXd R_eigen = to_matrix(R);
    Eigen::VectorXd v_eigen = to_vector(v);

    Eigen::MatrixXd A = B_eigen.transpose() * R_eigen * B_eigen;

    Eigen::VectorXd x = gradiente_coniugato(A, v_eigen);

    return from_vector(x);
}
