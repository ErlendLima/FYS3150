#ifndef SOLARSYS_H
#define SOLARSYS_H

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include "planet.h"

#define pi 3.141592653589793238462643383279502884197169

// class Planet;

class SolarSys
{
public:
  SolarSys(){};
  virtual ~SolarSys(){};
  // Add planet to solar system. Can take pointer or args.
  void add(std::shared_ptr<Planet> planet) {planets.push_back(planet);};
  void add(double M, double x0, double y0, double z0,
                    double vx0, double vy0, double vz0);
  std::vector<std::shared_ptr<Planet>> planets; // Pointers to planet instances
private:
  double time    = 0.0;
  const double G = 4*pi*pi;
};

#endif /* SOLARSYS_H */
