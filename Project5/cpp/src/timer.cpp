#include <iostream>
#include <ctime>
#include <chrono>
#include "timer.h"

void Timer::start(){
  // Save time points at startup (both wall time and cpu time)
    startWallTime = std::chrono::high_resolution_clock::now();
    startCPUTime  = std::clock();
}

void Timer::print(){
    // Print wall time and cpu time when called
    std::cout << "Wall time: " << wallTime() << "s\n"
              << "CPU time : " << CPUTime()  << "s"   << std::endl;
}

double Timer::wallTime(){
  std::chrono::duration<double> wTime = (std::chrono::high_resolution_clock::now() - startWallTime);
  return wTime.count();
}

double Timer::CPUTime(){
  return (std::clock() - startCPUTime)/static_cast<double>(CLOCKS_PER_SEC);
}
