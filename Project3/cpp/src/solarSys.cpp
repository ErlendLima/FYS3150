#include <string>
#include "solarSys.h"

void SolarSys::add(std::string name, double M, vec3 pos, vec3 vel, unsigned int n)
{
  std::shared_ptr<Planet> planet = std::make_shared<Planet>(name, M, pos, vel,n);
  add(planet);
  n_planets++;
}

void SolarSys::updateTotalEnergy(){
  totalEnergy = 0.0;
  for(auto & planet: planets){
    totalEnergy += planet->kineticEnergy();
  }
  totalEnergy += potentialEnergy();
}

double SolarSys::potentialEnergy(){
  double potTot = 0.0;
  for(auto & planet: planets){
    for(auto & other: planets){
      if(planet == other)continue;
      potTot += planet->potentialEnergy(*other);
    }
  }
  return 0.0;
}

