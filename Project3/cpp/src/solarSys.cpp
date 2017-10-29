#include <string>
#include "solarSys.h"

void SolarSys::add(std::string name, double M, vec3 pos, vec3 vel, unsigned int n)
{
  std::shared_ptr<Planet> planet = std::make_shared<Planet>(name, M, pos, vel,n);
  add(planet);
  n_planets++;
}

double SolarSys::kineticEnergy() const{
    double energy = 0.0;
    for(auto & planet: planets){
        energy += planet->kineticEnergy();
    }
    return energy;
}

double SolarSys::potentialEnergy() const{
  double energy = 0.0;
  for(auto & planet: planets){
    for(auto & other: planets){
      if(planet == other)continue;
      energy += planet->potentialEnergy(*other)/2;
    }
  }
  return energy;
}

vec3 SolarSys::angularMomentum() const{
  // Returns total angular momentum in the system
  vec3 angmom = 0.0;
  for(auto & planet: planets){
    angmom += planet->angularMomentum(COM);
  }
  return angmom;
}

void SolarSys::updateCOM(){
  COM = 0.0; // Reset first
  double totalMass = 0.0;
  for(auto & planet: planets){
    COM += planet->pos * planet->mass;
    totalMass += planet->mass;
  }
  COM /= totalMass;
}

void SolarSys::sort(){
    // Place the sun in the initial position of the array
    auto temp = planets;
    unsigned int i = 1;
    for (auto& planet: planets){
        if (planet->name == "Sun")
            temp[0] = planet;
        else{
            temp[i] = planet;
            i++;
        }
    }
    planets = temp;
}
