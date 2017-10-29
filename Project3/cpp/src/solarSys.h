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

class SolarSys
{
public:
    SolarSys(){};
    virtual ~SolarSys(){};
    // Add planet to solar system. Can take pointer or arguments
    void add(std::shared_ptr<Planet> planet) {planets.push_back(planet);};
    void add(std::string, double M, vec3 pos, vec3 vel, unsigned int n);

    std::vector<std::shared_ptr<Planet>> planets; // Pointers to planet instance
    unsigned int n_planets = 0;

    void updateTotalEnergy();
    double potentialEnergy() const;
    double kineticEnergy() const;
    vec3 angularMomentum() const;
    void readParameters(const std::string& filename);
    void sort();
    vec3 COM; // Center of mass of the system
    void updateCOM();

private:
};

#endif /* SOLARSYS_H */
