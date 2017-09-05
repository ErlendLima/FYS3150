#include <iostream>
#include <cmath>
#include "solver.h"
#include <memory.h>


int main(int argc, char *argv[])
{
    int retval = 0;
    // Using f(x) = 100exp(-10x)
    std::function<double(double)> func = [](double x){return 100*exp(-10*x);};
    std::function<double(double)> funcAnalytic = [](double x){return 1-(1-exp(-10)*x) - exp(-10*x);};
    Solver solver(func);
    solver.setAnalytical(funcAnalytic);
    solver.setSavepath("data/");
    solver.setRepetitions(1e0);
    try{
        // retval = solver.solve(Method::LU, 10, 1e3, 10);
        retval = solver.solve(Method::GENERAL, 10, 1e6, 10);
        // retval = solver.solve(Method::SPECIAL, 10, 1e6, 10);
        solver.calculateError(1e1, 1e6, 1e2);
    } catch (std::exception &ex){
        std::cerr << "Error: " << ex.what() << std::endl;
        return -1;
    }
    return retval;
}
