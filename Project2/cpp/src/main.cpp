#include <iostream>
#include <cmath>
#include <memory.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <armadillo>
#include "given_methods.h"

int main(int argc, char const *argv[]) {
  unsigned int n = 5;
  double maxiter = n*n*n;
  double epsilon = 1.0e-8;
  unsigned int iter = 0;
  unsigned int k, l;
  double max_offdiag = 1000.0;

  arma::mat A = arma::zeros<arma::mat>(n, n);
  for(unsigned int i = 0; i < n; i++){
    for(unsigned int j = 0; j < n; j++){
      if(i == j){
        A(i,j) = 2.0;
      }
      else{
        A(i,j) = 1.0;
      }
    }
  }

  arma::mat R = arma::eye<arma::mat>(n, n); // Eigenvalue matrix

  while(fabs(max_offdiag) > epsilon && static_cast<double>(iter) < maxiter){
    // Find maximum off diagonal element
    find_max_offdiag(A, k, l, n);
    max_offdiag = A(l,k);
    jacobirotate(A, R, k, l, n);

    iter++;
  }
  A.print();
  return 0;
}
