#include "planet.h"

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
  double x1,y1,z1, x2,y2,z2, xx,yy,zz;

  x1 = pos[0];
  y1 = pos[1];
  z1 = pos[2];

  x2 = otherPlanet.position[0];
  y2 = otherPlanet.position[1];
  z2 = otherPlanet.position[2];

  xx = x1-x2;
  yy = y1-y2;
  zz = z1-z2;

  return sqrt(xx*xx + yy*yy + zz*zz);
}

double Planet::gravitationalForce(Planet otherPlanet, double G){
  double r = distance(otherPlanet);
  if(r!=0) return G*mass*otherPlanet.mass/(r*r);
  else return 0;
}

double Planet::acceleration(Planet otherPlanet, double G){
  double r = distance(otherPlanet);
  if(r!=0) return gravitationalForce(otherPlanet,G)/(mass*r) //TODO: IS THIS RIGHT?
}
