#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <ctime>

using duration  = std::chrono::high_resolution_clock::duration;
using timePoint = std::chrono::high_resolution_clock::time_point;

class Timer
{
public:
  Timer(){};

  void     start();
  void     print();
  duration wallTime();
  clock_t  CPUTime();

private:
  timePoint startWallTime;
  clock_t   startCPUTime;
};

#endif /* TIMER_H */
