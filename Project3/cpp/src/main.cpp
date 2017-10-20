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
    std::string parameterpath = "../data/parameters.json";
  Solver solver(parameterpath);
  solver.solve();
  return 0;
}
