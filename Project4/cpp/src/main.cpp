#include <iostream>
#include <cmath>
#include <memory.h>
#include <random>
#include <armadillo>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "ising.h"

int main(int argc, char const *argv[]) {

  unsigned int L = 2;    // Lattice size
  arma::mat spin(L,L);   // Matrix for spins in the system



  std::mt19937 gen;
  gen.seed(1233);

  std::uniform_real_distribution<double> distribution(0.0,1.0);
  double x = distribution(gen);
  std::cout << x << std::endl;
  return 0;
}
