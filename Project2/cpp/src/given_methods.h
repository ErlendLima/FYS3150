#ifndef GIVEN_METHODS_H
#define GIVEN_METHODS_H

#include <armadillo>

void jacobi(arma::mat& A, unsigned int& n);
void jacobirotate(arma::mat& A, arma::mat& R, unsigned int& k, unsigned int& l, unsigned int& n);
void find_max_offdiag(arma::mat& A, unsigned int& k, unsigned int& l, unsigned int& n);

#endif /* GIVEN_METHODS_H */
