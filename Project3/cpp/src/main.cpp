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
  Method method = Method::VERLET; // Define method
  unsigned int N_peryear = 1000;  // Steps per year
  double years           = 2;     // Number of years to sim
  unsigned int N         = static_cast<unsigned int>(years*N_peryear);
  double dt              = 1.0/N_peryear;
  Solver solver;
  solver.doSave(true);
  solver.solve(method, N, dt);
  return 0;
}
