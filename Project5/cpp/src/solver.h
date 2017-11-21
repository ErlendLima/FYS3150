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
    void solveVerlet();
    // void solveEuler();
    void solveSystem(std::function<void(std::shared_ptr<Planet>)>&)
    void updateForces();
    void initSystem();
    void saveToFile();

private:
    unsigned int n; // Number of steps
    double dt, dx, dy;
    Metamodel model;
    void EulerStep(std::shared_ptr<Planet>);
    void VerletStep1(std::shared_ptr<Planet>);
    void VerletStep2(std::shared_ptr<Planet>);
    void ECStep(std::shared_ptr<Planet>);
    void startTiming();
    void endTiming();


    std::chrono::high_resolution_clock::time_point startWallTime;
    clock_t startCPUTime;
};

#endif /* SOLVER_H */
