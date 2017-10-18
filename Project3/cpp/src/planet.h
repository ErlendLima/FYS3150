#ifndef PLANET_H
#define PLANET_H
#include <cmath>
#include <vector>
#include "Vec3/vec3.h"
using std::vector;

class Planet
{
public:
  // Constructors
  Planet();
  Planet(double M, double x0, double y0, double z0, double vx0, double vy0, double vz0);

  // Methods
  double distance(Planet otherPlanet);
  double gravitationalForce(Planet otherPlanet, double G);
  double acceleration(Planet otherPlanet, double G);
  double kineticEnergy(){return kinetic;};
  double potentialEnergy(Planet &otherPlanet, double G, double eps);
  void resetAcc();
  double mass;
  vec3 pos;
  vec3 vel;
  vec3 acc;

private:
  double potential;
  double kinetic;
};

#endif // PLANET_H
