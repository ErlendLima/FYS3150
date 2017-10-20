#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cmath>
#include <memory>
#include <algorithm>
#include <armadillo>
#include <json/json.h>
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
    for(unsigned int i = 1; i <= n; i++){
        for(auto & planet: sys.planets)
            VerletStep1(planet);

        updateForces();
        for(auto & planet: sys.planets){
            VerletStep2(planet);
            planet->writePosToMat(i);
        }
    sys.updateTotalEnergy();
    std::cout << sys.getTotalEnergy() << std::endl;
    }
    endTiming();
}

void Solver::solveSystem(std::function<void(std::shared_ptr<Planet>)>& stepper){
  startTiming();
  // Loop over time
  for(unsigned int i = 1; i <= n; i++){
    // Loop over every planet to find acceleration of each planet
      updateForces();
    // Forward planet positions in time with method of choice
    for(auto & planet: sys.planets){
      stepper(planet);
      planet->writePosToMat(i);
    }
    sys.updateTotalEnergy();
    std::cout << sys.getTotalEnergy() << std::endl;
  }
  endTiming();
}

void Solver::updateForces(){
    for(auto & planet: sys.planets){
        if(planet->name == "Sun") continue;
        planet->acc_prev = planet->acc;
        planet->resetAcc();
        planet->resetF();

        // Loop over all the other planets
        for(auto & other: sys.planets){
            if(planet == other)continue;
            else{
                planet->calculateAcc(*other);
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
    std::cout << using_planets << std::endl;
}

bool Solver::usePlanet(const std::string& planet_name){
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


void Solver::saveToFile(){
  std::ofstream myfile;
  myfile.open(savepath + "/cpp.txt");
  // Loop over time points
  for(unsigned int i = 0; i < n; i++){
    // Loop over planets
    unsigned int j = 0;
    for(auto & planet: sys.planets){
      myfile << planet->pos_array(0,i) << " ";
      myfile << planet->pos_array(1,i) << " ";
      myfile << planet->pos_array(2,i) << " ";
      if(j == sys.n_planets-1){
        myfile << "\n";}
      j++;
    }
  }
  myfile.close();
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
