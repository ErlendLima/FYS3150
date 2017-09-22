#include <armadillo>
#include "schrodinger.h"
#include "given_methods.h"

arma::mat hamiltonianMat(double rho_min, double rho_max, unsigned int N){
  double h = (rho_max - rho_min)/N;
  arma::vec V = arma::zeros<arma::vec>(N+1);
  for(unsigned int i = 0; i <= N; i++){
    V(i) = pow(rho_min + i*h, 2);
  }
  double offdiag = -1.0/(h*h);
  arma::mat H = arma::mat(N+1, N+1, arma::fill::zeros);
  for (unsigned int row = 0; row < N+1; row++){
    if (row > 0)
      H(row, row-1) = offdiag;
    H(row,row) = 2/(h*h) + V(row);
    if (row < N)
      H(row, row+1) = offdiag;
  }
  return H;
}

void solve(double rho_min, double rho_max, unsigned int N){
  arma::mat H = hamiltonianMat(rho_min, rho_max, N);
  jacobi(H);
  H.diag().print();
}
