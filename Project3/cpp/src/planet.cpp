#include "planet.h"
#include "Vec3/vec3.h"

Planet::Planet(){
  mass      = 1.;
  pos[0]    = 1.;
  pos[1]    = 0.;
  pos[2]    = 0.;
  vel[0]    = 0.;
  vel[1]    = 0.;
  vel[2]    = 0.;
  potential = 0.;
  kinetic   = 0.;
}

Planet::Planet(double M, double x0, double y0, double z0, double vx0, double vy0, double vz0){
  mass = M;
  pos[0] = x0;
  pos[1] = y0;
  pos[2] = z0;
  vel[0] = vx0;
  vel[1] = vy0;
  vel[2] = vz0;
  potential = 0.;
  kinetic = 0.;
}

double Planet::distance(Planet otherPlanet){
  vec3 diff;
  diff = otherPlanet.pos - pos;

  return diff.length();
}

double Planet::gravitationalForce(Planet otherPlanet, double G){
  double r = distance(otherPlanet);
  if(r!=0) return G*mass*otherPlanet.mass/(r*r);
  else return 0;
}

double Planet::acceleration(Planet otherPlanet, double G){
  double r = distance(otherPlanet);
  if(r!=0) return gravitationalForce(otherPlanet,G)/(mass); //TODO: IS THIS RIGHT?
  else return 0;
}

void Planet::resetAcc(){
  acc[0] = 0.0;
  acc[1] = 0.0;
  acc[2] = 0.0;
}
