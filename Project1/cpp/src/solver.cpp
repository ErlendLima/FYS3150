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

int Solver::solve(Method method, unsigned int low, unsigned int high, unsigned int step) {
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
    default:
        std::cout << "=== NO METHOD CHOSEN ===" << std::endl;
        return -1;
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
    return 0;
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
        solution = thomasSpecial(btilde);
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

void Solver::calculateError(unsigned int n_start, unsigned int n_stop, unsigned int step){
    saveFlag = false;  // Dont dump everything
    unsigned int n_iterations = (n_stop - n_start)/step;
    double errors[2][n_iterations] = {0};
    unsigned int yo = 0;

    std::ofstream outputFile("data/E.txt");

    for(unsigned int n = n_start; n <= n_stop; n+=step){
        setup(n);          // Reset setup
        arma::vec x_num     = arma::zeros(n);
        arma::vec x_ana     = arma::zeros(n);
        arma::vec rel_error = arma::zeros(n);

        // TODO: Replace with specialized algorithm, currently an issue with running it
        // arma::vec a = arma::vec(n+2); a.fill(-1);
        // arma::vec b = arma::vec(n+2); b.fill(2);
        // arma::vec c = arma::vec(n+2); c.fill(-1);
        // b[0] = 1; c[0] = 0; b[n+1] = 1; a[n+1] = 0;

        x_ana  = analyticSolution(domain);
        x_num  = thomasSpecial(btilde);

        for(unsigned int i = 2; i <= n-2; i++){
            rel_error[i] = fabs((x_num[i] - x_ana[i])/x_ana[i]);
        }

        errors[0][yo] = n;
        errors[1][yo] = log10(rel_error.max());
        outputFile << n << " " << errors[1][yo] << std::endl;
        std::cout << "n = " << errors[0][yo];
        std::cout << " Log Relative error = "<< errors[1][yo] << std::endl;
        yo += 1;
    }
    outputFile.close();
}
