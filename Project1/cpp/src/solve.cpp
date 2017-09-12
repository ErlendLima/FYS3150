#include <armadillo>
#include "solve.h"

arma::vec thomas(const arma::vec& a, const arma::vec& b, const arma::vec& c, const arma::vec& v){
    /* Implementation of Thomas Algorithm */
    if(arma::numel(a) != arma::numel(b) ||
       arma::numel(b) != arma::numel(c) ||
       arma::numel(c) != arma::numel(v))
        throw std::runtime_error("Input vectors must be of equal length");

    const size_t n   = arma::numel(a);

    arma::vec b_prime = arma::zeros(n);
    arma::vec v_prime = arma::zeros(n);
    arma::vec u       = arma::zeros(n);

    b_prime(1) = b(1);
    v_prime(1) = v(1);

    for(unsigned int i = 2; i < n-1; i++){
        b_prime(i) = b(i) - (a(i)/b_prime(i-1))*c(i-1);
        v_prime(i) = v(i) - (a(i)/b_prime(i-1))*v_prime(i-1);
    }
    for(int i = n-2; i > 0; i--){
        u(i) = (v_prime(i) - c(i)*u(i+1))/b_prime(i);
    }

    return u;
}

arma::vec thomasBook(arma::vec& a, arma::vec& b, arma::vec&c, arma::vec& v){
    int N = arma::numel(a) - 2;
    arma::vec u = arma::zeros(N+2);
    double d;
    for (int i = 2; i < N+1; i++) {
        d = a(i-1)/b(i-1);
        b(i) -= c(i-1)*d;
        v(i) -= v(i-1)*d;
    }

    u(N) = v(N)/b(N);
    for(int i = N-1; i > 0; i--)
        u[i] = (v[i] - c[i]*u[i+1])/b[i];
    return u;
}

arma::mat tridiagonalMat(unsigned int size, double upper, double middle, double lower){
    auto mat = arma::mat(size, size, arma::fill::zeros);
    for (unsigned int row = 0; row < size; row++){
        if (row > 0)
            mat(row, row-1) = lower;
        mat(row, row) = middle;
        if (row < size-1)
            mat(row, row+1) = upper;
    }
    return mat;
}


arma::vec thomasSpecial(const arma::vec& v){

  const size_t n   = arma::numel(v);

  // double b_prime[n];
  arma::vec b_prime = arma::zeros(n);
  arma::vec v_prime = arma::zeros(n);
  arma::vec u       = arma::zeros(n);

  v_prime(0) = v(0);

  for(unsigned int i = 1; i <= n-1; i++){
      b_prime(i) = static_cast<double>(i+1)/i;
      v_prime(i) = v(i) + (v_prime(i-1)/b_prime(i));
  }

  for(unsigned int i = n-2; i >= 1; i--){
      u(i) = (v_prime(i) + u(i+1))/b_prime(i);
  }
  return u;
}
