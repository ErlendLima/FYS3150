#include <iostream>
#include <cmath>
#include <memory.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <armadillo>
#include "jacobi.h"
#include "schrodinger.h"

int main(int argc, char const *argv[]) {

  arma::vec timings = arma::vec(40);

  for(unsigned int i = 1; i <= 40; i++){
    timings(i-1) = solve(1e-4, 10, i*5);
  }
  timings.save("../data/timings_jacobi.txt", arma::raw_ascii);
  return 0;
}
