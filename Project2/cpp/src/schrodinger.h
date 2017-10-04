#ifndef SCHRODINGER_H
#define SCHRODINGER_H

#include <armadillo>

enum class Method{JACOBI, ARMA};
arma::mat hamiltonianMat(double rho_min, double rho_max, unsigned int N);
arma::mat hamiltonianMat_repulsion(double rho_min, double rho_max, unsigned int N, double omega_r);
double solve(double rho_min, double rho_max, unsigned int N, Method& method);

#endif /* SCHRODINGER_H */
