#ifndef SOLVER_H
#define SOLVER_H
#include "metamodel.h"

class Solver
{
public:
    Solver(Metamodel& metamodel);
    virtual ~Solver(){};

    int solve();
    void forwardStep(double alpha, arma::mat& u, unsigned int t) const;
    void forwardEuler(double alpha, arma::mat& u) const;
    void backwardEuler(double alpha, arma::mat& u) const;
    void crankNicolson(double alpha, arma::mat& u) const;
    void tridiag(double alpha, arma::vec& u, unsigned int N);

private:
    unsigned int xsteps, ysteps, tsteps;
    Metamodel model;
};

#endif /* SOLVER_H */
