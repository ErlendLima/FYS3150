#include <gtest/gtest.h>
#include <armadillo>
#include <memory>
#include "../src/jacobi.h"

#define TOLERANCE 1e-6

// Test offdiagonal element finder for symmetric 3x3 matrix
TEST(find_max_offdiag, offdiag_symmetric_3x3){
  arma::mat A;
  A << -1 << 2 << 3 << arma::endr
    <<  2 << 1 << 2 << arma::endr
    <<  3 << 2 << 5;

  unsigned int k, l, n;
  n = 3;

  find_max_offdiag(A, k, l, n);

  ASSERT_EQ(l, 0);
  ASSERT_EQ(k, 2);
}

// Random 50x50 matrix with one element manipulated to 100, which is the
// max element the function is looking for
TEST(find_max_offdiag, offdiag_random_50x50){
  unsigned int l, k, n;
  l = 0;
  k = 4;
  n = 50;
  arma::mat A = arma::randu(n,n);
  A(l,k) = 100.0;
  find_max_offdiag(A, k, l, n);

  ASSERT_EQ(l, 0);
  ASSERT_EQ(k, 4);
}

// Test eigenvalues for a simple matrix (3x3) with known eigenvalues
TEST(jacobi, jacobi_eigenvals_3x3){
  arma::mat A;
  arma::vec eigvals_computed;
  arma::mat eigvecs_computed;
  arma::vec eigvals_expected;
  double tol = 1e-8;

  // Set up A with known eigenvalues
  A << 3 << 2 << 4 << arma::endr
    << 2 << 0 << 2 << arma::endr
    << 4 << 2 << 3;

  eigvals_expected << -1 << -1 << 8;

  jacobi(eigvals_computed, eigvecs_computed, A);

  eigvals_computed.print();
  eigvals_expected.print();

  ASSERT_TRUE(arma::approx_equal(eigvals_computed, eigvals_expected, "absdiff", tol));
}
