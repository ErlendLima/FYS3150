#include <json/json.h>
#include <json/writer.h>
#include <iostream>
#include <fstream>
#include "metamodel.h"

void writeMetaData(metamodel& model){
  std::ofstream metafile("../data/meta.json");
  Json::Value root;
  root["evolution"]["dim"]  = Json::arrayValue;
  root["evolution"]["type"] = "int64";
  root["evolution"]["path"] = "evolution.bin";
  root["evolution"]["dim"].append(model.M);
  root["evolution"]["dim"].append(model.N);
  root["evolution"]["dim"].append(model.N);


  root["magnetic moment"]["dim"]  = Json::arrayValue;
  root["magnetic moment"]["type"] = "int32";
  root["magnetic moment"]["path"] = "magneticmoment.bin";
  root["magnetic moment"]["dim"].append(model.M);

  root["energy"]["dim"]  = Json::arrayValue;
  root["energy"]["type"] = "float64";
  root["energy"]["path"] = "energies.bin";
  root["energy"]["dim"].append(model.M);

  root["saveperiod"]    = 1000;
  root["seed"]          = model.seed;
  root["lattice size"]  = model.N;
  root["MC iterations"] = model.M;
  metafile << root << std::endl;
  metafile.close();
  //TODO: ENDRE SAVEPERIOD TIL NUMBER OF SAVES
}

void save(metamodel& model, std::vector<arma::imat>& states, std::vector<double>& energies, std::vector<int>& magmoments){
  // Write energy to file
  std::ofstream energyStream;
  energyStream.open(model.basepath + model.energypath, std::ios::out | std::ios::binary);
  energyStream.write((char*)&energies[0], model.M*sizeof(energies[0]));
  energyStream.close();

  // Write magnetic moment to file
  std::ofstream magmom;
  magmom.open(model.basepath + model.magneticmomentpath, std::ios::out | std::ios::binary);
  magmom.write((char*)&magmoments[0], model.M*sizeof(magmoments[0]));
  magmom.close();

  std::ofstream evoStream;
  evoStream.open(model.basepath + model.evolutionpath, std::ios::out | std::ios::binary);
  for(size_t MCCycle = 0; MCCycle < model.M; MCCycle++){
    evoStream.write((char*)&states[MCCycle](0,0), model.N*model.N*sizeof(states[0](0,0)));
  }
  evoStream.close();
}
