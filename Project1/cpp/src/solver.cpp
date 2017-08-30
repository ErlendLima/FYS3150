#include <memory>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include "solver.h"
#include "solve.h"

Solver::Solver(double (*function)(double)){
    fn = function;
}

Solver::~Solver(){};

void Solver::setup(unsigned int n){
    double h = 1/static_cast<double>(n+2);
    domain = h*arma::linspace(0, n+2, n+2); // Armadillo lacks arange :(

    btilde = domain;
    btilde.transform(fn);
    btilde *= h*h; // b~ = f(x)h^2

    // Set the boundary conditions
    btilde[0] = lowerBound; btilde[n+1] = upperBound;
}

void Solver::solve(Method method, unsigned int low, unsigned int high, unsigned int step) {
    char identifier;
    void (Solver::*targetSolver)(unsigned int);
    switch (method) {
    case Method::GENERAL:
        std::cout << "=== Using the general method ===" << std::endl;
        targetSolver = &Solver::solveGeneral;
        identifier = 'G';
        break;
    case Method::SPECIAL:
        std::cout << "=== Using the specialized method ===" << std::endl;
        targetSolver = &Solver::solveSpecial;
        identifier = 'S';
        break;
    case Method::LU:
        std::cout << "=== Using LU decomposition === " << std::endl;
        targetSolver = &Solver::solveLU;
        identifier = 'L';
        break;
    }

    for(unsigned int n = low; n <= high; n *= step){
        std::cout << "Solving for " << n << "×" << n << " matrix using "
                  << repetitions << " repetitions\n";
        (this->*targetSolver)(n);
        if(saveFlag){
            std::stringstream name;
            name << identifier << n << ".txt";
            save(name.str());
        }
    }
}

void Solver::solveGeneral(unsigned int n) {
    setup(n);
    arma::vec a = arma::vec(n+2); a.fill(-1);
    arma::vec b = arma::vec(n+2); b.fill(2);
    arma::vec c = arma::vec(n+2); c.fill(-1);
    b[0] = 1; c[0] = 0; b[n+1] = 1; a[n+1] = 0;

    startTiming();
    for(unsigned int r = 0; r < repetitions; r++)
        solution = thomas(a, b, c, btilde);
    endTiming();
}

void Solver::solveSpecial(unsigned int n){
    setup(n);

    startTiming();
    for(unsigned int r = 0; r < repetitions; r++)
        solution = thomasSpecial(-1, 2, -1, btilde);
    endTiming();
}

void Solver::solveLU(unsigned int n) {
    setup(n);
    arma::mat A = tridiagonalMat(n+2, -1, 2, -1);
    arma::mat L, U;
    arma::vec y;

    // Ax = b -> LUx = b -> Ly = b -> Ux = y
    startTiming();
    for(unsigned int r = 0; r < repetitions; r++){
        arma::lu(L,U,A);
        y = arma::solve(L, btilde);
        solution = arma::solve(U, y);
    }
    endTiming();
}

void Solver::save(const std::string& identifier){
    solution.save(savepath+identifier, arma::raw_ascii);
}

void Solver::startTiming(){
    startWallTime = std::chrono::high_resolution_clock::now();
    startCPUTime  = std::clock();
}

void Solver::endTiming(){
    auto CPUTime = (std::clock() - startCPUTime)/static_cast<double>(CLOCKS_PER_SEC);
    std::chrono::duration<double> wallTime = (std::chrono::high_resolution_clock::now() - startWallTime);
    std::cout << "Average wall time: " << wallTime.count()/repetitions << "s\n"
              << "Average CPU time:  " << CPUTime/repetitions << "s" << std::endl;
}
