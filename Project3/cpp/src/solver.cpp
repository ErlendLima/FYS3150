#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cmath>
#include <memory>
#include <omp.h>
#include <algorithm>
#include <armadillo>
#include <jsoncpp/json/json.h>
#include "solver.h"
#include "solarSys.h"
#include "planet.h"
#include "Vec3/vec3.h"

#define G 39.478417604357434475337963

using std::placeholders::_1;

Solver::Solver(const std::string& parameterpath){
    readParameters(parameterpath);
}

int Solver::solve(){
  initSystem();

  // Holds step function for the chosen method
  std::function<void(std::shared_ptr<Planet>)> stepper;
  switch (method){
    case Method::EULER:
      std::cout << "=== Simulating system with Euler's method ===" << std::endl;
      stepper = std::bind(&Solver::EulerStep, this, _1);
      solveSystem(stepper);
      break;
    case Method::VERLET:
      std::cout << "=== Simulating system with Velocity Verlet method ===" << std::endl;
      solveSystemVV();
      break;
    case Method::EULERCROMER:
      std::cout << "=== Simulating system with Euler-Cromer's method ===" << std::endl;
      stepper = std::bind(&Solver::ECStep, this, _1);
      break;
    default:
      std::cout << "=== NO METHOD CHOSEN ===" << std::endl;
      return -1;
    }
    if(saveFlag) saveToFile();
    return 0;
}

void Solver::solveSystemVV(){
    startTiming();
    updateForces();
    updateEnergy(0);
    double progress = 0.1;
    for(unsigned int i = 1; i <= n; i++){

        for(auto & planet: sys.planets)
            VerletStep1(planet);

        updateForces();
        for(auto & planet: sys.planets){
            VerletStep2(planet);
            planet->writePosToMat(i);
        }

        // Write out the progress for each 10% increment
        if(i/static_cast<double>(n) >= progress){
            std::cout << 100*progress << "% completed" << std::endl;
            progress += 0.1;
        }
        updateEnergy(i);
    }
    endTiming();
}

void Solver::solveSystem(std::function<void(std::shared_ptr<Planet>)>& stepper){
    startTiming();
    updateEnergy(0);
    // Loop over time
    for(unsigned int i = 1; i <= n; i++){
        // Loop over every planet to find acceleration of each planet
        updateForces();
        // Forward planet positions in time with method of choice
        for(auto & planet: sys.planets){
            stepper(planet);
            planet->writePosToMat(i);
        }
        updateEnergy(i);
    }
    endTiming();
}

void Solver::updateForces(){
    for(unsigned int i = 0; i < sys.planets.size(); i++){
        if(freezeSun && sys.planets[i]->name == "Sun") continue;
        sys.planets[i]->acc_prev = sys.planets[i]->acc;
        sys.planets[i]->resetAcc();
        sys.planets[i]->resetF();

        // Loop over all the other planets
        for(auto & other: sys.planets){
            if(sys.planets[i] == other)continue;
            else{
                sys.planets[i]->calculateAcc(*other);
            }
        }
    }
}


void Solver::readParameters(const std::string& filename){
    // Load the JSON file and read the relevant parameters
    std::ifstream parameters(filename);
    parameters >> root;
    std::string smethod     = root["method"].asString();
    unsigned int N_per_year = root["steps per year"].asInt();
    unsigned int num_years  = root["number of years"].asInt();
    saveFlag                = root["do save results"].asBool();
    use_all_planets         = root["use all planets"].asBool();
    freezeSun               = root["freeze sun"].asBool();
    twoBodyApproximation    = root["use two body approximation"].asBool();
    planets_to_use          = root["use planets"];

    // Act on the read parameters
    n = num_years*N_per_year;
    dt = 1.0/N_per_year;

    std::transform(smethod.begin(), smethod.end(), smethod.begin(), ::tolower);
    if(smethod == "euler")
        method = Method::EULER;
    else if(smethod == "verlet")
        method = Method::VERLET;
    else
        throw std::runtime_error("Unknown method");

}

void Solver::initSystem(){
    auto planets = root["planets"];
    for(unsigned int i = 0; i < planets.size(); i ++){
        if(!usePlanet(planets[i]["name"].asString()))
            continue;

        sys.add(planets[i]["name"].asString(),
                planets[i]["mass"].asDouble(),
                vec3(planets[i]["position"][0].asDouble(),
                     planets[i]["position"][1].asDouble(),
                     planets[i]["position"][2].asDouble()),
                vec3(planets[i]["velocity"][0].asDouble(),
                     planets[i]["velocity"][1].asDouble(),
                     planets[i]["velocity"][2].asDouble()),
                n);
    }
    if(sys.planets.size() <= 0)
        throw std::runtime_error("No planets found during initialization");

    std::string using_planets = "Using planets ";
    for(auto &planet: sys.planets)
        using_planets += planet->name + ", ";
    std::cout << using_planets + "\r\r." << std::endl;

    // Set up the energy array with the format
    // time - kinetic energy - potential energy
    energyArray = arma::zeros(3, n+1);
}

bool Solver::usePlanet(const std::string& planet_name) const{
    if(use_all_planets)
        return true;
    for(unsigned int i = 0; i < planets_to_use.size(); i++){
        if(planet_name == planets_to_use[i].asString())
            return true;
    }
    return false;
}

void Solver::EulerStep(std::shared_ptr<Planet> planet){
  planet->vel_tmp = planet->vel;   // Save velocity temporarily for Euler
  planet->vel    += planet->acc*dt;
  planet->pos    += planet->vel_tmp*dt;
}

void Solver::VerletStep1(std::shared_ptr<Planet> planet){
  planet->pos += planet->vel*dt + 0.5*dt*dt*planet->acc;
}

void Solver::VerletStep2(std::shared_ptr<Planet> planet){
  planet->vel += 0.5*dt*(planet->acc + planet->acc_prev);
}

void Solver::ECStep(std::shared_ptr<Planet> planet){
  planet->vel += planet->acc*dt;
  planet->pos += planet->vel*dt;
}

void Solver::updateEnergy(unsigned int step){
    energyArray(0, step) = step*dt;
    energyArray(1, step) = sys.kineticEnergy();
    energyArray(2, step) = sys.potentialEnergy();
}

void Solver::saveToFile(){
    // Save the position
    std::ofstream positionstream;
    positionstream.open(savepath + "/position.txt");
    // Loop over time points
    for(unsigned int i = 0; i < n; i++){
        // Loop over planets
        unsigned int j = 0;
        for(auto & planet: sys.planets){
            positionstream << planet->pos_array(0, i) << " ";
            positionstream << planet->pos_array(1, i) << " ";
            positionstream << planet->pos_array(2, i) << " ";
            if(j == sys.n_planets-1){
                positionstream << "\n";}
            j++;
        }
    }
    positionstream.close();

    // Save the energy
    energyArray.save(savepath + "/energy.txt", arma::raw_ascii);
}

void Solver::startTiming(){
  startWallTime = std::chrono::high_resolution_clock::now();
  startCPUTime  = std::clock();
}

void Solver::endTiming(){
  auto CPUTime = (std::clock() - startCPUTime)/static_cast<double>(CLOCKS_PER_SEC);
  std::chrono::duration<double> wallTime = (std::chrono::high_resolution_clock::now() - startWallTime);
  std::cout << "Wall time: " << wallTime.count() << "s\n"
            << "CPU time: " << CPUTime << "s" << std::endl;
}
