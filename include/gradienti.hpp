#include <Eigen/Dense>
#include <Eigen/SVD>

double condA(const Eigen::MatrixXd& A)
{
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
  Eigen::VectorXd singularValuesA = svd.singularValues();
  return singularValuesA.maxCoeff() / singularValuesA.minCoeff();
}

Eigen::VectorXd gradiente_coniugato(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, const Eigen::VectorXd& x0, double res_tol, unsigned int it_max, unsigned int& it)
{
  Eigen::VectorXd x = x0;
  Eigen::VectorXd r = b - A * x;
  Eigen::VectorXd p = r;
  double res_norm_0 = r.norm();

  it = 0;
  while (it < it_max && r.norm() > res_tol * res_norm_0)
  {
    Eigen::VectorXd Ap = A * p;
    double pAp = p.dot(Ap);

    double alpha_k = p.dot(r) / pAp;
    x = x + alpha_k * p;

    Eigen::VectorXd r_new = b - A * x;
    double beta_k = p.dot(A * r_new) / pAp;
    p = r_new - beta_k * p;

    r = r_new;
    it++;
  }
  return x;
}