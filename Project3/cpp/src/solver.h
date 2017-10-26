#ifndef SOLVER_H
#define SOLVER_H
#include <chrono>
#include <jsoncpp/json/json.h>
#include "solarSys.h"

enum class Method{EULER, EULERCROMER, VERLET};
class Solver
{
public:
    Solver(const std::string& parameterpath);
    virtual ~Solver(){};

    int solve();
    void solveVerlet();
    // void solveEuler();
    void solveSystem(std::function<void(std::shared_ptr<Planet>)>&,
                     std::function<void(std::shared_ptr<Planet>)>&);
    void doSave(bool flag){saveFlag = flag;};
    void setSavepath(const std::string& path){savepath = path;}
    void updateForces();
    void initSystem();
    void saveToFile();

private:
    SolarSys sys;

    unsigned int n; // Number of steps
    double dt;
    void EulerStep(std::shared_ptr<Planet>);
    void VerletStep1(std::shared_ptr<Planet>);
    void VerletStep2(std::shared_ptr<Planet>);
    void ECStep(std::shared_ptr<Planet>);
    void readParameters(const std::string& filename);
    bool usePlanet(const std::string& planet) const;
    void startTiming();
    void endTiming();
    void updateEnergy(unsigned int n);
    void updateAngMom(unsigned int n);
    void nop(const std::shared_ptr<Planet>) const{};

    bool saveFlag = true;
    std::string savepath = "../data";
    Method method;
    Json::Value root;
    Json::Value planets_to_use;
    bool use_all_planets = true;
    bool freezeSun = false;
    bool twoBodyApproximation = false;
    double gravitationalExponent = 2;

    arma::mat angMomArray;
    arma::mat energyArray;
    std::chrono::high_resolution_clock::time_point startWallTime;
    clock_t startCPUTime;
};

#endif /* SOLVER_H */
