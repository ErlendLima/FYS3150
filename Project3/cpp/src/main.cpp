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
  // Initialize solver and run simulation. The initial values and other parameters
  // are provided in a .json file. See git repo for detailed description.
  std::string parameterpath = "../data/parameters.json";
  // try {
      Solver solver(parameterpath);
      solver.solve();
  // } catch (std::exception &ex) {
  //     std::cerr << "Error: " << ex.what() << std::endl;
  // }

  return 0;
}
