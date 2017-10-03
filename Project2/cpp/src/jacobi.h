#ifndef JACOBI_H
#define JACOBI_H

#include <armadillo>
#include <ctime>

void jacobi( arma::vec& eigval, arma::mat& eigvec, arma::mat& A);
void jacobirotate(arma::mat& A, arma::mat& R, unsigned int& k, unsigned int& l, unsigned int& n);
void find_max_offdiag(arma::mat& A, unsigned int& k, unsigned int& l, unsigned int& n);

#endif /* JACOBI_H */
