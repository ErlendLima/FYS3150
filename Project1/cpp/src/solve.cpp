
#include <armadillo>
#include <fstream>
#include <math.h>
#include <sstream>
#include <chrono>
#include <ctime>
#include "solve.h"

void solveGeneral(double (*func)(double), const unsigned int low, const unsigned int high, const unsigned int step){
    for(unsigned int n = low; n <= high; n *= step){
        double h = 1/(static_cast<double>(n+1));
        std::cout << "Solving " << n << "×" << n << " with stepsize " << h <<  std::endl;

        arma::vec btilde = h*arma::linspace(0, n+2, n+2); // Armadillo lacks arange :(
        btilde.transform(func);
        btilde *= h*h; // b~ = f(x)h^2

        arma::vec a = arma::vec(n+2); a.fill(-1);
        arma::vec b = arma::vec(n+2); b.fill(2);
        arma::vec c = arma::vec(n+2); c.fill(-1);

        auto startWallTime = std::chrono::high_resolution_clock::now();
        auto startCPUTime  = std::clock();
        arma::vec solution = thomas(a, b, c, btilde);
        auto CPUTime = (std::clock() - startCPUTime)/static_cast<double>(CLOCKS_PER_SEC);
        std::chrono::duration<double> wallTime = (std::chrono::high_resolution_clock::now() - startWallTime);
        std::cout << "Wall time: " << wallTime.count() << "s\n"
                  << "CPU time:  " << CPUTime << "s" << std::endl;

        // Save result for further analysis
        std::stringstream name;
        name << "n" << n << ".txt";
        solution.save(name.str(), arma::raw_ascii);
    }
}

void solveLU(double (*func)(double), const unsigned int low, const unsigned int high, const unsigned int step){
    for (unsigned int n = low; n <= high; n *= step) {
        double h = 1/(static_cast<double>(n+1));
        std::cout << "Solving " << n << "×" << n << " with stepsize " << h
                  << " using LU decomposition" << std::endl;

        arma::vec btilde = h*arma::linspace(0, n+2, n+2);
        btilde.transform(func);
        btilde *= h*h; // b~ = f(x)h^2

        arma::mat A = tridiagonalMat(n+2, -1, 2, -1);
        arma::mat L, U;
        arma::vec x, y;
        // Ax = b -> LUx = b -> Ly = b -> Ux = y
        arma::lu(L,U,A);
        y = arma::solve(L, btilde);
        x = arma::solve(U, y);

        std::stringstream name;
        name << "n" << n << "_LU.txt";
        x.save(name.str(), arma::raw_ascii);
    }
}

arma::vec thomas(const arma::vec& a, const arma::vec& b, const arma::vec& c, const arma::vec& f){
    /* Implementation of Thomas Algorithm as described p. 186*/
    if(arma::numel(a) != arma::numel(b) ||
       arma::numel(b) != arma::numel(c) ||
       arma::numel(c) != arma::numel(f))
        throw std::runtime_error("Input vectors are of different length");

    const size_t n   = arma::numel(a);
    arma::vec tmp    = arma::zeros(n);
    arma::vec u      = arma::zeros(n);

    // Forward sweep
    double btmp = b[1];
    u[1]        = f[1]/btmp;
    for(unsigned int i = 2; i <= n-2; i++){
        tmp[i] = c[i-1]/btmp;
        btmp   = b[i]-a[i]*tmp[i];
        u[i]   = (f[i]-a[i]*u[i-1])/btmp;
    }

    // Backward substitution
    for(unsigned int i = n-3; i > 0; i--)
        u[i] -= tmp[i+1]*u[i+1];

    return u;
}



arma::mat tridiagonalMat(unsigned int size, double upper, double middle, double lower){
    auto mat = arma::mat(size, size, arma::fill::zeros);
    for (unsigned int row = 0; row < size; row++){
        if (row > 0)
            mat(row, row-1) = lower;
        mat(row, row) = middle;
        if (row < size-1)
            mat(row, row+1) = upper;
    }
    return mat;
}
