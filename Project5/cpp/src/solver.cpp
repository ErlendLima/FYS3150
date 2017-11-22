#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <armadillo>
#include "solver.h"
#include "metamodel.h"

#define G 39.478417604357434475337963

using std::placeholders::_1;

Solver::Solver(Metamodel& metamodel)
    : model(metamodel),
      dt(model.getDt()),
      dx(model.getDx()),
      dy(model.getDy())
{}

int Solver::solve(){
    // Wrapper for initializing, solving and saving the solution of the system.
    initSystem();

    std::function<void(std::shared_ptr<Planet>)> stepOne;
    std::function<void(std::shared_ptr<Planet>)> stepTwo;

    switch (method){
    case Method::FORWARD_EULER:
      std::cout << "=== Using Forward Euler ===" << std::endl;
      stepOne = std::bind(&Solver::nop, this, _1);
      stepTwo = std::bind(&Solver::EulerStep, this, _1);
      solveSystem(stepOne, stepTwo);
      break;
    case Method::BACKWARD_EULER:
      std::cout << "=== Using Backward Euler ===" << std::endl;
      stepOne = std::bind(&Solver::VerletStep1, this, _1);
      stepTwo = std::bind(&Solver::VerletStep2, this, _1);
      solveSystem(stepOne, stepTwo);
      break;
    case Method::CRANK_NICOLSON:
      std::cout << "=== Using Crank-Nicolson Scheme ===" << std::endl;
      stepOne = std::bind(&Solver::nop, this, _1);
      stepTwo = std::bind(&Solver::ECStep, this, _1);
      solveSystem(stepOne, stepTwo);
      break;
    default:
      std::cout << "=== NO METHOD CHOSEN ===" << std::endl;
      return -1;
    }
    saveToFile();
    return 0;
}

void Solver::solveSystem(std::function<void(std::shared_ptr<Planet>)>& stepOne,
                         std::function<void(std::shared_ptr<Planet>)>& stepTwo){
    // Solves the time development of the system. To generalize the code for
    // both algorithms, the forwarding functions are passed as args. The first
    // step is only used for the Verlet method because the acceleration is
    // updated mid-step.
    startTiming();
    sys.updateCOM();
    updateForces();
    updateEnergy(0);
    angMomArray(0) = sys.angularMomentum().length();
    double progress = 0.1;
    // Loop over time
    // Write out the progress for each 10% increment
    if(i/static_cast<double>(n) >= progress){
        std::cout << 100*progress << "% completed" << std::endl;
        progress += 0.1;
    }
    endTiming();
}

void Solver::EulerStep(std::shared_ptr<Planet> planet){
    planet->vel_tmp = planet->vel;   // Save velocity temporarily for Euler
    planet->vel    += planet->acc*dt;
    planet->pos    += planet->vel_tmp*dt;
}

void Solver::VerletStep1(std::shared_ptr<Planet> planet){
    planet->pos += planet->vel*dt + 0.5*dt*dt*planet->acc;
}

void Solver::VerletStep2(std::shared_ptr<Planet> planet){
    planet->vel += 0.5*dt*(planet->acc + planet->acc_prev);
}

void Solver::ECStep(std::shared_ptr<Planet> planet){
    planet->vel += planet->acc*dt;
    planet->pos += planet->vel*dt;
}

void Solver::startTiming(){
  // Start timing the algorithm (both wall time and cpu time)
  startWallTime = std::chrono::high_resolution_clock::now();
  startCPUTime  = std::clock();
}

void Solver::endTiming(){
  // Print wall time and cpu time when called
  auto CPUTime = (std::clock() - startCPUTime)/static_cast<double>(CLOCKS_PER_SEC);
  std::chrono::duration<double> wallTime = (std::chrono::high_resolution_clock::now() - startWallTime);
  std::cout << "Wall time: " << wallTime.count() << "s\n"
            << "CPU time: " << CPUTime << "s" << std::endl;
}
