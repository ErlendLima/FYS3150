#include <iostream>
#include <cmath>
#include <memory.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "solver.h"
#include "planet.h"
#include "solarSys.h"

int main(int argc, char *argv[]) {
  Solver solver;
  Method method = Method::EULER;
  unsigned int N = 10;
  double dt = 0.1;
  solver.solve(method, N, dt);
  return 0;
}

//KLASSE FOR SOLVER, KLASSE FOR OBJEKTER (Planet, stjerne, osv), KLASSE FOR FELLES I ODE SOLVER
