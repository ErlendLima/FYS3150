#ifndef SOLVER_H
#define SOLVER_H
#include <chrono>
#include "metamodel.h"

enum class Method{FORWARD_EULER, BACKWARD_EULER, CRANK_NICOLSON};
class Solver
{
public:
    Solver(Metamodel& metamodel);
    virtual ~Solver(){};

    int solve();
    void forwardStep(double alpha, arma::mat& u, arma::mat& uPrev) const;
    void forwardEuler(double alpha, arma::mat& u) const;
    void backwardEuler(double alpha, arma::mat& u) const;
    void crankNicolson(double alpha, arma::mat& u) const;

    void solveSystem(std::function<void(double, arma::mat)>&);
    void initSystem();
    void saveToFile();

private:
    unsigned int xsteps, ysteps, tsteps;
    Metamodel model;
    void startTiming();
    void endTiming();


    std::chrono::high_resolution_clock::time_point startWallTime;
    clock_t startCPUTime;
};

#endif /* SOLVER_H */
