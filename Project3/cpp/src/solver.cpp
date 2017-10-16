#include <iostream>
#include <ctime>
#include <chrono>
#include <cmath>
#include "solver.h"

using namespace std::placeholders;

// Constructor
Solver::Solver(){
  saveFlag = false;  // Default to no save
  tot_planets = 0;
  radius      = 100;
  tot_mass    = 0;
  G = 4*M_PI*M_PI
  std::cout << "Constructed Solver instance" << std::endl;
}

// Destructor
Solver::~Solver(){};

int Solver::solve(Method method, double time, ){
  char identifier;
  switch (method){
    case Method::EULER:
      std::cout << "=== Simulating system with Euler's method ===" << std::endl;
      // Call Euler method function
    case Method::VERLET:
      std::cout <<< "=== Simulating system with Velocity Verlet method ===" << std::endl;
      // Call Verlet method function
      identifier = 'V';
      break;
    default:
      std::cout << "=== NO METHOD CHOSEN ===" << std::endl;
      return -1;
    }
    // Save to file?
    return 0;
}

void Solver::solveEuler(unsigned int n, double dt){
  std::cout << "NOT IMPLEMENTED :(" << std::endl;
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
