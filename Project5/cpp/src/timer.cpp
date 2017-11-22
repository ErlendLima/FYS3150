#include <ctime>
#include <chrono>

void Timer::start(){
  // Save time points at startup (both wall time and cpu time)
  startWallTime = std::chrono::high_resolution_clock::now();
  startCPUTime  = std::clock();
}

void Timer::print(){
  // Print wall time and cpu time when called
  auto                          CPUTime  = CPUTime();
  std::chrono::duration<double> wallTime = wallTime();

  std::cout << "Wall time: " << wallTime.count() << "s\n"
            << "CPU time: " << CPUTime << "s" << std::endl;
}

std::chrono::high_resolution_clock::duration Timer::wallTime(){
  return (std::chrono::high_resolution_clock::now() - startWallTime);
}

clock_t Timer::CPUTime(){
  return (std::clock() - startCPUTime)/static_cast<double>(CLOCKS_PER_SEC);
}
