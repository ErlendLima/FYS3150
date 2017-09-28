#include <armadillo>
#include <cmath>
#include "jacobi.h"

void jacobi(arma::vec& eigval, arma::mat& eigvec, arma::mat& A){
  unsigned int n = A.n_cols;
  double maxiter = 1e8;
  double epsilon = 1.0e-10;
  unsigned int iter = 0;
  unsigned int k, l;
  double max_offdiag = 1000.0;

  arma::mat A_copy = arma::mat(A);
  eigvec = arma::eye<arma::mat>(n, n); // Eigenvalue matrix

  while(fabs(max_offdiag) > epsilon && static_cast<double>(iter) < maxiter){
    // Find maximum off diagonal element
    find_max_offdiag(A_copy, k, l, n);
    max_offdiag = A_copy(l,k);
    jacobirotate(A_copy, eigvec, k, l, n);

    iter++;
  }
  std::cout << "Completed Jacobi eigenvalue algorithm with " << iter << " iterations" << std::endl;
  eigval = arma::sort(diagvec(A_copy));
}

void jacobirotate(arma::mat& A, arma::mat& R, unsigned int& k, unsigned int& l, unsigned int& n){
  // Rotate: Find values of cos and sin
  double s,c;
  if(A(k,l) != 0.0){
    double t,tau;
    tau = (A(l,l) - A(k,k))/(2*A(k,l));
    if(tau>0){
      t = 1.0/(tau + sqrt(1.0 + tau*tau));
    }
    else{
      t = -1.0/(-tau + sqrt(1.0 + tau*tau));
    }

    c = 1/sqrt(1+t*t);
    s = c*t;
  }
  else{
    c = 1.0;
    s = 0.0;
  }
  double a_kk, a_ll, a_ik, a_il, r_ik, r_il;
  a_kk = A(k,k);
  a_ll = A(l,l);
  // Changing the matrix elements with indices k and l
  A(k,k) = c*c*a_kk - 2.0*c*s*A(k,l) + s*s*a_ll;
  A(l,l) = s*s*a_kk + 2.0*c*s*A(k,l) + c*c*a_ll;
  A(k,l) = 0.0;
  A(l,k) = 0.0;
  // Then we change the remaining elements
  for(unsigned int i = 0; i<n; i++){
    if( i != k && i != l){
      a_ik = A(i,k);
      a_il = A(i,l);
      A(i,k) = c*a_ik - s*a_il;
      A(k,i) = A(i,k);
      A(i,l) = c*a_il + s*a_ik;
      A(l,i) = A(i,l);
    }
  r_ik = R(i,k);
  r_il = R(i,l);
  R(i,k) = c*r_ik - s*r_il;
  R(i,l) = c*r_il + s*r_ik;
  }
}

void find_max_offdiag(arma::mat& A, unsigned int& k, unsigned int& l, unsigned int& n){
  double max = 0.0;
  for(unsigned int i = 0; i < n; i++){
    for(unsigned int j = i+1; j < n; j++){
      if (fabs(A(i,j)) > max){
        max = fabs(A(i,j));
        l = i;
        k = j;
      }
    }
  }
}
