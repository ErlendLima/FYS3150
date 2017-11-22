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
      tsteps(model.getTsteps()),
      xsteps(model.getXsteps())
{}

int Solver::solve(){
    // Wrapper for initializing, solving and saving the solution of the system.
    initSystem();
    double alpha = model.getAlpha();
    arma::mat u = model.getU();

    switch (model.getMethod()){
    case Method::FORWARD_EULER:
        std::cout << "=== Using Forward Euler ===" << std::endl;
        forwardEuler(alpha, u);
        break;
    case Method::BACKWARD_EULER:
        std::cout << "=== Using Backward Euler ===" << std::endl;
        backwardEuler(alpha, u);
        break;
    case Method::CRANK_NICOLSON:
        std::cout << "=== Using Crank-Nicolson Scheme ===" << std::endl;
        crankNicolson(alpha, u);
        break;
    default:
        std::cout << "=== NO METHOD CHOSEN ===" << std::endl;
        return -1;
    }
    model.save();
    return 0;
}

void Solver::forwardEuler(double alpha, arma::mat& u) const{
    for (unsigned int t = 0; t < tsteps; t++)
        forwardStep(alpha, u[t], u[t-1]);
}

void Solver::forwardStep(double alpha, arma::mat& u, arma::mat& uPrev) const{
    for (unsigned int x = 1; x < xsteps; x++)
        u[x] = alpha*uPrev[x-1] + (1 - 2*alpha)*uPrev[x] + alpha*uPrev[x+1];
}

void Solver::backwardEuler(double alpha, arma::mat& u) const{
    for (unsigned int t = 1; t < tsteps; t++) {
        u[t] = u[t-1];
        tridiag(alpha, u[t], xsteps);
    }
}

void Solver::crankNicolson(double alpha, arma::mat& u) const{
    for (unsigned int t = 1; t < tsteps; t++) {
        forwardStep(alpha/2, u[t], u[t-1]);
        tridiag(alpha/2, u[t], N);
    }
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
