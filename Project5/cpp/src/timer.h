#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <ctime>

class Timer
{
public:
  Timer(){};

  void   start();
  void   print();
  double wallTime();
  double CPUTime();

private:
  std::chrono::high_resolution_clock::time_point startWallTime;
  clock_t   startCPUTime;
};

#endif /* TIMER_H */
