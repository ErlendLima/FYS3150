#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <armadillo>
#include "solver.h"
#include "metamodel.h"
#include "timer.h"

using std::placeholders::_1;

Solver::Solver(Metamodel& metamodel)
    : model(metamodel),
      tsteps(metamodel.getTsteps()),
      xsteps(metamodel.getXsteps())
{}

int Solver::solve(){
    // Wrapper for initializing, solving and saving the solution of the system.
    double alpha = model.getAlpha();
    arma::mat u = model.getU();
    Timer timer;
    timer.start();

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

    timer.print();
    model.getU() = u;
    model.save();
    return 0;
}

void Solver::forwardEuler(double alpha, arma::mat& u) const{
    for (unsigned int t = 1; t < tsteps; t++){
        forwardStep(alpha, u, t);
    }
}

void Solver::forwardStep(double alpha, arma::mat& u, unsigned int t) const{
    for (unsigned int x = 1; x < xsteps+1; x++){
        u(t, x) = alpha*u(t-1, x-1) + (1 - 2*alpha)*u(t-1, x) + alpha*u(t-1, x+1);
    }
}

void Solver::backwardEuler(double alpha, arma::mat& u) const{
    for (unsigned int t = 1; t < tsteps; t++) {
        for (unsigned int x = 0; x < xsteps; x++)
            u(t, x) = u(t-1, x);
        tridiag(alpha, u, t);
    }
}

void Solver::crankNicolson(double alpha, arma::mat& u) const{
    for (unsigned int t = 1; t < tsteps; t++) {
        u.row(t).print();
        forwardStep(alpha/2, u, t);
        u.row(t).print();
        tridiag(alpha/2, u, t);
        u.row(t).print();
        std::cout << "+++++++++++++++++++++++\n";
    }
    u.row(0).print();
    u.row(tsteps-4).print();
    u.row(tsteps-3).print();
    u.row(tsteps-2).print();
    u.row(tsteps-1).print();
}

void Solver::tridiag(double alpha, arma::mat& u, unsigned int t) const{
    arma::vec d = arma::zeros<arma::vec>(xsteps) + (1 + 2*alpha); // Diagonal elements
    arma::vec b = arma::zeros<arma::vec>(xsteps-1) - alpha;       // Offdiagonal elements

  for(unsigned int i = 1; i < xsteps; i++){
    // Normalize row i
    b(i-1)  /= d(i-1);
    u(t, i) /= d(i-1);
    d(i-1)   = 1.0;
    // Eliminate
    u(i+1)  += u(t, i)*alpha;
    d(i)    += b(i-1)*alpha;
  }
  // Normalize bottom row
  u(t, xsteps) /= d(xsteps-1);
  d(xsteps-1)  = 1.0;

  // Backward substitute
  for(unsigned int i = xsteps; i >= 2; i--){
      u(t, i) -= u(t, i+1)*b(i-2);
  }
  u(t, 1) -= u(t, 2)*b(xsteps-2);
}
