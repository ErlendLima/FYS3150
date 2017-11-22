#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
  Timer(){};
  start();
  print();
  std::chrono::high_resolution_clock::duration wallTime();
  clock_t CPUTime();

private:
  std::chrono::high_resolution_clock::time_point startWallTime;
  clock_t startCPUTime;
};

#endif /* TIMER_H /*
