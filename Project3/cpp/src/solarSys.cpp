#include "solarSys.h"
#include <string>

void SolarSys::add(std::string name, double M, vec3 pos, vec3 vel, unsigned int n)
{
  std::shared_ptr<Planet> planet = std::make_shared<Planet>(name, M, pos, vel,n);
  add(planet);
  n_planets++;
}
