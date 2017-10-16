#include "solarSys.h"

void SolarSys::add(double M, double x0, double y0, double z0,
                       double vx0, double vy0, double vz0)
{
  std::shared_ptr<Planet> planet = std::make_shared<Planet>(M, x0, y0, z0, vx0, vy0, vz0);
  add(planet);
}
