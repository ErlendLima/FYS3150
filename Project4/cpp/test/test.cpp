#include <gtest/gtest.h>
#include <armadillo>
#include <memory>
#include <string>
#include "../src/ising.h"

#define TOLERANCE 1.0e-9

TEST(Ising, MagnetizationIsCorrect){
  // Check magnetization for some state
  arma::imat A;
  A << -1 << 1 << -1 << arma::endr
    <<  1 << 1 << -1 << arma::endr
    << -1 << 1 << -1;
  ASSERT_EQ(magnetization(A),-1);
}

TEST(Ising, CorrectInitialEntriesRandom){
  // Check that initialized state matrix has only 1 and -1 as entries.
  arma::imat A = setInitialStateRandom(100);
  ASSERT_FALSE(arma::any(arma::vectorise(A) != 1 && arma::vectorise(A) != -1));
}

TEST(Ising, CorrectInitialEntriesOrdered){
  // Simply check that each element is one in the initialized matrix.
  // Somewhat redundant, maybe remove this.....
  arma::imat A = setInitialStateOrdered(100, 1);
  ASSERT_TRUE(arma::all(arma::vectorise(A) == 1));
}

TEST(Ising, NeighborCheckCenter){
  // Check that neighborsum is correct for a special case
  arma::imat A;
  A << -1 << 1 << -1 << arma::endr
    <<  1 << 1 <<  1 << arma::endr
    << -1 << 1 << -1;
  ASSERT_EQ(sumNeighbors(1,1,A),4);
}

TEST(Ising, NeighborCheckEdge){
  // Check that summation of neighbors is correct for edge case
  arma::imat A;
  A << 100 << 0 << -3 << arma::endr
    << 0 << 100 << 100 << arma::endr
    << 2 << 1000 << 1000;
  ASSERT_EQ(sumNeighbors(0,0,A),-1);
}

TEST(Ising, ProbabilityMap){
  // Check that probability map implementation works. The calculation is the
  // as in the cpp implementation
  auto ps = makeProbabilities(1.0);
  for(double dE = -4.0; dE < 5.0; dE+=2){
    ASSERT_NEAR(ps[dE], exp(-dE), TOLERANCE);
  }
}

TEST(Ising, PeriodUpperBound){
  // Check that period returns the first index when outside upper bound is called
  ASSERT_EQ(period(10,10), 0);
}

TEST(Ising, PeriodLowerBound){
  // Check that period returns the last index when outside lower bound
  ASSERT_EQ(period(-1,3), 2);
}
