#include <gtest/gtest.h>
#include <armadillo>
#include <memory>
#include "../src/ising.h"

#define TOLERANCE 1.0e-9

TEST(Ising, mag){
  arma::mat A;
  A << -1 << 1 << -1 << arma::endr
    <<  1 << 1 << -1 << arma::endr
    << -1 << 1 << -1;
  ASSERT_EQ(magnetization(A),-1);
}
