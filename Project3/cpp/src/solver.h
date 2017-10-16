#ifndef SOLVER_H
#define SOLVER_H
#include <chrono>
#include "solarSys.h"

enum class Method{EULER, VERLET};
class Solver
{
public:
  Solver(){saveFlag = false;};
  virtual ~Solver(){};
  // Trenger man destructor her? Objekt blir vel slettet nar prog termineres uansett

  int solve(Method, unsigned int N, double dt);
  void solveEuler(unsigned int N, double dt);
  void solveVerlet(unsigned int N, double dt);
  void doSave(bool flag){saveFlag = flag;};
  void setSavepath(const std::string& path){savepath = path;}
  void initSystem();

private:
  bool saveFlag = true;
  std::string savepath = "";

  std::chrono::high_resolution_clock::time_point startWallTime;
  clock_t startCPUTime;

  void startTiming();
  void endTiming();
};

#endif /* SOLVER_H */
