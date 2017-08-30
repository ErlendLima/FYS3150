#include <iostream>
#include <cmath>
#include "solve.h"


int main(int argc, char *argv[])
{
    // Using f(x) = 100exp(-10x)
    double (*func)(double) = [](double x){return 100*exp(-10*x);};

    try{
        solveGeneral(func, 10, 1e6, 10, 3, -1);
        // solveLU(func, 10, 1e3, 10);
    } catch (std::exception &ex){
        std::cerr << "Error: " << ex.what() << std::endl;
        return -1;
    }
    return 0;
}


