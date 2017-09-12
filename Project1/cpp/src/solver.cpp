#include <memory>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <omp.h>
#include "solver.h"
#include "solve.h"

using namespace std::placeholders;

Solver::Solver(const std::function<double(double)>& function){
    fn = function;
}

Solver::~Solver(){};

std::unique_ptr<arma::vec> Solver::makeDomain(unsigned int n){
    double h = 1/static_cast<double>(n+1);
    auto domain = std::make_unique<arma::vec>(h*arma::linspace(0, n+1, n)); // Armadillo lacks arange :(
    return std::move(domain);
}

std::unique_ptr<arma::vec> Solver::makeBtilde(unsigned int n){
    double h = 1/static_cast<double>(n+1);
    std::unique_ptr<arma::vec> btilde = makeDomain(n);
    btilde->transform(fn);
    (*btilde) *= h*h; // b~ = f(x)h^2

    // Set the boundary conditions
    return std::move(btilde);
}

int Solver::solve(Method method, unsigned int low, unsigned int high, unsigned int step) {
    char identifier;
    // void (Solver::*targetSolver)(unsigned int);
    std::function<void(unsigned int)> targetSolver;
    switch (method) {
    case Method::GENERAL:
        std::cout << "=== Using the general method ===" << std::endl;
        targetSolver = std::bind(&Solver::solveGeneral, this, _1);
        identifier = 'G';
        break;
    case Method::SPECIAL:
        std::cout << "=== Using the specialized method ===" << std::endl;
        targetSolver = std::bind(&Solver::solveSpecial, this, _1);
        identifier = 'S';
        break;
    case Method::LU:
        std::cout << "=== Using LU decomposition === " << std::endl;
        targetSolver = std::bind(&Solver::solveLU, this, _1);
        identifier = 'L';
        break;
    default:
        std::cout << "=== NO METHOD CHOSEN ===" << std::endl;
        return -1;
    }

    for(unsigned int n = low; n <= high; n *= step){
        std::cout << "Solving for " << n << "×" << n << " matrix using "
                  << repetitions << " repetitions\n";
        targetSolver(n);
        if(saveFlag){
            std::stringstream name;
            name << identifier << n << ".txt";
            save(name.str());
        }
    }
    return 0;
}

void Solver::solveGeneral(unsigned int n) {
    auto btilde = makeBtilde(n);
    arma::vec a = arma::vec(n); a.fill(-1);
    arma::vec b = arma::vec(n); b.fill(2);
    arma::vec c = arma::vec(n); c.fill(-1);

    startTiming();
    for(unsigned int r = 0; r < repetitions; r++)
        solution = thomas(a, b, c, (*btilde));
    endTiming();
}

void Solver::solveSpecial(unsigned int n){
    auto btilde = makeBtilde(n);

    startTiming();
    for(unsigned int r = 0; r < repetitions; r++)
        solution = thomasSpecial((*btilde));
    endTiming();
}

void Solver::solveLU(unsigned int n) {
    auto btilde = makeBtilde(n);
    arma::mat A = tridiagonalMat(n, -1, 2, -1);
    arma::mat L, U;
    arma::vec y;
    std::cout << '\n';

    // Ax = b -> LUx = b -> Ly = b -> Ux = y
    startTiming();
    for(unsigned int r = 0; r < repetitions; r++){
        arma::lu(L,U,A);
        y = arma::solve(L, (*btilde));
        solution = arma::solve(U, y);
    }
    endTiming();
    solution.print();
}

void Solver::save(const std::string& identifier){
    solution.save(savepath+identifier, arma::raw_ascii);
}

void Solver::startTiming(){
    startWallTime = std::chrono::high_resolution_clock::now();
    startCPUTime  = std::clock();
}

void Solver::endTiming(){
    auto CPUTime = (std::clock() - startCPUTime)/CLOCKS_PER_SEC;
    std::chrono::duration<double> wallTime = (std::chrono::high_resolution_clock::now() - startWallTime);
    std::cout << "Average wall time: " << wallTime.count()/repetitions << "s\n"
              << "Average CPU time:  " << CPUTime/repetitions << "s" << std::endl;
}

void Solver::calculateError(unsigned int start, unsigned int stop){
    unsigned int iterations = 50;
    unsigned int j          = 0;
    arma::vec logspace      = arma::logspace(log10(start), log10(stop), iterations);
    arma::mat errors        = arma::zeros(iterations+1, 2);

    logspace.transform(floor);
    for (const unsigned int& n: logspace) {
        // Set up the necessary vectors
        arma::vec domain     = *makeDomain(n);
        arma::vec btilde     = *makeBtilde(n);
        arma::vec analytical = domain;
        arma::vec error      = arma::zeros(n);
        analytical.transform(fnAnalytical);

        // Set up the method
        arma::vec a = arma::vec(n); a.fill(-1);
        arma::vec b = arma::vec(n); b.fill(2);
        arma::vec c = arma::vec(n); c.fill(-1);

        arma::vec numerical = thomas(a, b, c, btilde);

        // Compute relative error
        error = arma::abs(analytical - numerical)/analytical;
        auto i = error(arma::span(0, n-2)).index_max();
        std::cout << "n = " << n
                  << ":\t x = " << domain(i)
                  << "\tN(x) = " << numerical(i)
                  << "\tf(x) = " << analytical(i)
                  << "\t|N-f|/f = " << error(i) << '\n';
        errors(j, 0) = n;
        errors(j, 1) = error(i);
        j++;
    }
    errors.save(savepath + "E.txt", arma::raw_ascii);
}
