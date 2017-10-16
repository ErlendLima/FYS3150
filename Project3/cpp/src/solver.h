#ifndef SOLVER_H
#define SOLVER_H
#include <chrono>

enum class Method{EULER, VERLET};
class Solver
{
public:
  Solver();
  // virtual ~Solver();
  // Trenger man destructor her? Objekt blir vel slettet nar prog termineres uansett

  int solve(Method, unsigned int time, unsigned int N);
  void solveEuler();
  void solveVerlet();
  void loadSystem();
  void doSave(bool flag){saveFlag = flag;};
  void setSavepath(const std::string& path){savepath = path;}

private:
  bool saveFlag = true;
  std::string savepath = "";

  std::chrono::high_resolution_clock::time_point startWallTime;
  clock_t startCPUTime;

  void startTiming();
  void endTiming();
};

#endif /* SOLVER_H */
