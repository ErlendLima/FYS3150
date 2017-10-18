#include <iostream>
#include <cmath>
#include <memory.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "solver.h"
#include "planet.h"
#include "solarSys.h"
#include "Vec3/vec3.h"

int main(int argc, char *argv[]) {
  Solver solver;
  Method method = Method::EULER; // Define method
  unsigned int N = 50;          // Simulate 365 days
  double dt = 1.0/50.0;           // Step size one day
  solver.solve(method, N, dt);
  return 0;
}

/* KLASSE FOR SOLVER,
   KLASSE FOR OBJEKTER (Planet, stjerne, osv),
   KLASSE FOR FELLES I ODE SOLVER
*/
//Nesten et pent lite dikt :)
