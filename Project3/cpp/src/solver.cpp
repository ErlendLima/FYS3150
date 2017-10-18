#include <iostream>
#include <ctime>
#include <chrono>
#include <cmath>
#include "solver.h"
#include "solarSys.h"
#include "planet.h"
#include "Vec3/vec3.h"

#define pi 3.141592653589793238462643383279502884197169
#define G 39.47841760435743447533796399950460454125479762896316250565

int Solver::solve(Method method, unsigned int N, double dt){
  char identifier;
  switch (method){
    case Method::EULER:
      std::cout << "=== Simulating system with Euler's method ===" << std::endl;
      identifier = 'E';
      initSystem();
      solveEuler(N, dt);
      break;
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
  double t = 0.0;
  vec3 diff;

  // Setup array to save positions
  double positions[3][sys.n_planets][n] = {0.0};

  // Loop over time
  for(unsigned int i = 0; i < n; i++){
    t = (static_cast<double>(i)*dt);
    // Loop over every planet to find acceleration of each planet
    for(auto & planet: sys.planets){
        planet->resetAcc();
        // Loop over all the other planets
        for(auto & other: sys.planets){
            if(planet == other){
              continue;
            }
            else{
              diff = other->pos - planet->pos;
              planet->acc += diff*G*(other->mass)/pow(diff.length(),3);
            }
        }
    }
    // Forward positions after calculating accelerations
    for(auto & planet: sys.planets){
      planet->vel += planet->acc*dt;
      planet->pos += planet->vel*dt;
    }
  }
}

void Solver::initSystem(){
  sys.add(3.0e-6, 1.0, 0.0, 0.0, 0.0, 2*pi, 0.0); // Add planet earth
  sys.add(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0); // Add sun in center
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
