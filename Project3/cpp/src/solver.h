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
  // I vårt tilfelle, nei, vi trenger ikke en destructor, siden vi bare bruker minnetrygg kode.
  // Hvis vi brukte manuell minneallokering uten en god destructor og objektene "døde"
  // i programmets levetid, hadde programmet "reservert" mer og mer minne av operativsystemet
  // sidene kadaverene fra de døde objektene fortsatt lå i minnet. 
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

  SolarSys sys;

  void startTiming();
  void endTiming();
};

#endif /* SOLVER_H */
