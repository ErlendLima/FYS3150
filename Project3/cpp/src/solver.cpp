#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cmath>
#include <memory>
#include <armadillo>
#include "solver.h"
#include "solarSys.h"
#include "planet.h"
#include "Vec3/vec3.h"

#define G 39.478417604357434475337963

using std::placeholders::_1;

int Solver::solve(Method method, unsigned int N, double timestep){
  n = N;
  dt = timestep;
  initSystem();

  char identifier;

  // Holds step function for the chosen method
  std::function<void(std::shared_ptr<Planet>)> stepper;
  switch (method){
    case Method::EULER:
      std::cout << "=== Simulating system with Euler's method ===" << std::endl;
      identifier = 'E';
      stepper = std::bind(&Solver::EulerStep, this, _1);
      break;
    case Method::VERLET:
      std::cout << "=== Simulating system with Velocity Verlet method ===" << std::endl;
      identifier = 'V';
      stepper = std::bind(&Solver::VerletStep, this, _1);
      break;
    default:
      std::cout << "=== NO METHOD CHOSEN ===" << std::endl;
      return -1;
    }
    solveSystem(stepper);
    if(saveFlag) saveToFile();
    return 0;
}

void Solver::solveSystem(std::function<void(std::shared_ptr<Planet>)>& stepper){
  double t = 0.0;

  startTiming();
  // Loop over time
  for(unsigned int i = 1; i <= n; i++){
    // Loop over every planet to find acceleration of each planet
    for(auto & planet: sys.planets){
      if(planet->name == "Sun") continue;
      planet->resetAcc();
      planet->resetF();

      // Loop over all the other planets
      for(auto & other: sys.planets){
          if(planet == other)continue;
          else{
            planet->accp = planet->acc;
            planet->calculateAcc(*other);
          }
      }
    }
    // Forward planet positions in time with method of choice
    for(auto & planet: sys.planets){
      stepper(planet);
      planet->writePosToMat(i);
    }
    t = i*dt;
  }
  endTiming();
}

void Solver::initSystem(){
  sys.add("Sun", 1.0, vec3(), vec3(), n); // Add sun in center
  sys.add("Earth", 3.0e-6, vec3(1.0, 0.0, 0.0), vec3(0.0, 2*pi, 0.0), n); // Add planet earth
}

void Solver::EulerStep(std::shared_ptr<Planet> planet){
  planet->vel += planet->acc*dt;
  planet->pos += planet->vel*dt;

  planet->updateKinetic();
}
void Solver::VerletStep(std::shared_ptr<Planet> planet){
  planet->pos += planet->vel + dt*dt*planet->acc/2;
  planet->vel += dt*(planet->acc + planet->accp)/2;

  planet->updateKinetic();
}

void Solver::saveToFile(){
  std::ofstream myfile;
  myfile.open(savepath + "/cpp.txt");
  // Loop over time points
  for(unsigned int i = 0; i < n; i++){
    // Loop over planets
    unsigned int j = 0;
    for(auto & planet: sys.planets){
      myfile << planet->pos_array(0,i) << " ";
      myfile << planet->pos_array(1,i) << " ";
      myfile << planet->pos_array(2,i) << " ";
      if(j == sys.n_planets-1){
        myfile << "\n";}
      j++;
    }
  }
  myfile.close();
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
