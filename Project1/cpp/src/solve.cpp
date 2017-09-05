#include <armadillo>
#include "solve.h"

arma::vec thomas(const arma::vec& a, const arma::vec& b, const arma::vec& c, const arma::vec& v){
    /* Implementation of Thomas Algorithm as described p. 186*/
    if(arma::numel(a) != arma::numel(b) ||
       arma::numel(b) != arma::numel(c) ||
       arma::numel(c) != arma::numel(v))
        throw std::runtime_error("Input vectors must be of equal length");

    const size_t n   = arma::numel(a);

    arma::vec b_prime = arma::zeros(n);
    arma::vec v_prime = arma::zeros(n);
    arma::vec u       = arma::zeros(n);

    b_prime[0] = b[0];
    v_prime[0] = v[0];

    for(unsigned int i = 1; i <= n-1; i++){
        b_prime(i) = b(i) - (a(i)/b_prime(i-1))*c(i-1);
        v_prime(i) = v(i) - (a(i)/b_prime(i-1))*v_prime(i-1);
    }
    for(unsigned int i = n-2; i >= 1; i--){
        u(i) = (v_prime(i) - c(i)*u(i+1))/b_prime(i);
    }

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


arma::vec thomasSpecial(const arma::vec& v){

  const size_t n   = arma::numel(v);

  double b_prime[n];
  arma::vec v_prime = arma::zeros(n);
  arma::vec u       = arma::zeros(n);

  v_prime(0) = v(0);

  for(unsigned int i = 1; i <= n-1; i++){
      b_prime[i] = static_cast<double>(i+1)/i;
      v_prime(i) = v(i) + (v_prime(i-1)/b_prime[i]);
  }
  // u[n] = v_prime[n]/b_prime[n];
  std::cout << u(n-1);

  for(unsigned int i = n-2; i >= 1; i--){
      u(i) = (v_prime(i) + u(i+1))/b_prime[i];
  }
  std::cout << u(n-1);
  return u;
}
