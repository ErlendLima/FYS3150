#include <iostream>
#include <cmath>
#include "solver.h"


int main(int argc, char *argv[])
{
    // Using f(x) = 100exp(-10x)
    double (*func)(double) = [](double x){return 100*exp(-10*x);};
    Solver solver(func);
    solver.setSavepath("data/");
    solver.setRepetitions(1e0);
    try{
        // solver.solve(Method::GENERAL, 10, 1e6, 10);
        // solver.solve(Method::SPECIAL, 10, 1e6, 10);
        solver.calculateError(10, 100, 10);
    } catch (std::exception &ex){
        std::cerr << "Error: " << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
