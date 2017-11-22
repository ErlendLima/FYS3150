#include <iostream>
#include <fstream>
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

    std::function<void(double, arma::mat&)> solver;

    switch (method){
    case Method::FORWARD_EULER:
        std::cout << "=== Using Forward Euler ===" << std::endl;
        solver = std::bind(&Solver::forwardEuler, this, _1);
        solveSystem(solver);
        break;
    case Method::BACKWARD_EULER:
        std::cout << "=== Using Backward Euler ===" << std::endl;
        solver = std::bind(&Solver::forwardEuler, this, _1);
        solveSystem(solver);
        break;
    case Method::CRANK_NICOLSON:
        std::cout << "=== Using Crank-Nicolson Scheme ===" << std::endl;
        solver = std::bind(&Solver::forwardEuler, this, _1);
        solveSystem(solver);
        break;
    default:
        std::cout << "=== NO METHOD CHOSEN ===" << std::endl;
        return -1;
    }
    saveToFile();
    return 0;
}

void Solver::solveSystem(std::function<void(double, arma::mat)>& method){
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

void Solver::forwardEuler(double alpha, arma::mat& u) const{
    for (unsigned int t = 0; t < tsteps; t++)
        forwardStep(alpha, u[t], u[t-1], xsteps);
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
        forwardStep(alpha/2, u[t], u[t-1], xsteps);
        tridiag(alpha/2, u[t], N);
    }
}

void Solver::tridiag(double alpha, arma::vec& u, unsigned int N){
  arma::vec d = arma::zeros<arma::vec>(N) + (1 + 2*alpha) // Diagonal elements
  arma::vec b = arma::zeros<arma::vec>(N) - alpha         // Offdiagonal elements

  for(unsigned int i = 1; i < N; i++){
    // Normalize row i
    b[i-1] /= d[i-1];
    u[i]   /= d[i-1];
    d[i-1]  = 1.0;
    // Eliminate
    u[i+1] += u[i]*alpha;
    d[i]   += b[i-1]*alpha;
  }
  // Normalize bottom row
  u[N]   /= d[N-1];
  d[N-1]  = 1.0;

  // Backward substitute
  for(unsigned int i = N, i > 1; i--){
    u[i-1] -= u[i]*b[i-2];
    b[i-2] = 0.0 // This is never read, why bother >:(
  }
}
