#ifndef SOLARSYS_H
#define SOLARSYS_H

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include "planet.h"
#include "Vec3/vec3.h"

#define pi 3.141592653589793238462643
#define G 39.478417604357434475337963

// class Planet;

class SolarSys
{
public:
  SolarSys(){};
  virtual ~SolarSys(){};
  // Add planet to solar system. Can take pointer or args.
  void add(std::shared_ptr<Planet> planet) {planets.push_back(planet);};
  void add(std::string, double M, vec3 pos, vec3 vel, unsigned int n);
  std::vector<std::shared_ptr<Planet>> planets; // Pointers to planet instance
  unsigned int n_planets = 0;

  void updateTotalEnergy();
  double getTotalEnergy(){return totalEnergy;};
  double potentialEnergy();

private:
  double totalEnergy;
};

#endif /* SOLARSYS_H */
