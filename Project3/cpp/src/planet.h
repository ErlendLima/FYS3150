#ifndef PLANET_H
#define PLANET_H
#include <cmath>
#include <vector>
#include <string>
#include <armadillo>
#include "Vec3/vec3.h"
using std::vector;

class Planet
{
public:
  // Constructors
  Planet(const std::string&, double M, vec3 pos0, vec3 vel0, unsigned int n);

  // Methods
  double distance(const Planet& other) const;
  double gravitationalForce(const Planet& other) const;
  void calculateAcc(const Planet& other);
  void calculateAccRelativistic(const Planet& other);
  void force(const Planet& other);
  void relativisticForce(const Planet& other);
  double kineticEnergy() const {return 0.5*mass*pow(vel.length(),2);};
  double potentialEnergy(const Planet& other) const;
  double totalEnergy();
  void resetAcc();
  void resetF();
  void writePosToMat(unsigned int);
  // Vars
  const std::string name;
  double mass;
  vec3 pos;
  vec3 vel;
  vec3 vel_tmp;
  vec3 acc;
  vec3 acc_prev;
  vec3 F;
  arma::mat pos_array;

private:
  double potential = 0;
  double kinetic   = 0;
};

#endif // PLANET_H
