#ifndef PLANET_H
#define PLANET_H
#include <cmath>
#include <vector>
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

private:
  double mass;
  double pos[3];
  double vel[3];
  double potential;
  double kinetic;
};

#endif // PLANET_H
