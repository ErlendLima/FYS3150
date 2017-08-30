#include <armadillo>
#include "solve.h"

arma::vec thomas(const arma::vec& a, const arma::vec& b, const arma::vec& c, const arma::vec& f){
    /* Implementation of Thomas Algorithm as described p. 186*/
    if(arma::numel(a) != arma::numel(b) ||
       arma::numel(b) != arma::numel(c) ||
       arma::numel(c) != arma::numel(f))
        throw std::runtime_error("Input vectors must be of equal length");

    const size_t n   = arma::numel(a);
    arma::vec tmp    = arma::zeros(n);
    arma::vec u      = arma::zeros(n);

    // Forward sweep
    double btmp = b[1];
    u[1]        = f[1]/btmp;
    for(unsigned int i = 2; i <= n-2; i++){
        tmp[i] = c[i-1]/btmp;
        btmp   = b[i]-a[i]*tmp[i];
        u[i]   = (f[i]-a[i]*u[i-1])/btmp;
    }

    // Backward substitution
    for(unsigned int i = n-3; i > 0; i--)
        u[i] -= tmp[i+1]*u[i+1];

    return u;
}


arma::mat tridiagonalMat(unsigned int size, double upper, double middle, double lower){
    auto mat = arma::mat(size, size, arma::fill::zeros);
    for (unsigned int row = 1; row < size-1; row++){
        if (row > 0)
            mat(row, row-1) = lower;
        mat(row, row) = middle;
        if (row < size-1)
            mat(row, row+1) = upper;
    }

    // Ensure the boundary conditions
    mat(0,0) = 1;
    mat(size-1, size-1) = 1;
    return mat;
}


arma::vec thomasSpecial(double upper, double middle, double lower, const arma::vec& f){
  const size_t n   = arma::numel(f);
  arma::vec u      = arma::zeros(n);
  arma::vec a_inv  = arma::zeros(n);
  arma::vec d      = arma::zeros(n);

  for(unsigned int i = 1; i < n+1; i++){
      a_inv[i] = (double)i/(i+1);
      d[i] = f[i] + f[i-1]*a_inv[i];
  }
  u[n] = d[n]*a_inv[n];

  for(unsigned int i = n-1; i > 0; i--){
    u[i] = (d[i] + u[i+1])*a_inv[i];
  }
  return u;
}
