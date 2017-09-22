#ifndef SCHRODINGER_H
#define SCHRODINGER_H

#include <armadillo>

arma::mat hamiltonianMat(double rho_min, double rho_max, unsigned int N);
void solve(double rho_min, double rho_max, unsigned int N);

#endif /* SCHRODINGER_H */
