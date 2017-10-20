#ifndef SOLVER_H
#define SOLVER_H
#include <chrono>
#include "solarSys.h"

enum class Method{EULER, EULERCROMER, VERLET};
class Solver
{
public:
  Solver(){saveFlag = true;};
  virtual ~Solver(){};

  int solve(Method, unsigned int N, double dt);
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

  bool saveFlag = true;
  std::string savepath = "../data";

  void startTiming();
  void endTiming();
  std::chrono::high_resolution_clock::time_point startWallTime;
  clock_t startCPUTime;
};

#endif /* SOLVER_H */
