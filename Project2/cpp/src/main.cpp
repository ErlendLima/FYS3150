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

  unsigned int N = 300;

  solve(1e-4, 10, N);
  return 0;
}
