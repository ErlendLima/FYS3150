#include <iostream>
#include <cmath>
#include <memory.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <armadillo>
#include "jacobi.h"
#include "schrodinger.h"

int handleArguments(int argc, char *argv[], Method& method);

int main(int argc, char *argv[]) {

    Method method = Method::JACOBI;

    int retval = handleArguments(argc, argv, method);
    if (retval < 0)
        return retval;
    std::string filename = (method == Method::JACOBI) ? "data/timings_jacobi.txt" : "data/timings_arma.txt";
    arma::vec timings = arma::vec(40);

    for(unsigned int i = 1; i <= 40; i++){
        timings(i-1) = solve(1e-4, 10, i*5, method);
    }
    timings.save(filename, arma::raw_ascii);
    return 0;
}


int handleArguments(int argc, char *argv[], Method& method){
    opterr = 0;
    int c;

    while (( c = getopt(argc, argv, "ah?")) != EOF ) {
        switch (c) {
        case 'a':
            method = Method::ARMA;
            break;
        case 'h':
        case '?':
            std::cout << "Usage: solve [-a|-h|-?]\n"
                      << "-a     uses armadillo's eigenvalue solver.\n"
                      << "-h-?   shows this help message." << std::endl;
            return -1;
        default:
            abort();
            break;
        }
    }
    return 0;
}
