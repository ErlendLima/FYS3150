#include <memory>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include "solver.h"
#include "solve.h"

using pvec = std::unique_ptr<arma::vec>;

Solver::Solver(double (*function)(double)){
    fn = function;
}

Solver::~Solver(){};

void Solver::setup(unsigned int n){
    double h = 1/(static_cast<double>(n+1));

    btilde = h*arma::linspace(0, n+2, n+2); // Armadillo lacks arange :(
    btilde.transform(fn);
    btilde *= h*h; // b~ = f(x)h^2

    // Set the boundary conditions
    btilde[0] = lowerBound; btilde[n+1] = upperBound;
}

void Solver::solve(Method method, unsigned int low, unsigned int high, unsigned int step) {
    char identifier;
    for(unsigned int n = low; n <= high; n *= step){
        std::cout << "Solving for " << n << "×" << n << " ";
        switch (method) {
        case Method::GENERAL:
            std::cout << "using the general method\n";
            solveGeneral(n);
            identifier = 'G';
            break;
        case Method::SPECIAL:
            std::cout << "using the specialized method\n";
            identifier = 'S';
            break;
        case Method::LU:
            std::cout << "using LU decomposition\n";
            solveLU(n);
            identifier = 'L';
            break;
        }
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

    auto startWallTime = std::chrono::high_resolution_clock::now();
    auto startCPUTime  = std::clock();
    solution = thomas(a, b, c, btilde);
    auto CPUTime = (std::clock() - startCPUTime)/static_cast<double>(CLOCKS_PER_SEC);
    std::chrono::duration<double> wallTime = (std::chrono::high_resolution_clock::now() - startWallTime);
    std::cout << "Wall time: " << wallTime.count() << "s\n"
              << "CPU time:  " << CPUTime << "s" << std::endl;
}

void Solver::solveLU(unsigned int n) {
    setup(n);
    arma::mat A = tridiagonalMat(n+2, -1, 2, -1);
    arma::mat L, U;
    arma::vec y;
    // Ax = b -> LUx = b -> Ly = b -> Ux = y
    arma::lu(L,U,A);
    y = arma::solve(L, btilde);
    solution = arma::solve(U, y);
}

void Solver::save(const std::string& identifier){
    solution.save(savepath+identifier, arma::raw_ascii);
}
