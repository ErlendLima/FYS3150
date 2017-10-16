#include <iostream>
#include <ctime>
#include <chrono>
#include <cmath>
#include "solver.h"
#include "solarSys.h"
#include "planet.h"

#define pi 3.141592653589793238462643383279502884197169

using namespace std::placeholders;

int Solver::solve(Method method, unsigned int N, double dt){
  char identifier;
  switch (method){
    case Method::EULER:
      std::cout << "=== Simulating system with Euler's method ===" << std::endl;
      identifier = 'E';
      solveEuler(N, dt);
    case Method::VERLET:
      std::cout << "=== Simulating system with Velocity Verlet method ===" << std::endl;
      identifier = 'V';
      // Call Verlet method function
      break;
    default:
      std::cout << "=== NO METHOD CHOSEN ===" << std::endl;
      return -1;
    }
    // Save to file?
    return 0;
}

void Solver::solveEuler(unsigned int n, double dt){
  // Loop over each planet, for each planet loop over other planets, find forces, sum em and forward equations

}
void Solver::initSystem(){
  SolarSys sys;
  sys.add(5.972e24 , 1., 0., 0., 0., 1., 0.); // Add planet 1
  sys.add(1.9891e30, 0., 0., 0., 0., 0., 0.); // Add sun in center
}

void Solver::startTiming(){
  startWallTime = std::chrono::high_resolution_clock::now();
  startCPUTime  = std::clock();
}

void Solver::endTiming(){
  auto CPUTime = (std::clock() - startCPUTime)/static_cast<double>(CLOCKS_PER_SEC);
  std::chrono::duration<double> wallTime = (std::chrono::high_resolution_clock::now() - startWallTime);
  std::cout << "Wall time: " << wallTime.count() << "s\n"
            << "CPU time: " << CPUTime << "s" << std::endl;
}
