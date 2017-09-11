#include <iostream>
#include <cmath>
#include <memory.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "solver.h"


int handleArguments(int argc, char *argv[], Method& method, int& start, int& stop, int& step, bool& flag, int& repetitions);

int main(int argc, char *argv[])
{
    int retval = 0;
    // Using f(x) = 100exp(-10x)
    std::function<double(double)> func = [](double x){return 100*exp(-10*x);};
    std::function<double(double)> funcAnalytic = [](double x){return 1-(1-exp(-10))*x - exp(-10*x);};

    Solver solver(func);
    solver.setAnalytical(funcAnalytic);
    solver.setSavepath("data/");
    Method method = Method::GENERAL;
    int start = 10; int stop = 1e5; int step = 10;
    bool calcError = false; int repetitions = 1;
    retval = handleArguments(argc, argv, method, start, stop, step, calcError, repetitions);
    if (retval < 0)
        return retval;

    solver.setRepetitions(repetitions);

    try{
        if (calcError)
            solver.calculateError(1e6, 1e7);
        else
            retval = solver.solve(method, start, stop, step);
    } catch (std::exception &ex){
        std::cerr << "Error: " << ex.what() << std::endl;
        return -1;
    }
    return retval;
}

int handleArguments(int argc, char *argv[], Method& method, int& start, int& stop, int& step, bool& flag, int& repetitions){
    opterr = 0;
    int c;

    while (( c = getopt(argc, argv, "glser:hX:Y:Z:")) != EOF ) {
        switch (c) {
        case 'g':
            method = Method::GENERAL;
            start = 10; stop = 1e5; step = 10;
            break;
        case 'l':
            method = Method::LU;
            start = 10; stop = 1e3; step = 10;
            break;
        case 's':
            method = Method::SPECIAL;
            start = 10; stop = 1e5; step = 10;
            break;
        case 'e':
            flag = true;
            break;
        case 'r':
            repetitions = std::atoi(optarg);
            break;
        case 'X': start = std::atoi(optarg); break;
        case 'Y': stop = std::atoi(optarg); break;
        case 'Z': step = std::atoi(optarg); break;
        case 'h':
            std::cout << "Usage: -g for general, -s for special, -l for LU, -e for error calculation and -r N for number of repetitions." << std::endl;
            return -1;
        default:
            abort();
            break;
        }
    }
    return 0;
}
