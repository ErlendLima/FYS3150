#include <iostream>
#include <ctime>
#include <chrono>
#include <cmath>
#include "solver.h"
#include "solarSys.h"
#include "planet.h"

#define pi 3.141592653589793238462643383279502884197169

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
  sys.modifyTime(0.0);
  double Fx;
  double Fy;
  double Fz;

  std::vector<std::shared_ptr<Planet>>::iterator iter;
  std::vector<std::shared_ptr<Planet>>::iterator iter2;

  // Loop over time
  for(unsigned int i = 0; i < n; i++){
    sys.modifyTime(static_cast<double>(i)*dt);
    // Loop over every planet
    for(iter = sys.planets.begin(); iter != sys.planets.end(); iter++){
      // Loop over every other planet for each planet
      for(iter2 = sys.planets.begin(); iter2 != sys.planets.end(); iter2++){
        if(iter == iter2){continue;}
        else{
          std::cout << "hei" << std::endl;
        }
      // Forward velocity and position
      }
    }
    std::cout << sys.getTime() << std::endl;
  }
}

void Solver::initSystem(){
  sys.add(5.972e24 , 1.0, 0.0, 0.0, 0.0, 1.0, 0.0); // Add planet 1
  sys.add(1.9891e30, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0); // Add sun in center
  sys.add(2.3213e10, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0);
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
