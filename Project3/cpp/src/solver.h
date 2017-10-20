#ifndef SOLVER_H
#define SOLVER_H
#include <chrono>
#include <json/json.h>
#include "solarSys.h"

enum class Method{EULER, EULERCROMER, VERLET};
class Solver
{
public:
    Solver(const std::string& parameterpath);
  virtual ~Solver(){};

  int solve();
  void solveSystem(std::function<void(std::shared_ptr<Planet>)>&);
  void solveSystemVV();
  void doSave(bool flag){saveFlag = flag;};
  void setSavepath(const std::string& path){savepath = path;}
  void updateForces();
  void initSystem();
  void saveToFile();

private:
  SolarSys sys;

  unsigned int n; // Antall steg
  double dt;
  void EulerStep(std::shared_ptr<Planet>);
  void VerletStep1(std::shared_ptr<Planet>);
  void VerletStep2(std::shared_ptr<Planet>);
  void ECStep(std::shared_ptr<Planet>);
    void readParameters(const std::string& filename);
    bool usePlanet(const std::string& planet);

  bool saveFlag = true;
  std::string savepath = "../data";
    Method method;
    Json::Value root;
    Json::Value planets_to_use;
    bool use_all_planets;

  void startTiming();
  void endTiming();
  std::chrono::high_resolution_clock::time_point startWallTime;
  clock_t startCPUTime;
};

#endif /* SOLVER_H */
