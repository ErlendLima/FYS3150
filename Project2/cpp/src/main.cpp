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

  jacobi(A, n);
  A.print();
  return 0;
}
